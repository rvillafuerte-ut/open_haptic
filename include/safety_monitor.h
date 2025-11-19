// ==============================================================================
// SAFETY MONITOR HEADER
// Functions for safety checks and emergency stops
// ==============================================================================

#ifndef SAFETY_MONITOR_H
#define SAFETY_MONITOR_H

#include <Eigen/Dense>
#include <cstdint>

// ==============================================================================
// FUNCTION DECLARATIONS
// ==============================================================================

/**
 * Checks if the system is operating within safe limits.
 * Monitors motor currents and position errors.
 * Prints warning if unsafe.
 * 
 * @param dxl1_cur Current of motor 1
 * @param dxl2_cur Current of motor 2
 * @param dxl3_cur Current of motor 3
 * @param dxl4_cur Current of motor 4
 * @param y Current position
 * @param yd Desired position
 * @return true if safe, false if unsafe (should trigger stop)
 */
bool checkSafety(int16_t dxl1_cur, int16_t dxl2_cur, int16_t dxl3_cur, int16_t dxl4_cur,
                 const Eigen::Vector4d& y, const Eigen::Vector4d& yd);

/**
 * Checks if the robot is within defined workspace boundaries.
 * @param y Current position
 * @return true if inside workspace
 */
bool checkWorkspaceLimits(const Eigen::Vector4d& y);

/**
 * Triggers an emergency stop action.
 * Prints reason and can perform other safety actions.
 * @param reason String explaining the stop cause
 */
void emergencyStop(const char* reason);

#endif // SAFETY_MONITOR_H
