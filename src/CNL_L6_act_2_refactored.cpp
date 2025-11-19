// ==============================================================================
// REFACTORED ROBOTIC ARM TELEOP CONTROL
// Modular version with separated functions for controller, trajectory, etc.
// ==============================================================================

#include <iostream>
#include <chrono>
#include <fcntl.h>
#include <termios.h>
#include <stdlib.h>
#include <stdio.h>
#include <csignal>
#include <signal.h>
#include <fstream>
#include <unistd.h>
#include <atomic>
#include <thread>
#include <cmath>

#include "dynamixel_sdk.h"
#include <Eigen/Dense>
#include <HD/hd.h>

// ==============================================================================
// CONFIGURATION & CONSTANTS
// ==============================================================================

// Dynamixel Communication
#define PROTOCOL_VERSION            2.0
#define BAUDRATE                    4500000
#define DEVICENAME                  "/dev/ttyUSB0"

// Motor IDs
#define DXL1_ID                     11
#define DXL2_ID                     12
#define DXL3_ID                     13
#define DXL4_ID                     14
#define GRIPPER_ID                  15

// Register Addresses
#define ADDR_OPERATING_MODE         11
#define ADDR_TORQUE_ENABLE          64
#define ADDR_GOAL_CURRENT           102
#define ADDR_GOAL_POSITION          116
#define ADDR_PROFILE_ACCELERATION   108
#define ADDR_PROFILE_VELOCITY       112
#define ADDR_CURRENT_LIMIT          38
#define ADDR_PRESENT_CURRENT        126
#define ADDR_PRESENT_VELOCITY       128
#define ADDR_PRESENT_POSITION       132
#define ADDR_RETURN_DELAY           9

// Data Lengths
#define LEN_GOAL_CURRENT            2
#define LEN_PRESENT_POSITION        4
#define LEN_PRESENT_VELOCITY        4
#define LEN_PRESENT_CURRENT         2
#define TOTAL_LEN_READ              10

// Control Limits
#define TORQUE_ENABLE               1
#define TORQUE_DISABLE              0
#define CURR_PEAK                   330
#define CURR_MAX                    290

// Gripper Configuration
#define GRIPPER_CURR_LIMIT          260
#define GRIPPER_CURR_OPEN           240
#define GRIPPER_CURR_CLOSE          180
#define GRIPPER_OPEN_TICKS          1400
#define GRIPPER_OPEN_DIR            (-1)

#define ESC_ASCII_VALUE             0x1b
#define STDIN_FILENO                0

// ==============================================================================
// GLOBAL STATE
// ==============================================================================

// Control flags
volatile sig_atomic_t shutdown_requested = 0;
static bool teleop_active = false;

// Gripper state
static bool gripper_calibrated = false;
static int32_t gripper_closed_pos_ticks = 0;
static int32_t gripper_open_pos_ticks = 0;
static int gripper_open_dir_state = GRIPPER_OPEN_DIR;

// Haptic device state
struct HapticSample {
    double position[3];
    double transform[16];
    double timestamp;
};

static HHD g_hDevice = HD_INVALID_HANDLE;
static std::atomic<bool> g_hSampleReady{false};
static HapticSample g_hSample;
static std::atomic<int> g_hButtons{0};

// Filter state
Eigen::Vector4d yd_filtered_state = Eigen::Vector4d::Zero();
Eigen::Vector4d yd_prev_state = Eigen::Vector4d::Zero();

// ==============================================================================
// FORWARD DECLARATIONS
// ==============================================================================

void OMDyn(const double* q, const double* dq, double* M, double* phib);

// ==============================================================================
// UTILITY FUNCTIONS
// ==============================================================================

void signalHandler(int signum) {
    shutdown_requested = 1;
}

int getch() {
    struct termios oldt, newt;
    int ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}

int kbhit(void) {
    struct termios oldt, newt;
    int ch, oldf;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);
    if (ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }
    return 0;
}

// ==============================================================================
// HAPTIC DEVICE FUNCTIONS
// ==============================================================================

static HDCallbackCode HDCALLBACK hapticsServoCallback(void* /*user*/) {
    hdBeginFrame(g_hDevice);
    hdGetDoublev(HD_CURRENT_POSITION, g_hSample.position);
    hdGetDoublev(HD_CURRENT_TRANSFORM, g_hSample.transform);
    g_hSample.timestamp = hdGetSchedulerTimeStamp();
    int buttons = 0;
    hdGetIntegerv(HD_CURRENT_BUTTONS, &buttons);
    g_hButtons.store(buttons, std::memory_order_release);
    hdEndFrame(g_hDevice);
    g_hSampleReady.store(true, std::memory_order_release);
    return HD_CALLBACK_CONTINUE;
}

bool initializeHapticDevice() {
    HDErrorInfo error;
    g_hDevice = hdInitDevice(HD_DEFAULT_DEVICE);
    if (HD_DEVICE_ERROR(error = hdGetError())) {
        printf("Failed to initialize haptic device: %s\n", hdGetErrorString(error.errorCode));
        return false;
    }
    
    hdEnable(HD_FORCE_OUTPUT);
    hdStartScheduler();
    if (HD_DEVICE_ERROR(error = hdGetError())) {
        printf("Failed to start haptic scheduler: %s\n", hdGetErrorString(error.errorCode));
        return false;
    }
    
    hdScheduleAsynchronous(hapticsServoCallback, nullptr, HD_MAX_SCHEDULER_PRIORITY);
    printf("Haptic device initialized successfully\n");
    return true;
}

void shutdownHapticDevice() {
    if (g_hDevice != HD_INVALID_HANDLE) {
        hdStopScheduler();
        hdDisableDevice(g_hDevice);
        g_hDevice = HD_INVALID_HANDLE;
        printf("Haptic device shut down\n");
    }
}

// ==============================================================================
// DYNAMIXEL MOTOR FUNCTIONS
// ==============================================================================

bool initializeDynamixel(dynamixel::PortHandler* portHandler,
                         dynamixel::PacketHandler* packetHandler) {
    
    if (!portHandler->openPort()) {
        printf("Failed to open the port!\n");
        return false;
    }
    printf("Succeeded to open the port!\n");
    
    if (!portHandler->setBaudRate(BAUDRATE)) {
        printf("Failed to change the baudrate!\n");
        return false;
    }
    printf("Succeeded to change the baudrate!\n");
    
    return true;
}

