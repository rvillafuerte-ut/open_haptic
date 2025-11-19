// ==============================================================================
// GRIPPER CONTROLLER IMPLEMENTATION
// ==============================================================================

#include "gripper_controller.h"
#include "dynamixel_controller.h"
#include <cstdio>

// Global state definitions
bool gripper_calibrated = false;
int32_t gripper_closed_pos_ticks = 0;
int32_t gripper_open_pos_ticks = 0;
int gripper_open_dir_state = GRIPPER_OPEN_DIR;

// Internal state
static bool gripper_is_open = false;

void configureGripper() {
    dynamixel::PacketHandler* packetHandler = getPacketHandler();
    dynamixel::PortHandler* portHandler = getPortHandler();
    
    if (!packetHandler || !portHandler) return;
    
    uint8_t dxl_error = 0;
    int dxl_comm_result;
    
    // Set to Current-based Position Control Mode (mode 5)
    uint8_t gripper_mode = 5;
    dxl_comm_result = packetHandler->write1ByteTxRx(portHandler, GRIPPER_ID,
                                                      ADDR_OPERATING_MODE, gripper_mode, &dxl_error);
    if (dxl_comm_result != COMM_SUCCESS || dxl_error != 0) {
        printf("⚠ Gripper: Mode 5 failed, trying fallback mode 3\n");
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
        printf("✗ Gripper: Failed to enable torque\n");
        return;
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
        printf("✓ Gripper calibrated: closed=%d, open=%d (ticks)\n",
               gripper_closed_pos_ticks, gripper_open_pos_ticks);
        
        // Initialize gripper to current position (no movement)
        packetHandler->write4ByteTxRx(portHandler, GRIPPER_ID,
                                      ADDR_GOAL_POSITION, (uint32_t)gripper_closed_pos_ticks, &dxl_error);
    }
}

void handleGripperControl(int buttons,
                          int& prev_buttons,
                          std::chrono::steady_clock::time_point& button_press_start) {
    
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
            printf("⟲ Gripper direction flipped: open=%d\n", gripper_open_pos_ticks);
        }
        else {
            // Short press: toggle gripper open/close
            gripper_is_open = !gripper_is_open;
            
            if (gripper_is_open) {
                openGripper();
            } else {
                closeGripper();
            }
        }
    }
    
    prev_buttons = buttons;
}

void openGripper() {
    dynamixel::PacketHandler* packetHandler = getPacketHandler();
    dynamixel::PortHandler* portHandler = getPortHandler();
    if (!packetHandler || !portHandler) return;

    uint8_t dxl_error = 0;
    packetHandler->write4ByteTxRx(portHandler, GRIPPER_ID,
                                  ADDR_GOAL_POSITION, (uint32_t)gripper_open_pos_ticks, &dxl_error);
    packetHandler->write2ByteTxRx(portHandler, GRIPPER_ID,
                                  ADDR_GOAL_CURRENT, GRIPPER_CURR_OPEN, &dxl_error);
    printf(" Gripper OPEN (pos=%d)\n", gripper_open_pos_ticks);
    gripper_is_open = true;
}

void closeGripper() {
    dynamixel::PacketHandler* packetHandler = getPacketHandler();
    dynamixel::PortHandler* portHandler = getPortHandler();
    if (!packetHandler || !portHandler) return;

    uint8_t dxl_error = 0;
    packetHandler->write4ByteTxRx(portHandler, GRIPPER_ID,
                                  ADDR_GOAL_POSITION, (uint32_t)gripper_closed_pos_ticks, &dxl_error);
    packetHandler->write2ByteTxRx(portHandler, GRIPPER_ID,
                                  ADDR_GOAL_CURRENT, GRIPPER_CURR_CLOSE, &dxl_error);
    printf(" Gripper CLOSED (pos=%d)\n", gripper_closed_pos_ticks);
    gripper_is_open = false;
}

bool isGripperOpen() {
    return gripper_is_open;
}
