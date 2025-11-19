// ==============================================================================
// GRIPPER CONTROLLER HEADER
// Functions for gripper control (ID 15)
// ==============================================================================

#ifndef GRIPPER_CONTROLLER_H
#define GRIPPER_CONTROLLER_H

#include "dynamixel_sdk.h"
#include <chrono>

// ==============================================================================
// CONSTANTS
// ==============================================================================

#define GRIPPER_ID                  15
#define GRIPPER_CURR_LIMIT          260
#define GRIPPER_CURR_OPEN           240
#define GRIPPER_CURR_CLOSE          180
#define GRIPPER_OPEN_TICKS          1400
#define GRIPPER_OPEN_DIR            (-1)

// ==============================================================================
// GLOBAL STATE
// ==============================================================================

extern bool gripper_calibrated;
extern int32_t gripper_closed_pos_ticks;
extern int32_t gripper_open_pos_ticks;
extern int gripper_open_dir_state;

// ==============================================================================
// FUNCTION DECLARATIONS
// ==============================================================================

/**
 * Configures the gripper motor (ID 15).
 * Sets Current-based Position Control Mode (Mode 5).
 * Calibrates the closed position based on current location.
 */
void configureGripper();

/**
 * Handles gripper logic based on button inputs.
 * Short press (Button 2): Toggle Open/Close.
 * Long press (>0.8s): Flip opening direction.
 * @param buttons Current button bitmask
 * @param prev_buttons Previous button bitmask
 * @param button_press_start Timestamp of button press start
 */
void handleGripperControl(int buttons, 
                          int& prev_buttons, 
                          std::chrono::steady_clock::time_point& button_press_start);

/**
 * Commands the gripper to open.
 * Sets goal position and current limit.
 */
void openGripper();

/**
 * Commands the gripper to close.
 * Sets goal position and current limit.
 */
void closeGripper();

/**
 * Returns true if the gripper is currently in the open state.
 */
bool isGripperOpen();

#endif // GRIPPER_CONTROLLER_H