bool configurMotor(dynamixel::PacketHandler* packetHandler,
                     dynamixel::PortHandler* portHandler,
                     uint8_t motor_id,
                     uint8_t operating_mode,
                     uint16_t current_limit) {
    
    uint8_t dxl_error = 0;
    int dxl_comm_result;
    
    // Set operating mode (Current Control = 0)
    dxl_comm_result = packetHandler->write1ByteTxRx(portHandler, motor_id,
                                                      ADDR_OPERATING_MODE, operating_mode, &dxl_error);
    if (dxl_comm_result != COMM_SUCCESS || dxl_error != 0) {
        printf("Motor %d: Failed to set operating mode\n", motor_id);
        return false;
    }
    
    // Set return delay to zero
    dxl_comm_result = packetHandler->write1ByteTxRx(portHandler, motor_id,
                                                      ADDR_RETURN_DELAY, 0, &dxl_error);
    if (dxl_comm_result != COMM_SUCCESS || dxl_error != 0) {
        printf("Motor %d: Failed to set return delay\n", motor_id);
        return false;
    }
    
    // Set current limit
    dxl_comm_result = packetHandler->write2ByteTxRx(portHandler, motor_id,
                                                      ADDR_CURRENT_LIMIT, current_limit, &dxl_error);
    if (dxl_comm_result != COMM_SUCCESS || dxl_error != 0) {
        printf("Motor %d: Failed to set current limit\n", motor_id);
        return false;
    }
    
    // Enable torque
    dxl_comm_result = packetHandler->write1ByteTxRx(portHandler, motor_id,
                                                      ADDR_TORQUE_ENABLE, TORQUE_ENABLE, &dxl_error);
    if (dxl_comm_result != COMM_SUCCESS || dxl_error != 0) {
        printf("Motor %d: Failed to enable torque\n", motor_id);
        return false;
    }
    
    printf("Motor %d configured successfully\n", motor_id);
    return true;
}

bool configureGripper(dynamixel::PacketHandler* packetHandler,
                      dynamixel::PortHandler* portHandler) {
    
    uint8_t dxl_error = 0;
    int dxl_comm_result;
    
    // Set to Current-based Position Control Mode (mode 5)
    uint8_t gripper_mode = 5;
    dxl_comm_result = packetHandler->write1ByteTxRx(portHandler, GRIPPER_ID,
                                                      ADDR_OPERATING_MODE, gripper_mode, &dxl_error);
    if (dxl_comm_result != COMM_SUCCESS || dxl_error != 0) {
        printf("Gripper: Failed to set mode 5, trying fallback to mode 3\n");
        gripper_mode = 3;
        packetHandler->write1ByteTxRx(portHandler, GRIPPER_ID,
                                      ADDR_OPERATING_MODE, gripper_mode, &dxl_error);
    }
    
    // Set return delay to zero
    packetHandler->write1ByteTxRx(portHandler, GRIPPER_ID, ADDR_RETURN_DELAY, 0, &dxl_error);
    
    // Set current limit
    packetHandler->write2ByteTxRx(portHandler, GRIPPER_ID,
                                  ADDR_CURRENT_LIMIT, GRIPPER_CURR_LIMIT, &dxl_error);
    
    // Set motion profile
    packetHandler->write4ByteTxRx(portHandler, GRIPPER_ID, ADDR_PROFILE_ACCELERATION, 150, &dxl_error);
    packetHandler->write4ByteTxRx(portHandler, GRIPPER_ID, ADDR_PROFILE_VELOCITY, 400, &dxl_error);
    
    // Enable torque
    dxl_comm_result = packetHandler->write1ByteTxRx(portHandler, GRIPPER_ID,
                                                      ADDR_TORQUE_ENABLE, TORQUE_ENABLE, &dxl_error);
    if (dxl_comm_result != COMM_SUCCESS || dxl_error != 0) {
        printf("Gripper: Failed to enable torque\n");
        return false;
    }
    
    // Calibrate gripper position
    uint32_t present_pos = 0;
    dxl_comm_result = packetHandler->read4ByteTxRx(portHandler, GRIPPER_ID,
                                                     ADDR_PRESENT_POSITION, &present_pos, &dxl_error);
    if (dxl_comm_result == COMM_SUCCESS && dxl_error == 0) {
        gripper_closed_pos_ticks = (int32_t)present_pos;
        int64_t open_candidate = (int64_t)gripper_closed_pos_ticks +
                                 (int64_t)gripper_open_dir_state * (int64_t)GRIPPER_OPEN_TICKS;
        if (open_candidate < 0) open_candidate = 0;
        if (open_candidate > 4095) open_candidate = 4095;
        gripper_open_pos_ticks = (int32_t)open_candidate;
        gripper_calibrated = true;
        printf("Gripper calibrated: closed=%d, open=%d (ticks)\n",
               gripper_closed_pos_ticks, gripper_open_pos_ticks);
        
        // Initialize gripper to current position (no movement)
        packetHandler->write4ByteTxRx(portHandler, GRIPPER_ID,
                                      ADDR_GOAL_POSITION, (uint32_t)gripper_closed_pos_ticks, &dxl_error);
    }
    
    printf("Gripper configured successfully\n");
    return true;
}

void disableAllMotors(dynamixel::PacketHandler* packetHandler,
                      dynamixel::PortHandler* portHandler) {
    uint8_t dxl_error = 0;
    
    packetHandler->write1ByteTxRx(portHandler, DXL1_ID, ADDR_TORQUE_ENABLE, TORQUE_DISABLE, &dxl_error);
    packetHandler->write1ByteTxRx(portHandler, DXL2_ID, ADDR_TORQUE_ENABLE, TORQUE_DISABLE, &dxl_error);
    packetHandler->write1ByteTxRx(portHandler, DXL3_ID, ADDR_TORQUE_ENABLE, TORQUE_DISABLE, &dxl_error);
    packetHandler->write1ByteTxRx(portHandler, DXL4_ID, ADDR_TORQUE_ENABLE, TORQUE_DISABLE, &dxl_error);
    packetHandler->write1ByteTxRx(portHandler, GRIPPER_ID, ADDR_TORQUE_ENABLE, TORQUE_DISABLE, &dxl_error);
    
    printf("All motors disabled\n");
}

// ==============================================================================
// TRAJECTORY GENERATION FUNCTIONS
// ==============================================================================

/**
 * Get desired trajectory from Phantom Omni haptic device
 * Returns: yd = [x, y, z, orientation] in task space
 */
