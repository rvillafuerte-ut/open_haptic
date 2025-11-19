// ==============================================================================
// DYNAMIXEL CONTROLLER HEADER
// Functions for motor initialization, configuration, and control
// ==============================================================================

#ifndef DYNAMIXEL_CONTROLLER_H
#define DYNAMIXEL_CONTROLLER_H

#include "dynamixel_sdk.h"
#include <Eigen/Dense>
#include <cstdint>

// ==============================================================================
// CONSTANTS
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

// Control Constants
#define TORQUE_ENABLE               1
#define TORQUE_DISABLE              0
#define CURR_PEAK                   330
#define CURR_MAX                    290

// ==============================================================================
// FUNCTION DECLARATIONS
// ==============================================================================

/**
 * Initializes the Dynamixel SDK port and packet handlers.
 * Opens the USB port and sets the baudrate to 4.5Mbps.
 * Returns true if successful.
 */
bool initializeDynamixel();

/**
 * Configures a specific motor (ID) for Current Control Mode.
 * Sets return delay time, current limits, and enables torque.
 * @param id Motor ID (11-14)
 */
void configureMotor(uint8_t id);

/**
 * Reads the current state (Position, Velocity, Current) of all 4 motors.
 * Uses GroupSyncRead for efficient communication.
 * @param pos Array to store positions (ticks)
 * @param vel Array to store velocities (ticks/s)
 * @param cur Array to store currents (mA)
 */
void readMotorStates(int32_t* pos, int32_t* vel, int16_t* cur);

/**
 * Converts raw motor data to joint space (radians).
 * Handles unit conversion and coordinate frame adjustments.
 * @param pos_ticks Raw position ticks
 * @param vel_ticks Raw velocity ticks
 * @param q Output joint positions (rad)
 * @param dq Output joint velocities (rad/s)
 */
void convertToJointSpace(const int32_t* pos_ticks, const int32_t* vel_ticks, 
                         Eigen::Vector4d& q, Eigen::Vector4d& dq);

/**
 * Sends current (torque) commands to all 4 motors.
 * Uses GroupSyncWrite for simultaneous execution.
 * @param c1 Current for motor 1 (mA)
 * @param c2 Current for motor 2 (mA)
 * @param c3 Current for motor 3 (mA)
 * @param c4 Current for motor 4 (mA)
 */
void sendCurrentCommands(int16_t c1, int16_t c2, int16_t c3, int16_t c4);

/**
 * Disables torque on all motors and closes the port.
 * Call this before exiting the program.
 */
void disableAllMotors();

/**
 * Returns the internal PortHandler instance.
 */
dynamixel::PortHandler* getPortHandler();

/**
 * Returns the internal PacketHandler instance.
 */
dynamixel::PacketHandler* getPacketHandler();

#endif // DYNAMIXEL_CONTROLLER_H
