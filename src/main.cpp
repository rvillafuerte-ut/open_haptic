// ==============================================================================
// MAIN CONTROL LOOP
// ==============================================================================

#include <iostream>
#include <thread>
#include <chrono>
#include <cmath>
#include <csignal>

// Include modular headers
#include "dynamixel_controller.h"
#include "haptic_controller.h"
#include "gripper_controller.h"
#include "robot_controller.h"
#include "trajectory_generator.h"
#include "safety_monitor.h"
#include "utilities.h"

int main(int argc, char* argv[]) {
    // Parse command line arguments (same as original)
    if(argc != 5 && argc != 10) {
        printf("Usage: %s <lambda> <DZ> <bf*10> <vf> [<sf1> <sf2> <sf3> <sf4> <ff>]\n", argv[0]);
        printf("Example: %s 25 0 10 0.9\n", argv[0]);
        return 0;
    }
    
    float lambda = atof(argv[1]);
    float DZ = atof(argv[2]);
    int bff = atoi(argv[3]);
    float vf = atof(argv[4]);
    float bf = (float)bff / 10.f;
    
    printf("lambda %f DZ %f bf %f vf %f\n", lambda, DZ, bf, vf);
    
    if(lambda < 0 || lambda > 90 || DZ < 0 || DZ > 60 || bff > 25) {
        printf("Invalid lambda or DZ\n");
        return 0;
    }
    
    double sf1 = 0.9, sf2 = 0.9, sf3 = 0.9, sf4 = 0.9;
    double ff = 1.0;
    if(argc == 10) {
        sf1 = atof(argv[5]);
        sf2 = atof(argv[6]);
        sf3 = atof(argv[7]);
        sf4 = atof(argv[8]);
        ff = atof(argv[9]);
    }
    
    // 1. Initialization
    printStartupBanner();
    
    // Setup signal handler for Ctrl+C
    signal(SIGINT, signalHandler);

    // Initialize Dynamixel Motors
    if (!initializeDynamixel()) {
        printf("Error: Failed to initialize Dynamixel motors.\n");
        return 1;
    }

    // Initialize Haptic Device
    if (!initializeHapticDevice()) {
        printf("Error: Failed to initialize Haptic device.\n");
        // Close Dynamixel before exiting
        disableAllMotors(); 
        return 1;
    }

    // Configure Motors (IDs 11-14)
    configureMotor(11);
    configureMotor(12);
    configureMotor(13);
    configureMotor(14);
    
    // Configure Gripper (ID 15)
    configureGripper();

    printControlInstructions();

    // 2. Control Loop Variables
    // NO fixed dt - use real elapsed time like original (line 456-522 of CNL_L6_act_2.cpp)
    auto loop_start = std::chrono::high_resolution_clock::now();
    
    // State vectors
    Eigen::Vector4d q = Eigen::Vector4d::Zero();    // Joint positions
    Eigen::Vector4d dq = Eigen::Vector4d::Zero();   // Joint velocities
    Eigen::Vector4d y = Eigen::Vector4d::Zero();    // Task space position
    Eigen::Vector4d dy = Eigen::Vector4d::Zero();   // Task space velocity
    
    // Desired state vectors
    Eigen::Vector4d yd = Eigen::Vector4d::Zero();
    Eigen::Vector4d dyd = Eigen::Vector4d::Zero();
    Eigen::Vector4d ddyd = Eigen::Vector4d::Zero();
    Eigen::Vector4d yd_prev = Eigen::Vector4d::Zero();
    
    // Motor readings
    int16_t dxl_cur[4] = {0, 0, 0, 0};
    int32_t dxl_pos[4] = {0, 0, 0, 0};
    int32_t dxl_vel[4] = {0, 0, 0, 0};
    
    // Control outputs
    int16_t curr_cmd[4] = {0, 0, 0, 0};
    
    // Button state
    int prev_buttons = 0;
    auto button_press_start = std::chrono::steady_clock::now();
    
    // Teleop State
    Eigen::Vector4d h_base = Eigen::Vector4d::Zero();
    Eigen::Vector4d yd_home = Eigen::Vector4d::Zero();
    Eigen::Vector4d h_cur = Eigen::Vector4d::Zero();
    bool home_captured = false;
    
    // Trajectory filter states (declared outside to allow reset on arming)
    Eigen::Vector4d yd_filtered = Eigen::Vector4d::Zero();
    Eigen::Vector4d yd_prev_internal = Eigen::Vector4d::Zero();
    Eigen::Vector4d yd_prev_for_vel = Eigen::Vector4d::Zero();
    Eigen::Vector4d dyd_prev = Eigen::Vector4d::Zero();
    Eigen::Vector4d dyd_for_accel = Eigen::Vector4d::Zero();
    Eigen::Vector4d ddyd_prev = Eigen::Vector4d::Zero();
    
    // Initial position capture
    readMotorStates(dxl_pos, dxl_vel, dxl_cur);
    convertToJointSpace(dxl_pos, dxl_vel, q, dq);
    y = forwardKinematics(q);
    yd = y; // Set initial desired to current
    yd_prev = y;
    
    printf("Starting control loop. Press Ctrl+C to stop.\n");

    // 3. Main Loop (NO SLEEP - runs at maximum speed like original line 471)
    while (!shutdown_requested) {
        // --- A. Read Sensors ---
        readMotorStates(dxl_pos, dxl_vel, dxl_cur);
        convertToJointSpace(dxl_pos, dxl_vel, q, dq);
        
        // Calculate elapsed time from start (EXACT from original line 520-522)
        auto end_time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end_time - loop_start;
        double t = elapsed.count();
        
        // Update task space state using EXACT formulas from original
        y = forwardKinematics(q);
        
        double q1 = q(0), q2 = q(1), q3 = q(2), q4 = q(3);
        double dq1 = dq(0), dq2 = dq(1), dq3 = dq(2), dq4 = dq(3);
        
        // dy calculated with exact formulas from original code
        dy[0] = dq4*cos(q2+q3+q4+3.141592653589793/2.0)*cos(q1)*(6.3E+1/5.0E+2)+dq2*cos(q1)*(cos(q2+3.7E+1/2.0E+2)*(1.3E+1/1.0E+2)+cos(q2+q3+3.141592653589793/2.0)*(3.1E+1/2.5E+2)+cos(q2+q3+q4+3.141592653589793/2.0)*(6.3E+1/5.0E+2))-dq1*sin(q1)*(sin(q2+3.7E+1/2.0E+2)*(1.3E+1/1.0E+2)+sin(q2+q3+3.141592653589793/2.0)*(3.1E+1/2.5E+2)+sin(q2+q3+q4+3.141592653589793/2.0)*(6.3E+1/5.0E+2))+dq3*cos(q1)*(cos(q2+q3+3.141592653589793/2.0)*(3.1E+1/2.5E+2)+cos(q2+q3+q4+3.141592653589793/2.0)*(6.3E+1/5.0E+2));
        dy[1] = dq4*cos(q2+q3+q4+3.141592653589793/2.0)*sin(q1)*(6.3E+1/5.0E+2)+dq2*sin(q1)*(cos(q2+3.7E+1/2.0E+2)*(1.3E+1/1.0E+2)+cos(q2+q3+3.141592653589793/2.0)*(3.1E+1/2.5E+2)+cos(q2+q3+q4+3.141592653589793/2.0)*(6.3E+1/5.0E+2))+dq1*cos(q1)*(sin(q2+3.7E+1/2.0E+2)*(1.3E+1/1.0E+2)+sin(q2+q3+3.141592653589793/2.0)*(3.1E+1/2.5E+2)+sin(q2+q3+q4+3.141592653589793/2.0)*(6.3E+1/5.0E+2))+dq3*sin(q1)*(cos(q2+q3+3.141592653589793/2.0)*(3.1E+1/2.5E+2)+cos(q2+q3+q4+3.141592653589793/2.0)*(6.3E+1/5.0E+2));
        dy[2] = -dq3*(sin(q2+q3+3.141592653589793/2.0)*(3.1E+1/2.5E+2)+sin(q2+q3+q4+3.141592653589793/2.0)*(6.3E+1/5.0E+2))-dq4*sin(q2+q3+q4+3.141592653589793/2.0)*(6.3E+1/5.0E+2)-dq2*(sin(q2+3.7E+1/2.0E+2)*(1.3E+1/1.0E+2)+sin(q2+q3+3.141592653589793/2.0)*(3.1E+1/2.5E+2)+sin(q2+q3+q4+3.141592653589793/2.0)*(6.3E+1/5.0E+2));
        dy[3] = dq2+dq3+dq4;
        
        // --- B. User Input & Trajectory ---
        
        // Get Haptic Buttons
        int buttons = getHapticButtons();
        
        // Update haptic state
        if (g_hSampleReady.exchange(false)) {
            h_cur(0) = g_hSample.position[0];
            h_cur(1) = g_hSample.position[1];
            h_cur(2) = g_hSample.position[2];
            // Roll from transform (atan2(T[6], T[10]))
            h_cur(3) = atan2(g_hSample.transform[6], g_hSample.transform[10]);
        }

        // Button 1 Logic (Teleop Control)
        const int BTN1_MASK = 1;
        if ((buttons & BTN1_MASK) && !(prev_buttons & BTN1_MASK)) {
            if (!teleop_active) {
                // Enable Teleop (EXACT from original lines 676-695)
                yd_home = y; // Capture current robot position
                h_base = h_cur; // Capture current haptic position
                teleop_active = true;
                home_captured = true;
                
                // CRITICAL: Reset all filter states to current position (prevents initial jump)
                yd_filtered = yd_home;
                yd_prev_internal = yd_home;
                yd_prev_for_vel = yd_home;
                dyd_prev.setZero();
                dyd_for_accel.setZero();
                ddyd_prev.setZero();
                yd_prev = yd_home;
                
                printf("Teleop ENABLED. Home captured.\n");
                std::cout << "Home: " << yd_home.transpose() << std::endl;
                std::cout << "[ARMING] Initial error should be zero: err=[" 
                          << (y(0)-yd_home(0)) << ", " << (y(1)-yd_home(1)) << ", " 
                          << (y(2)-yd_home(2)) << ", " << (y(3)-yd_home(3)) << "]\n";
            } else {
                // Disable Teleop / Shutdown
                printf("Teleop DISABLED (Shutdown requested).\n");
                shutdown_requested = 1;
            }
        }
        
        if (teleop_active) {
            // Coordinate system mapping (EXACT from original)
            Eigen::Vector4d delta_haptic = h_cur - h_base;
            
            Eigen::Vector4d delta;
            delta(0) = delta_haptic(2);   // Haptic Z -> Arm X
            delta(1) = delta_haptic(0);   // Haptic X -> Arm Y
            delta(2) = delta_haptic(1);   // Haptic Y -> Arm Z
            delta(3) = delta_haptic(3);   // Haptic Roll -> Arm Yaw
            
            // Scaling (EXACT from original)
            const double x_scale = 0.0011;
            const double y_scale = 0.0011;
            const double z_scale = 0.001;
            const double ang_scale = 1.20;
            Eigen::Vector4d scale;
            scale << x_scale, y_scale, z_scale, ang_scale;
            
            Eigen::Vector4d offset = scale.cwiseProduct(delta);
            
            // Clamp offset (EXACT from original)
            Eigen::Vector4d max_offset;
            max_offset << 0.30, 0.30, 0.15, 0.5;
            for(int i=0; i<4; i++) {
                if(offset(i) > max_offset(i)) offset(i) = max_offset(i);
                if(offset(i) < -max_offset(i)) offset(i) = -max_offset(i);
            }
            
            Eigen::Vector4d yd_target = yd_home + offset;
            
            // Low-pass filter + rate limiter (EXACT from original lines 810-850)
            // Filter tuning (EXACT from original)
            const double alpha = 0.15;
            
            // Exponential moving average filter
            yd_filtered = alpha * yd_target + (1.0 - alpha) * yd_filtered;
            
            // Rate limiter (EXACT from original)
            const double max_rate_pos = 0.003;
            const double max_rate_ang = 0.02;
            Eigen::Vector4d max_rate; 
            max_rate << max_rate_pos, max_rate_pos, max_rate_pos, max_rate_ang;
            
            Eigen::Vector4d yd_change = yd_filtered - yd_prev_internal;
            for(int i=0; i<4; i++) {
                if(yd_change(i) > max_rate(i)) yd_change(i) = max_rate(i);
                if(yd_change(i) < -max_rate(i)) yd_change(i) = -max_rate(i);
            }
            
            yd = yd_prev_internal + yd_change;
            
            // Compute desired velocity (EXACT from original lines 820-830)
            const double dt_control = 0.001; // 1kHz from original
            dyd = (yd - yd_prev_for_vel) / dt_control;
            
            // Velocity smoothing (EXACT from original)
            const double vel_alpha = 0.3;
            dyd = vel_alpha * dyd + (1.0 - vel_alpha) * dyd_prev;
            dyd_prev = dyd;
            
            // Acceleration computation (EXACT from original)
            ddyd = (dyd - dyd_for_accel) / dt_control;
            
            // Acceleration smoothing (EXACT from original)
            const double accel_alpha = 0.5;
            ddyd = accel_alpha * ddyd + (1.0 - accel_alpha) * ddyd_prev;
            ddyd_prev = ddyd;
            
            // Update states for next iteration (EXACT from original)
            yd_prev_for_vel = yd;
            dyd_for_accel = dyd;
            yd_prev_internal = yd;
            
        } else {
            // Hold position (EXACT from original)
            yd = yd_prev;
            dyd.setZero();
            ddyd.setZero();
        }
        
        // --- C. Safety Check ---
        if (!checkSafety(dxl_cur[0], dxl_cur[1], dxl_cur[2], dxl_cur[3], y, yd)) {
            shutdown_requested = 1;
            break;
        }
        
        // --- D. Compute Control ---
        // Calculate Jinv (inverse Jacobian) using exact formulas from original
        double Jinv_arr[4][4];
        Jinv_arr[0][0] = (sin(q1)*-5.0E+2)/(cos(q2+q3+q4)*6.3E+1+sin(q2+3.7E+1/2.0E+2)*6.5E+1+cos(q2+q3)*6.2E+1);
        Jinv_arr[0][1] = (cos(q1)*5.0E+2)/(cos(q2+q3+q4)*6.3E+1+sin(q2+3.7E+1/2.0E+2)*6.5E+1+cos(q2+q3)*6.2E+1);
        Jinv_arr[0][2] = 0.0;
        Jinv_arr[0][3] = 0.0;
        Jinv_arr[1][0] = (cos(q2+q3)*cos(q1)*(1.0E+2/1.3E+1))/cos(q3-3.7E+1/2.0E+2);
        Jinv_arr[1][1] = (cos(q2+q3)*sin(q1)*(1.0E+2/1.3E+1))/cos(q3-3.7E+1/2.0E+2);
        Jinv_arr[1][2] = (sin(q2+q3)*(-1.0E+2/1.3E+1))/cos(q3-3.7E+1/2.0E+2);
        Jinv_arr[1][3] = (sin(q4)*(6.3E+1/6.5E+1))/cos(q3-3.7E+1/2.0E+2);
        Jinv_arr[2][0] = (cos(q1)*(sin(q2+3.7E+1/2.0E+2)*6.5E+1+cos(q2+q3)*6.2E+1)*(-5.0E+1/4.03E+2))/cos(q3-3.7E+1/2.0E+2);
        Jinv_arr[2][1] = (sin(q1)*(sin(q2+3.7E+1/2.0E+2)*6.5E+1+cos(q2+q3)*6.2E+1)*(-5.0E+1/4.03E+2))/cos(q3-3.7E+1/2.0E+2);
        Jinv_arr[2][2] = ((cos(q2+3.7E+1/2.0E+2)*6.5E+1-sin(q2+q3)*6.2E+1)*(-5.0E+1/4.03E+2))/cos(q3-3.7E+1/2.0E+2);
        Jinv_arr[2][3] = ((cos(q3+q4-3.7E+1/2.0E+2)*6.5E+1+sin(q4)*6.2E+1)*(-1.563275434243176E-2))/cos(q3-3.7E+1/2.0E+2);
        Jinv_arr[3][0] = (sin(q2+3.7E+1/2.0E+2)*cos(q1)*(2.5E+2/3.1E+1))/cos(q3-3.7E+1/2.0E+2);
        Jinv_arr[3][1] = (sin(q2+3.7E+1/2.0E+2)*sin(q1)*(2.5E+2/3.1E+1))/cos(q3-3.7E+1/2.0E+2);
        Jinv_arr[3][2] = (cos(q2+3.7E+1/2.0E+2)*(2.5E+2/3.1E+1))/cos(q3-3.7E+1/2.0E+2);
        Jinv_arr[3][3] = (cos(q3-3.7E+1/2.0E+2)*6.2E+1+cos(q3+q4-3.7E+1/2.0E+2)*6.3E+1)/(cos(q3-3.7E+1/2.0E+2)*6.2E+1);
        Eigen::Matrix4d Jinv = Eigen::Map<Eigen::Matrix<double, 4, 4, Eigen::RowMajor>>(&Jinv_arr[0][0]);
        
        // Calculate dJ (Jacobian derivative) using exact formulas from original
        double dJ_arr[4][4];
        dJ_arr[0][0] = dq4*cos(q2+q3+q4+3.141592653589793/2.0)*sin(q1)*(-6.3E+1/5.0E+2)-dq2*sin(q1)*(cos(q2+3.7E+1/2.0E+2)*(1.3E+1/1.0E+2)+cos(q2+q3+3.141592653589793/2.0)*(3.1E+1/2.5E+2)+cos(q2+q3+q4+3.141592653589793/2.0)*(6.3E+1/5.0E+2))-dq1*cos(q1)*(sin(q2+3.7E+1/2.0E+2)*(1.3E+1/1.0E+2)+sin(q2+q3+3.141592653589793/2.0)*(3.1E+1/2.5E+2)+sin(q2+q3+q4+3.141592653589793/2.0)*(6.3E+1/5.0E+2))-dq3*sin(q1)*(cos(q2+q3+3.141592653589793/2.0)*(3.1E+1/2.5E+2)+cos(q2+q3+q4+3.141592653589793/2.0)*(6.3E+1/5.0E+2));
        dJ_arr[0][1] = dq4*cos(q1)*sin(q2+q3+q4+3.141592653589793/2.0)*(-6.3E+1/5.0E+2)-dq1*sin(q1)*(cos(q2+3.7E+1/2.0E+2)*(1.3E+1/1.0E+2)+cos(q2+q3+3.141592653589793/2.0)*(3.1E+1/2.5E+2)+cos(q2+q3+q4+3.141592653589793/2.0)*(6.3E+1/5.0E+2))-dq2*cos(q1)*(sin(q2+3.7E+1/2.0E+2)*(1.3E+1/1.0E+2)+sin(q2+q3+3.141592653589793/2.0)*(3.1E+1/2.5E+2)+sin(q2+q3+q4+3.141592653589793/2.0)*(6.3E+1/5.0E+2))-dq3*cos(q1)*(sin(q2+q3+3.141592653589793/2.0)*(3.1E+1/2.5E+2)+sin(q2+q3+q4+3.141592653589793/2.0)*(6.3E+1/5.0E+2));
        dJ_arr[0][2] = dq4*cos(q1)*sin(q2+q3+q4+3.141592653589793/2.0)*(-6.3E+1/5.0E+2)-dq1*sin(q1)*(cos(q2+q3+3.141592653589793/2.0)*(3.1E+1/2.5E+2)+cos(q2+q3+q4+3.141592653589793/2.0)*(6.3E+1/5.0E+2))-dq2*cos(q1)*(sin(q2+q3+3.141592653589793/2.0)*(3.1E+1/2.5E+2)+sin(q2+q3+q4+3.141592653589793/2.0)*(6.3E+1/5.0E+2))-dq3*cos(q1)*(sin(q2+q3+3.141592653589793/2.0)*(3.1E+1/2.5E+2)+sin(q2+q3+q4+3.141592653589793/2.0)*(6.3E+1/5.0E+2));
        dJ_arr[0][3] = dq1*cos(q2+q3+q4+3.141592653589793/2.0)*sin(q1)*(-6.3E+1/5.0E+2)-dq2*cos(q1)*sin(q2+q3+q4+3.141592653589793/2.0)*(6.3E+1/5.0E+2)-dq3*cos(q1)*sin(q2+q3+q4+3.141592653589793/2.0)*(6.3E+1/5.0E+2)-dq4*cos(q1)*sin(q2+q3+q4+3.141592653589793/2.0)*(6.3E+1/5.0E+2);
        dJ_arr[1][0] = dq4*cos(q2+q3+q4+3.141592653589793/2.0)*cos(q1)*(6.3E+1/5.0E+2)+dq2*cos(q1)*(cos(q2+3.7E+1/2.0E+2)*(1.3E+1/1.0E+2)+cos(q2+q3+3.141592653589793/2.0)*(3.1E+1/2.5E+2)+cos(q2+q3+q4+3.141592653589793/2.0)*(6.3E+1/5.0E+2))-dq1*sin(q1)*(sin(q2+3.7E+1/2.0E+2)*(1.3E+1/1.0E+2)+sin(q2+q3+3.141592653589793/2.0)*(3.1E+1/2.5E+2)+sin(q2+q3+q4+3.141592653589793/2.0)*(6.3E+1/5.0E+2))+dq3*cos(q1)*(cos(q2+q3+3.141592653589793/2.0)*(3.1E+1/2.5E+2)+cos(q2+q3+q4+3.141592653589793/2.0)*(6.3E+1/5.0E+2));
        dJ_arr[1][1] = -dq3*sin(q1)*(sin(q2+q3+3.141592653589793/2.0)*(3.1E+1/2.5E+2)+sin(q2+q3+q4+3.141592653589793/2.0)*(6.3E+1/5.0E+2))+dq1*cos(q1)*(cos(q2+3.7E+1/2.0E+2)*(1.3E+1/1.0E+2)+cos(q2+q3+3.141592653589793/2.0)*(3.1E+1/2.5E+2)+cos(q2+q3+q4+3.141592653589793/2.0)*(6.3E+1/5.0E+2))-dq4*sin(q2+q3+q4+3.141592653589793/2.0)*sin(q1)*(6.3E+1/5.0E+2)-dq2*sin(q1)*(sin(q2+3.7E+1/2.0E+2)*(1.3E+1/1.0E+2)+sin(q2+q3+3.141592653589793/2.0)*(3.1E+1/2.5E+2)+sin(q2+q3+q4+3.141592653589793/2.0)*(6.3E+1/5.0E+2));
        dJ_arr[1][2] = -dq2*sin(q1)*(sin(q2+q3+3.141592653589793/2.0)*(3.1E+1/2.5E+2)+sin(q2+q3+q4+3.141592653589793/2.0)*(6.3E+1/5.0E+2))-dq3*sin(q1)*(sin(q2+q3+3.141592653589793/2.0)*(3.1E+1/2.5E+2)+sin(q2+q3+q4+3.141592653589793/2.0)*(6.3E+1/5.0E+2))-dq4*sin(q2+q3+q4+3.141592653589793/2.0)*sin(q1)*(6.3E+1/5.0E+2)+dq1*cos(q1)*(cos(q2+q3+3.141592653589793/2.0)*(3.1E+1/2.5E+2)+cos(q2+q3+q4+3.141592653589793/2.0)*(6.3E+1/5.0E+2));
        dJ_arr[1][3] = dq1*cos(q2+q3+q4+3.141592653589793/2.0)*cos(q1)*(6.3E+1/5.0E+2)-dq2*sin(q2+q3+q4+3.141592653589793/2.0)*sin(q1)*(6.3E+1/5.0E+2)-dq3*sin(q2+q3+q4+3.141592653589793/2.0)*sin(q1)*(6.3E+1/5.0E+2)-dq4*sin(q2+q3+q4+3.141592653589793/2.0)*sin(q1)*(6.3E+1/5.0E+2);
        dJ_arr[2][0] = 0.0;
        dJ_arr[2][1] = dq4*cos(q2+q3+q4+3.141592653589793/2.0)*(-6.3E+1/5.0E+2)-dq2*(cos(q2+3.7E+1/2.0E+2)*(1.3E+1/1.0E+2)+cos(q2+q3+3.141592653589793/2.0)*(3.1E+1/2.5E+2)+cos(q2+q3+q4+3.141592653589793/2.0)*(6.3E+1/5.0E+2))-dq3*(cos(q2+q3+3.141592653589793/2.0)*(3.1E+1/2.5E+2)+cos(q2+q3+q4+3.141592653589793/2.0)*(6.3E+1/5.0E+2));
        dJ_arr[2][2] = dq4*cos(q2+q3+q4+3.141592653589793/2.0)*(-6.3E+1/5.0E+2)-dq2*(cos(q2+q3+3.141592653589793/2.0)*(3.1E+1/2.5E+2)+cos(q2+q3+q4+3.141592653589793/2.0)*(6.3E+1/5.0E+2))-dq3*(cos(q2+q3+3.141592653589793/2.0)*(3.1E+1/2.5E+2)+cos(q2+q3+q4+3.141592653589793/2.0)*(6.3E+1/5.0E+2));
        dJ_arr[2][3] = dq2*cos(q2+q3+q4+3.141592653589793/2.0)*(-6.3E+1/5.0E+2)-dq3*cos(q2+q3+q4+3.141592653589793/2.0)*(6.3E+1/5.0E+2)-dq4*cos(q2+q3+q4+3.141592653589793/2.0)*(6.3E+1/5.0E+2);
        dJ_arr[3][0] = 0.0;
        dJ_arr[3][1] = 0.0;
        dJ_arr[3][2] = 0.0;
        dJ_arr[3][3] = 0.0;
        Eigen::Matrix4d dJ = Eigen::Map<Eigen::Matrix<double, 4, 4, Eigen::RowMajor>>(&dJ_arr[0][0]);
        Eigen::Matrix4d J = Jinv.inverse();  // Not used directly but for reference
        
        computeControl(q, dq, y, dy, yd, dyd, ddyd, J, Jinv, dJ,
                       sf1, sf2, sf3, sf4,  // Use command line parameters
                       bf, DZ,               // Use command line parameters
                       curr_cmd[0], curr_cmd[1], curr_cmd[2], curr_cmd[3]);
        
        // --- E. Actuate ---
        sendCurrentCommands(curr_cmd[0], curr_cmd[1], curr_cmd[2], curr_cmd[3]);
        
        // Handle Gripper
        handleGripperControl(buttons, prev_buttons, button_press_start);
        // prev_buttons is updated by handleGripperControl
        
        // --- F. Display & Timing ---
        displayStatus(y, yd, (int)(t*1000)); // Display iteration based on time
        
        yd_prev = yd;
        // NO SLEEP - loop runs at maximum speed (original line 473 has usleep commented out)
    }

    // 4. Shutdown
    printf("\nShutting down...\n");
    disableAllMotors();
    shutdownHapticDevice();
    
    return 0;
}