Eigen::Vector4d getTrajectoryFromHaptic(double t, float vf) {
    Eigen::Vector4d yd = Eigen::Vector4d::Zero();
    
    if (!g_hSampleReady.load(std::memory_order_acquire)) {
        return yd;  // Return zeros if no haptic data
    }
    
    // Position scaling (convert from Phantom Omni space to robot workspace)
    // Adjust these scale factors based on your calibration
    double pos_scale_x = 0.6 * vf;
    double pos_scale_y = 0.6 * vf;
    double pos_scale_z = 0.6 * vf;
    
    // Position offset (center of robot workspace)
    double offset_x = 0.18;
    double offset_y = 0.0;
    double offset_z = 0.08;
    
    yd(0) = g_hSample.position[0] * pos_scale_x + offset_x;
    yd(1) = g_hSample.position[1] * pos_scale_y + offset_y;
    yd(2) = g_hSample.position[2] * pos_scale_z + offset_z;
    
    // Orientation from haptic transform matrix
    // Extract yaw angle from rotation matrix
    double yaw = atan2(g_hSample.transform[4], g_hSample.transform[0]);
    yd(3) = yaw;
    
    return yd;
}

/**
 * Apply low-pass filter to trajectory for smooth motion
 */
Eigen::Vector4d filterTrajectory(const Eigen::Vector4d& yd_raw, float bf) {
    // First-order low-pass filter: y_filt = alpha * y_raw + (1-alpha) * y_prev
    float alpha = bf;  // Filter coefficient (0 to 1)
    
    yd_filtered_state = alpha * yd_raw + (1.0f - alpha) * yd_filtered_state;
    
    return yd_filtered_state;
}

/**
 * Reset trajectory filter state (call when arming teleop)
 */
void resetTrajectoryFilter() {
    yd_filtered_state = Eigen::Vector4d::Zero();
    yd_prev_state = Eigen::Vector4d::Zero();
}

// ==============================================================================
// CONTROLLER FUNCTIONS
// ==============================================================================

/**
 * Forward kinematics: compute task space position from joint angles
 */
Eigen::Vector4d forwardKinematics(const Eigen::Vector4d& q) {
    Eigen::Vector4d y;
    
    // Link lengths (meters) - adjust based on your robot
    double L1 = 0.10;  // Base to joint 2
    double L2 = 0.10;  // Joint 2 to joint 3
    double L3 = 0.10;  // Joint 3 to end effector
    
    // Compute forward kinematics
    double c1 = cos(q(0));
    double s1 = sin(q(0));
    double c2 = cos(q(1));
    double s2 = sin(q(1));
    double c23 = cos(q(1) + q(2));
    double s23 = sin(q(1) + q(2));
    
    // Position
    y(0) = c1 * (L2 * c2 + L3 * c23);
    y(1) = s1 * (L2 * c2 + L3 * c23);
    y(2) = L1 + L2 * s2 + L3 * s23;
    
    // Orientation (yaw)
    y(3) = q(0) + q(1) + q(2) + q(3);
    
    return y;
}

/**
 * Jacobian: relates joint velocities to task space velocities
 */
Eigen::Matrix4d computeJacobian(const Eigen::Vector4d& q) {
    Eigen::Matrix4d J = Eigen::Matrix4d::Zero();
    
    // Analytical Jacobian computation
    // This is a simplified version - replace with your actual robot's Jacobian
    double L2 = 0.10;
    double L3 = 0.10;
    
    double c1 = cos(q(0));
    double s1 = sin(q(0));
    double c2 = cos(q(1));
    double s2 = sin(q(1));
    double c23 = cos(q(1) + q(2));
    double s23 = sin(q(1) + q(2));
    
    // J(0,:) - dx/dq
    J(0, 0) = -s1 * (L2 * c2 + L3 * c23);
    J(0, 1) = c1 * (-L2 * s2 - L3 * s23);
    J(0, 2) = c1 * (-L3 * s23);
    J(0, 3) = 0;
    
    // J(1,:) - dy/dq
    J(1, 0) = c1 * (L2 * c2 + L3 * c23);
    J(1, 1) = s1 * (-L2 * s2 - L3 * s23);
    J(1, 2) = s1 * (-L3 * s23);
    J(1, 3) = 0;
    
    // J(2,:) - dz/dq
    J(2, 0) = 0;
    J(2, 1) = L2 * c2 + L3 * c23;
    J(2, 2) = L3 * c23;
    J(2, 3) = 0;
    
    // J(3,:) - dorientation/dq
    J(3, 0) = 1;
    J(3, 1) = 1;
    J(3, 2) = 1;
    J(3, 3) = 1;
    
    return J;
}

/**
 * Main control law: Computed Torque Control with task space tracking
 */
void computeControl(const Eigen::Vector4d& q,
                    const Eigen::Vector4d& dq,
                    const Eigen::Vector4d& yd,
                    float lambda,
                    float DZ,
                    int16_t& curr_d1,
                    int16_t& curr_d2,
                    int16_t& curr_d3,
                    int16_t& curr_d4) {
    
    // Compute dynamics
    double M[16], phib[4];
    OMDyn(q.data(), dq.data(), M, phib);
    Eigen::Matrix4d M_mat = Eigen::Map<Eigen::Matrix4d>(M);
    Eigen::Vector4d phib_vec = Eigen::Map<Eigen::Vector4d>(phib);
    
    // Forward kinematics
    Eigen::Vector4d y = forwardKinematics(q);
    
    // Tracking error
    Eigen::Vector4d e = yd - y;
    
    // Jacobian
    Eigen::Matrix4d J = computeJacobian(q);
    
    // Task space velocity
    Eigen::Vector4d dy = J * dq;
    
    // Control law (PD + feedforward)
    double Kp = lambda * 10.0;  // Proportional gain
    double Kd = DZ * 2.0;        // Derivative gain
    
    Eigen::Vector4d tau_task = Kp * e - Kd * dy;
    
    // Map to joint space
    Eigen::Vector4d tau = J.transpose() * tau_task + phib_vec;
    
    // Convert torque to current (simplified)
    // tau = kt * current, where kt is motor torque constant
    double kt = 0.1;  // Adjust based on motor specs
    
    curr_d1 = (int16_t)(tau(0) / kt);
    curr_d2 = (int16_t)(tau(1) / kt);
    curr_d3 = (int16_t)(tau(2) / kt);
    curr_d4 = (int16_t)(tau(3) / kt);
    
    // Apply current limits
    if (curr_d1 > CURR_MAX) curr_d1 = CURR_MAX;
    if (curr_d1 < -CURR_MAX) curr_d1 = -CURR_MAX;
    if (curr_d2 > CURR_MAX) curr_d2 = CURR_MAX;
    if (curr_d2 < -CURR_MAX) curr_d2 = -CURR_MAX;
    if (curr_d3 > CURR_MAX) curr_d3 = CURR_MAX;
    if (curr_d3 < -CURR_MAX) curr_d3 = -CURR_MAX;
    if (curr_d4 > (int16_t)(0.8 * CURR_MAX)) curr_d4 = (int16_t)(0.8 * CURR_MAX);
    if (curr_d4 < -(int16_t)(0.8 * CURR_MAX)) curr_d4 = -(int16_t)(0.8 * CURR_MAX);
}

