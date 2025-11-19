// ==============================================================================
// TRAJECTORY GENERATOR HEADER
// Functions for generating desired trajectories
// ==============================================================================

#ifndef TRAJECTORY_GENERATOR_H
#define TRAJECTORY_GENERATOR_H

#include <Eigen/Dense>
#include <vector>

// ==============================================================================
// GLOBAL STATE
// ==============================================================================

extern Eigen::Vector4d yd_filtered_state;
extern Eigen::Vector4d yd_prev_state;

// ==============================================================================
// FUNCTION DECLARATIONS
// ==============================================================================

/**
 * Generates trajectory from Haptic Device input.
 * Maps haptic position to robot workspace with scaling.
 * @param t Current time
 * @param vf Velocity scaling factor
 * @return Desired position [x, y, z, yaw]
 */
Eigen::Vector4d getTrajectoryFromHaptic(double t, float vf);

/**
 * Generates a circular test trajectory.
 * @param t Current time
 * @param radius Radius of circle (m)
 * @param freq Frequency (Hz)
 * @return Desired position
 */
Eigen::Vector4d getTrajectoryCircular(double t, float radius, float freq);

/**
 * Generates trajectory from Keyboard input (WASD).
 * @param t Current time
 * @return Desired position
 */
Eigen::Vector4d getTrajectoryFromKeyboard(double t);

/**
 * Generates trajectory from a pre-recorded file.
 * @param t Current time
 * @param trajectory_data Vector of points
 * @param dt Time step between points
 * @return Desired position
 */
Eigen::Vector4d getTrajectoryFromFile(double t, 
                                      const std::vector<Eigen::VectorXd>& trajectory_data,
                                      double dt);

/**
 * Applies a low-pass filter to the trajectory.
 * Smooths out jitter from haptic device.
 * @param yd_raw Raw target position
 * @param bf Smoothing factor (0.0-1.0, lower is smoother)
 * @return Filtered position
 */
Eigen::Vector4d filterTrajectory(const Eigen::Vector4d& yd_raw, float bf);

/**
 * Resets the filter state.
 * Call this when enabling teleop to avoid jumps.
 */
void resetTrajectoryFilter();

/**
 * Clamps the trajectory to safe workspace limits.
 * Modifies the vector in-place.
 * @param yd Desired position to check/clamp
 */
void applyWorkspaceLimits(Eigen::Vector4d& yd);

#endif // TRAJECTORY_GENERATOR_H