// ==============================================================================
// GRIPPER CONTROL FUNCTIONS
// ==============================================================================

void handleGripperControl(dynamixel::PacketHandler* packetHandler,
                          dynamixel::PortHandler* portHandler,
                          int buttons,
                          static int& prev_buttons,
                          static auto& button_press_start) {
    
    static bool gripper_is_open = false;
    const int BTN2_MASK = 0x02;  // Button 2
    
    // Check for button 2 press
    if ((buttons & BTN2_MASK) && !(prev_buttons & BTN2_MASK)) {
        // Button 2 just pressed
        button_press_start = std::chrono::steady_clock::now();
    }
    else if (!(buttons & BTN2_MASK) && (prev_buttons & BTN2_MASK)) {
        // Button 2 just released
        auto press_duration = std::chrono::steady_clock::now() - button_press_start;
        double duration_sec = std::chrono::duration<double>(press_duration).count();
        
        if (duration_sec > 0.8) {
            // Long press: flip gripper direction
            gripper_open_dir_state *= -1;
            int64_t open_candidate = (int64_t)gripper_closed_pos_ticks +
                                     (int64_t)gripper_open_dir_state * (int64_t)GRIPPER_OPEN_TICKS;
            if (open_candidate < 0) open_candidate = 0;
            if (open_candidate > 4095) open_candidate = 4095;
            gripper_open_pos_ticks = (int32_t)open_candidate;
            printf("Gripper direction flipped: open=%d\n", gripper_open_pos_ticks);
        }
        else {
            // Short press: toggle gripper open/close
            gripper_is_open = !gripper_is_open;
            
            uint32_t goal_pos = gripper_is_open ? gripper_open_pos_ticks : gripper_closed_pos_ticks;
            uint16_t goal_curr = gripper_is_open ? GRIPPER_CURR_OPEN : GRIPPER_CURR_CLOSE;
            
            uint8_t dxl_error = 0;
            packetHandler->write4ByteTxRx(portHandler, GRIPPER_ID,
                                          ADDR_GOAL_POSITION, goal_pos, &dxl_error);
            packetHandler->write2ByteTxRx(portHandler, GRIPPER_ID,
                                          ADDR_GOAL_CURRENT, goal_curr, &dxl_error);
            
            printf("Gripper %s: pos=%d, curr=%d\n",
                   gripper_is_open ? "OPEN" : "CLOSED", goal_pos, goal_curr);
        }
    }
    
    prev_buttons = buttons;
}

// ==============================================================================
// SENSOR READING FUNCTIONS
// ==============================================================================

void readMotorStates(dynamixel::GroupFastSyncRead& groupSyncRead,
                     int32_t& dxl1_pos, int32_t& dxl2_pos, int32_t& dxl3_pos, int32_t& dxl4_pos,
                     int32_t& dxl1_vel, int32_t& dxl2_vel, int32_t& dxl3_vel, int32_t& dxl4_vel,
                     int16_t& dxl1_cur, int16_t& dxl2_cur, int16_t& dxl3_cur, int16_t& dxl4_cur) {
    
    groupSyncRead.txRxPacket();
    
    // Motor 1
    dxl1_cur = (int16_t)groupSyncRead.getData(DXL1_ID, ADDR_PRESENT_CURRENT, LEN_PRESENT_CURRENT);
    dxl1_vel = (int32_t)groupSyncRead.getData(DXL1_ID, ADDR_PRESENT_VELOCITY, LEN_PRESENT_VELOCITY);
    dxl1_pos = (int32_t)groupSyncRead.getData(DXL1_ID, ADDR_PRESENT_POSITION, LEN_PRESENT_POSITION);
    
    // Motor 2
    dxl2_cur = (int16_t)groupSyncRead.getData(DXL2_ID, ADDR_PRESENT_CURRENT, LEN_PRESENT_CURRENT);
    dxl2_vel = (int32_t)groupSyncRead.getData(DXL2_ID, ADDR_PRESENT_VELOCITY, LEN_PRESENT_VELOCITY);
    dxl2_pos = (int32_t)groupSyncRead.getData(DXL2_ID, ADDR_PRESENT_POSITION, LEN_PRESENT_POSITION);
    
    // Motor 3
    dxl3_cur = (int16_t)groupSyncRead.getData(DXL3_ID, ADDR_PRESENT_CURRENT, LEN_PRESENT_CURRENT);
    dxl3_vel = (int32_t)groupSyncRead.getData(DXL3_ID, ADDR_PRESENT_VELOCITY, LEN_PRESENT_VELOCITY);
    dxl3_pos = (int32_t)groupSyncRead.getData(DXL3_ID, ADDR_PRESENT_POSITION, LEN_PRESENT_POSITION);
    
    // Motor 4
    dxl4_cur = (int16_t)groupSyncRead.getData(DXL4_ID, ADDR_PRESENT_CURRENT, LEN_PRESENT_CURRENT);
    dxl4_vel = (int32_t)groupSyncRead.getData(DXL4_ID, ADDR_PRESENT_VELOCITY, LEN_PRESENT_VELOCITY);
    dxl4_pos = (int32_t)groupSyncRead.getData(DXL4_ID, ADDR_PRESENT_POSITION, LEN_PRESENT_POSITION);
}

void convertToJointSpace(int32_t dxl1_pos, int32_t dxl2_pos, int32_t dxl3_pos, int32_t dxl4_pos,
                         int32_t dxl1_vel, int32_t dxl2_vel, int32_t dxl3_vel, int32_t dxl4_vel,
                         Eigen::Vector4d& q, Eigen::Vector4d& dq) {
    
    // Convert Dynamixel positions to radians
    // Dynamixel range: 0-4095 corresponds to 0-360 degrees
    const double ticks_to_rad = 2.0 * M_PI / 4095.0;
    const double vel_to_rad_s = ticks_to_rad * 0.229;  // Velocity unit conversion
    
    q(0) = (dxl1_pos - 2048) * ticks_to_rad;
    q(1) = (dxl2_pos - 2048) * ticks_to_rad;
    q(2) = (dxl3_pos - 2048) * ticks_to_rad;
    q(3) = (dxl4_pos - 2048) * ticks_to_rad;
    
    dq(0) = dxl1_vel * vel_to_rad_s;
    dq(1) = dxl2_vel * vel_to_rad_s;
    dq(2) = dxl3_vel * vel_to_rad_s;
    dq(3) = dxl4_vel * vel_to_rad_s;
}

// ==============================================================================
// SAFETY FUNCTIONS
// ==============================================================================

bool checkSafety(int16_t dxl1_cur, int16_t dxl2_cur, int16_t dxl3_cur, int16_t dxl4_cur,
                 const Eigen::Vector4d& y, const Eigen::Vector4d& yd) {
    
    // Check current limits
    if (abs(dxl1_cur) > CURR_PEAK || abs(dxl2_cur) > CURR_PEAK ||
        abs(dxl3_cur) > CURR_PEAK || abs(dxl4_cur) > CURR_PEAK) {
        
        printf("\n[UNSAFE] Current limit exceeded: [%d %d %d %d]\n",
               dxl1_cur, dxl2_cur, dxl3_cur, dxl4_cur);
        printf("  Position: [%.4f %.4f %.4f %.4f]\n", y(0), y(1), y(2), y(3));
        printf("  Desired:  [%.4f %.4f %.4f %.4f]\n", yd(0), yd(1), yd(2), yd(3));
        printf("  Error:    [%.4f %.4f %.4f %.4f]\n",
               y(0)-yd(0), y(1)-yd(1), y(2)-yd(2), y(3)-yd(3));
        
        return false;
    }
    
    return true;
}

// ==============================================================================
// MAIN CONTROL LOOP
// ==============================================================================

int main(int argc, char* argv[]) {
    
    // Parse command line arguments
    if (argc != 5 && argc != 10) {
        printf("Usage: %s <lambda> <DZ> <bf_x10> <vf> [sf1 sf2 sf3 sf4 ff]\n", argv[0]);
        printf("  lambda: Control gain (0-90)\n");
        printf("  DZ: Damping zone (0-60)\n");
        printf("  bf_x10: Filter coefficient x10 (0-25)\n");
        printf("  vf: Velocity scale factor\n");
        return 0;
    }
    
    float lambda = atof(argv[1]);
    float DZ = atof(argv[2]);
    int bff = atoi(argv[3]);
    float vf = atof(argv[4]);
    float bf = (float)bff / 10.0f;
    
    printf("=== Control Parameters ===\n");
    printf("lambda: %.2f, DZ: %.2f, bf: %.2f, vf: %.2f\n", lambda, DZ, bf, vf);
    
    if (lambda < 0 || lambda > 90 || DZ < 0 || DZ > 60 || bff > 25) {
        printf("Invalid parameters!\n");
        return 0;
    }
    
    // Initialize signal handler
    std::signal(SIGINT, signalHandler);
    
    // Initialize Dynamixel communication
    dynamixel::PortHandler* portHandler = dynamixel::PortHandler::getPortHandler(DEVICENAME);
    dynamixel::PacketHandler* packetHandler = dynamixel::PacketHandler::getPacketHandler(PROTOCOL_VERSION);
    dynamixel::GroupSyncWrite groupSyncWriteCurr(portHandler, packetHandler, ADDR_GOAL_CURRENT, LEN_GOAL_CURRENT);
    dynamixel::GroupFastSyncRead groupSyncRead(portHandler, packetHandler, ADDR_PRESENT_CURRENT, TOTAL_LEN_READ);
    
    if (!initializeDynamixel(portHandler, packetHandler)) {
        printf("Press any key to terminate...\n");
        getch();
        return 0;
    }
    
    // Configure all motors
    if (!configurMotor(packetHandler, portHandler, DXL1_ID, 0, 300) ||
        !configurMotor(packetHandler, portHandler, DXL2_ID, 0, 300) ||
        !configurMotor(packetHandler, portHandler, DXL3_ID, 0, 300) ||
        !configurMotor(packetHandler, portHandler, DXL4_ID, 0, 300)) {
        printf("Failed to configure motors!\n");
        return 0;
    }
    
    // Configure gripper
    if (!configureGripper(packetHandler, portHandler)) {
        printf("Failed to configure gripper!\n");
        return 0;
    }
    
    // Add motors to sync read group
    groupSyncRead.addParam(DXL1_ID);
    groupSyncRead.addParam(DXL2_ID);
    groupSyncRead.addParam(DXL3_ID);
    groupSyncRead.addParam(DXL4_ID);
    
    // Initialize haptic device
    if (!initializeHapticDevice()) {
        printf("Warning: Haptic device not initialized. Continuing without haptic input.\n");
    }
    
    // Open log file
    std::ofstream logfile("data_log.txt", std::ios::out);
    if (!logfile.is_open()) {
        std::cerr << "Error opening log file!\n";
        return 1;
    }
    
    printf("\n=== System Ready ===\n");
    printf("Press Button 1 to ARM/DISARM teleop\n");
    printf("Press Button 2 to control gripper\n");
    printf("Press ESC to quit\n");
    printf("Press any key to start...\n");
    
    if (getch() == ESC_ASCII_VALUE) {
        disableAllMotors(packetHandler, portHandler);
        shutdownHapticDevice();
        portHandler->closePort();
        return 0;
    }
    
    // Control loop variables
    Eigen::Vector4d q = Eigen::Vector4d::Zero();
    Eigen::Vector4d dq = Eigen::Vector4d::Zero();
    int32_t dxl1_pos = 0, dxl2_pos = 0, dxl3_pos = 0, dxl4_pos = 0;
    int32_t dxl1_vel = 0, dxl2_vel = 0, dxl3_vel = 0, dxl4_vel = 0;
    int16_t dxl1_cur = 0, dxl2_cur = 0, dxl3_cur = 0, dxl4_cur = 0;
    int16_t curr_d1 = 0, curr_d2 = 0, curr_d3 = 0, curr_d4 = 0;
    
    int prev_buttons = 0;
    auto button_press_start = std::chrono::steady_clock::now();
    
    auto start_time = std::chrono::steady_clock::now();
    int loop_counter = 0;
    const int DISPLAY_INTERVAL = 10;
    const int LOG_FLUSH_INTERVAL = 1000;
    
    printf("\n=== Control Loop Started ===\n");
    
    // Main control loop
    do {
        // Timing
        auto current_time = std::chrono::steady_clock::now();
        double t = std::chrono::duration<double>(current_time - start_time).count();
        
        // Read motor states
        readMotorStates(groupSyncRead,
                        dxl1_pos, dxl2_pos, dxl3_pos, dxl4_pos,
                        dxl1_vel, dxl2_vel, dxl3_vel, dxl4_vel,
                        dxl1_cur, dxl2_cur, dxl3_cur, dxl4_cur);
        
        // Convert to joint space
        convertToJointSpace(dxl1_pos, dxl2_pos, dxl3_pos, dxl4_pos,
                            dxl1_vel, dxl2_vel, dxl3_vel, dxl4_vel,
                            q, dq);
        
        // Get haptic buttons
        int buttons = g_hButtons.load(std::memory_order_acquire);
        
        // Handle button 1: ARM/DISARM teleop
        const int BTN1_MASK = 0x01;
        static int prev_btn1 = 0;
        if ((buttons & BTN1_MASK) && !(prev_btn1 & BTN1_MASK)) {
            teleop_active = !teleop_active;
            if (teleop_active) {
                resetTrajectoryFilter();
                printf("\n[ARMED] Teleop control ACTIVE\n");
            } else {
                printf("\n[DISARMED] Teleop control INACTIVE\n");
            }
        }
        prev_btn1 = buttons & BTN1_MASK;
        
        // Handle gripper control
        handleGripperControl(packetHandler, portHandler, buttons, prev_buttons, button_press_start);
        
        // Compute control if teleop is active
        if (teleop_active) {
            // Get desired trajectory from haptic device
            Eigen::Vector4d yd_raw = getTrajectoryFromHaptic(t, vf);
            Eigen::Vector4d yd = filterTrajectory(yd_raw, bf);
            
            // Compute control currents
            computeControl(q, dq, yd, lambda, DZ, curr_d1, curr_d2, curr_d3, curr_d4);
            
            // Check safety
            Eigen::Vector4d y = forwardKinematics(q);
            if (!checkSafety(dxl1_cur, dxl2_cur, dxl3_cur, dxl4_cur, y, yd)) {
                shutdown_requested = 1;
            }
            
            // Display status
            if (loop_counter % DISPLAY_INTERVAL == 0) {
                printf("\r\033[K");  // Clear line
                printf("X: %+.3f→%+.3f  Y: %+.3f→%+.3f  Z: %+.3f→%+.3f  θ: %+.2f→%+.2f",
                       (float)y(0), (float)yd(0),
                       (float)y(1), (float)yd(1),
                       (float)y(2), (float)yd(2),
                       (float)y(3), (float)yd(3));
                fflush(stdout);
            }
            
            // Log data
            logfile << t << ", "
                    << y(0) << ", " << y(1) << ", " << y(2) << ", " << y(3) << ", "
                    << yd(0) << ", " << yd(1) << ", " << yd(2) << ", " << yd(3) << ", "
                    << dq(0) << ", " << dq(1) << ", " << dq(2) << ", " << dq(3) << ", "
                    << dxl1_cur << ", " << dxl2_cur << ", " << dxl3_cur << ", " << dxl4_cur << "\n";
            
            if (loop_counter % LOG_FLUSH_INTERVAL == 0) {
                logfile.flush();
            }
        }
        else {
            // Teleop inactive: send zero current
            curr_d1 = curr_d2 = curr_d3 = curr_d4 = 0;
        }
        
        // Send currents to motors
        uint8_t param1[2] = {(uint8_t)(curr_d1 & 0xFF), (uint8_t)((curr_d1 >> 8) & 0xFF)};
        uint8_t param2[2] = {(uint8_t)(curr_d2 & 0xFF), (uint8_t)((curr_d2 >> 8) & 0xFF)};
        uint8_t param3[2] = {(uint8_t)(curr_d3 & 0xFF), (uint8_t)((curr_d3 >> 8) & 0xFF)};
        uint8_t param4[2] = {(uint8_t)(curr_d4 & 0xFF), (uint8_t)((curr_d4 >> 8) & 0xFF)};
        
        groupSyncWriteCurr.addParam(DXL1_ID, param1);
        groupSyncWriteCurr.addParam(DXL2_ID, param2);
        groupSyncWriteCurr.addParam(DXL3_ID, param3);
        groupSyncWriteCurr.addParam(DXL4_ID, param4);
        groupSyncWriteCurr.txPacket();
        groupSyncWriteCurr.clearParam();
        
        loop_counter++;
        
    } while (!shutdown_requested);
    
    // Cleanup
    printf("\n\n=== Shutting down ===\n");
    disableAllMotors(packetHandler, portHandler);
    shutdownHapticDevice();
    logfile.close();
    portHandler->closePort();
    
    printf("Shutdown complete. Goodbye!\n");
    return 0;
}

// ==============================================================================
// DYNAMICS FUNCTION (AUTO-GENERATED - DO NOT MODIFY)
// ==============================================================================

void OMDyn(const double* q, const double* dq, double* M, double* phib) {
  double t10;
  double t100;
  double t101;
  double t104;
  double t106;
  double t107;
  double t11;
  double t115;
  double t116_tmp;
  double t12;
  double t15;
  double t16;
  double t17;
  double t18;
  double t19;
  double t2;
  double t20;
  double t21;
  double t22;
  double t23;
  double t24;
  double t25;
  double t26;
  double t27;
  double t28;
  double t29;
  double t3;
  double t30;
  double t31;
  double t32;
  double t33;
  double t34;
  double t35;
  double t36;
  double t37;
  double t38;
  double t4;
  double t46;
  double t48_tmp;
  double t49_tmp;
  double t5;
  double t50;
  double t52;
  double t52_tmp;
  double t53;
  double t53_tmp;
  double t6;
  double t61;
  double t62;
  double t64;
  double t64_tmp;
  double t66;
  double t67;
  double t67_tmp;
  double t68;
  double t68_tmp;
  double t7;
  double t72;
  double t73;
  double t76;
  double t76_tmp;
  double t8;
  double t80;
  double t80_tmp;
  double t82;
  double t82_tmp;
  double t9;
  double t98;
  t2 = cos(q[2]);
  t3 = cos(q[3]);
  t4 = sin(q[2]);
  t5 = sin(q[3]);
  t6 = q[1] + q[2];
  t7 = q[2] + q[3];
  t8 = dq[0] * dq[0];
  t9 = dq[1] * dq[1];
  t10 = dq[2] * dq[2];
  t11 = dq[3] * dq[3];
  t12 = q[1] * 2.0;
  t15 = cos(t12);
  t16 = sin(t12);
  t17 = cos(t6);
  t18 = cos(t7);
  t19 = q[3] + t6;
  t20 = sin(t6);
  t21 = sin(t7);
  t24 = q[1] + t6;
  t28 = t6 * 2.0;
  t46 = t5 * 0.0003578747437347655;
  t48_tmp = dq[1] * dq[3];
  t49_tmp = dq[2] * dq[3];
  t50 = t3 * 0.0035320003205986479;
  t52_tmp = t2 * t3;
  t52 = t52_tmp * 0.00052651602779200609;
  t53_tmp = t4 * t5;
  t53 = t53_tmp * 0.00052651602779200609;
  t67_tmp = t2 * t5;
  t67 = t67_tmp * 0.0017883348676894859;
  t68_tmp = t3 * t4;
  t68 = t68_tmp * 0.0017883348676894859;
  t22 = cos(t19);
  t23 = sin(t19);
  t25 = cos(t24);
  t26 = q[1] + t19;
  t27 = sin(t24);
  t31 = cos(t28);
  t32 = t6 + t19;
  t33 = sin(t28);
  t36 = q[3] * 2.0 + t28;
  t61 = t8 * t21 * 0.0002632580138960031;
  t62 = t9 * t21 * 0.00052651602779200609;
  t66 = -(t5 * t11 * 0.0017660001602993239);
  t72 = t8 * t18 * 0.000894167433844743;
  t73 = t9 * t18 * 0.0017883348676894859;
  t107 = (t5 * 0.0001789373718673827 + t3 * 0.0017660001602993239) +
         0.0010777913814770429;
  t29 = cos(t26);
  t30 = sin(t26);
  t34 = cos(t32);
  t35 = sin(t32);
  t37 = cos(t36);
  t38 = sin(t36);
  t104 = t8 * t33 * 0.0024776578125361871;
  t115 = (((t52 - t53) - t67) - t68) + t107;
  t116_tmp = t46 + t50;
  t67 = ((((((t116_tmp + t52) - t53) - t67) - t68) + t2 * 0.00100240441456248) -
         t4 * 0.0053461223198367682) +
        0.0060492094805849328;
  t64_tmp = t8 * t30;
  t64 = t64_tmp * 7.8548468742575741E-5;
  t80_tmp = t8 * t35;
  t80 = t80_tmp * 0.0017660001602993239;
  t100 = -(t8 * t31 * 2.7331035618357459E-9);
  t101 = t8 * t38 * 0.0004727803062273704;
  t68 = 1.359372084870438E+50 * cos(t19 + 0.017960143170678091) /
        6.1299821634635554E+56;
  t76_tmp = t8 * t34;
  t76 = -(t76_tmp * 0.0001789373718673827);
  t82_tmp = t8 * t29;
  t82 = -(t82_tmp * 0.00092880047356101074);
  t98 = -(t8 * t37 * 6.4649199983927934E-5);
  t19 = -(2.1899226517451251E+18 * sin(t19 + 1.4698174200878422) *
          6.4124981399849019E-20);
  t106 = -(cos(t6 - 1.17085425394026E-6) *
           4.3569993082336428E+39 / 1.0633823966279329E+40);
  t52 = -t68 - cos(t6 + 1.5707931241587512) *
                   5.3080995482529569E+54 / 7.846377169233351E+59;
  t53 = -(cos(q[1] + 0.14643291991477048) *
          3.6912047596486679E+50 / 2.3945242826029511E+55) +
        t52;
  M[0] =
      (((5.7840415964172063E+40 * cos(t24 - 1.3854495476534558) /
             1.0633823966279329E+43 +
         5.7840415964172063E+40 *
             cos(q[2] + 1.3854472059449479) /
             1.0633823966279329E+43) +
        (3.5649357967093821E+19 * cos(t26 - 1.4864272835062562) *
             5.2293562813332533E-23 +
         9.9536064990558271E+62 *
             (cos(t28 - 1.1030996887487746E-6) /
              4.0173451106474762E+65))) +
       (3.5649357967093821E+19 * cos(t7 + 1.2844694700921475) *
            5.2293562813332533E-23 +
        2.1899226517451251E+18 *
            cos(q[3] - 0.10097890670705442) *
            8.1055022360665421E-22)) +
      (((2.1899226517451251E+18 * cos(t32 - 0.10097890670705442) *
             8.1055022360665421E-22 +
         2.231677848771418E+51 * (cos(t36 - 0.13589972325856114) /
                                  4.6768052394588893E+54)) +
        3.017617666849213E+53 * (cos(t12 + 0.32877657078635497) *
                                 -1.3363823550460979E-56)) +
       0.0072605484133183859);
  M[1] = t53;
  M[2] = t52;
  M[3] = -t68;
  M[4] = t53;
  M[5] = (((((t2 * 0.0020048088291249608 - t4 * 0.01069224463967354) + t46) +
            t50) +
           (t52_tmp * 0.001053032055584012 - t67_tmp * 0.0035766697353789719)) +
          (t68_tmp * -0.0035766697353789719 - t53_tmp * 0.001053032055584012)) +
         0.014183673618319671;
  M[6] = t67;
  M[7] = t115;
  M[8] = t52;
  M[9] = t67;
  M[10] = t116_tmp + 0.0060492094805849328;
  M[11] = t107;
  M[12] = -t68;
  M[13] = t115;
  M[14] = t107;
  M[15] = 0.0010777913814770429;
  t52 = dq[0] * dq[2];
  t53 = dq[0] * dq[3];
  t67 = dq[0] * dq[1];
  t68 = dq[1] * dq[2];
  phib[0] =
      (((((((((((((((((((((((((((((((((((((t9 * t17 * 6.7650323630113937E-6 +
                                           t10 * t17 * 6.7650323630113937E-6) +
                                          t9 * t20 * 2.1665937170605689E-11) +
                                         t10 * t20 * 2.1665937170605689E-11) +
                                        t9 * t22 * 3.98258984851377E-9) +
                                       t9 * t23 * 2.2172215357361789E-7) +
                                      t10 * t22 * 3.98258984851377E-9) +
                                     t10 * t23 * 2.2172215357361789E-7) +
                                    t11 * t22 * 3.98258984851377E-9) +
                                   t11 * t23 * 2.2172215357361789E-7) +
                                  t9 * cos(q[1]) *
                                      2.2492329448778688E-6) +
                                 t9 * sin(q[1]) *
                                     1.5250214570970449E-5) +
                                (t52 * t2 * -0.0053461223198367682 -
                                 t52 * t4 * 0.00100240441456248)) +
                               (t53 * t3 * 0.0001789373718673827 -
                                t53 * t5 * 0.0017660001602993239)) +
                              t67 * t15 * 0.0026041938566917981) +
                             (t67 * t16 * 0.0076333846555699039 -
                              t52 * t18 * 0.0017883348676894859)) +
                            (t68 * t17 * 1.3530064726022791E-5 -
                             t53 * t18 * 0.0017883348676894859)) +
                           (t52 * t21 * -0.00052651602779200609 +
                            t68 * t20 * 4.3331874341211379E-11)) +
                          (t53 * t21 * -0.00052651602779200609 +
                           t68 * t22 * 7.96517969702754E-9)) +
                         (t67 * t25 * 0.010692249334322111 +
                          t68 * t23 * 4.4344430714723578E-7)) +
                        (t48_tmp * t22 * 7.96517969702754E-9 +
                         t52 * t25 * 0.0053461246671610563)) +
                       t48_tmp * t23 * 4.4344430714723578E-7) +
                      (t49_tmp * t22 * 7.96517969702754E-9 -
                       t67 * t27 * 0.0020047837909992232)) +
                     (t49_tmp * t23 * 4.4344430714723578E-7 -
                      t52 * t27 * 0.001002391895499612)) +
                    (t67 * t29 * 0.003715201894244043 -
                     t67 * t30 * 0.000314193874970303)) +
                   (t52 * t29 * 0.0018576009471220211 +
                    t67 * t31 * 5.4662071236714919E-9)) +
                  (t52 * t30 * -0.00015709693748515151 +
                   t53 * t29 * 0.0018576009471220211)) +
                 (t52 * t31 * 5.4662071236714919E-9 -
                  t53 * t30 * 0.00015709693748515151)) +
                t67 * t33 * -0.0049553156250723741) +
               t67 * t34 * 0.0003578747437347655) +
              t52 * t33 * -0.0049553156250723741) +
             (t67 * t35 * -0.0035320003205986479 +
              t52 * t34 * 0.0003578747437347655)) +
            (t52 * t35 * -0.0035320003205986479 +
             t53 * t34 * 0.0001789373718673827)) +
           (t67 * t37 * 0.00012929839996785589 -
            t53 * t35 * 0.0017660001602993239)) +
          t67 * t38 * -0.00094556061245474081) +
         t52 * t37 * 0.00012929839996785589) +
        t52 * t38 * -0.00094556061245474081) +
       t53 * t37 * 0.00012929839996785589) +
      t53 * t38 * -0.00094556061245474081;
  t52 = (((t48_tmp * t3 * 0.0003578747437347655 +
           t49_tmp * t3 * 0.0003578747437347655) +
          t3 * t11 * 0.0001789373718673827) -
         t48_tmp * t5 * 0.0035320003205986479) -
        t49_tmp * t5 * 0.0035320003205986479;
  t53 = t8 * t25;
  t67 = t8 * t27;
  phib[1] =
      (((((((((((((((((((((t52 + t66) + t76) + t80) + t98) + t100) + t101) +
                     t19) +
                    t104) +
                   t106) -
                  t2 * t10 * 0.0053461223198367682) -
                 t4 * t10 * 0.00100240441456248) +
                t8 * t15 * -0.0013020969283458991) +
               (t8 * t16 * -0.003816692327784952 -
                t10 * t18 * 0.0017883348676894859)) +
              (t11 * t18 * -0.0017883348676894859 -
               t10 * t21 * 0.00052651602779200609)) +
             (t11 * t21 * -0.00052651602779200609 -
              t53 * 0.0053461246671610563)) +
            (t67 * 0.001002391895499612 - t82_tmp * 0.0018576009471220211)) +
           t64_tmp * 0.00015709693748515151) +
          4.0301533745564482E+34 *
              cos(q[1] - 1.4091303705547358) *
              -1.540743955509789E-35) +
         (t68 * t2 * -0.01069224463967354 - t68 * t4 * 0.0020048088291249608)) +
        (t68 * t18 * -0.0035766697353789719 -
         t48_tmp * t18 * 0.0035766697353789719)) +
       (t49_tmp * t18 * -0.0035766697353789719 -
        t68 * t21 * 0.001053032055584012)) +
      (t48_tmp * t21 * -0.001053032055584012 -
       t49_tmp * t21 * 0.001053032055584012);
  phib[2] =
      ((((((((((((((((((t52 + t61) + t62) + t64) + t66) + t72) + t73) + t76) +
                 t80) +
                t82) +
               t98) +
              t100) +
             t101) +
            t19) +
           t104) +
          t106) +
         t2 * t8 * 0.0026730611599183841) +
        t2 * t9 * 0.0053461223198367682) +
       (t4 * t8 * 0.0005012022072812402 + t4 * t9 * 0.00100240441456248)) +
      (t53 * -0.0026730623335805281 + t67 * 0.00050119594774980579);
  phib[3] =
      (((((((((((((t61 + t62) + t64) + t72) + t73) + t82) + t98) + t101) +
            t19) -
           t3 * t8 * 8.9468685933691362E-5) -
          t3 * t9 * 0.0001789373718673827) +
         (t3 * t10 * -0.0001789373718673827 +
          t5 * t8 * 0.00088300008014966186)) +
        (t5 * t9 * 0.0017660001602993239 + t5 * t10 * 0.0017660001602993239)) +
       (t76_tmp * -8.9468685933691362E-5 + t80_tmp * 0.00088300008014966186)) +
      (t68 * t3 * -0.0003578747437347655 + t68 * t5 * 0.0035320003205986479);
}
