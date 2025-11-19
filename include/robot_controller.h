// ==============================================================================
// ROBOT CONTROLLER HEADER
// Kinematics, Dynamics, and Control Laws
// ==============================================================================

#ifndef ROBOT_CONTROLLER_H
#define ROBOT_CONTROLLER_H

#include <Eigen/Dense>
#include <cstdint>

// ==============================================================================
// FUNCTION DECLARATIONS
// ==============================================================================

/**
 * Computes Forward Kinematics.
 * Calculates task space position (x,y,z,yaw) from joint angles.
 * @param q Joint positions (rad)
 * @return Task space vector [x, y, z, yaw]
 */
Eigen::Vector4d forwardKinematics(const Eigen::Vector4d& q);

/**
 * Computes the Analytical Jacobian matrix.
 * Relates joint velocities to task space velocities.
 * @param q Joint positions (rad)
 * @return 4x4 Jacobian matrix
 */
Eigen::Matrix4d computeJacobian(const Eigen::Vector4d& q);

/**
 * Main Control Law (Computed Torque Control).
 * Calculates motor currents to track desired trajectory.
 * 
 * @param q Current joint positions
 * @param dq Current joint velocities
 * @param y Current task position
 * @param dy Current task velocity
 * @param yd Desired task position
 * @param dyd Desired task velocity
 * @param ddyd Desired task acceleration
 * @param J Jacobian matrix
 * @param Jinv Inverse Jacobian
 * @param dJ Jacobian derivative (optional/approx)
 * @param sf1-sf4 Scaling factors for each joint
 * @param bf Back-EMF compensation factor
 * @param DZ Deadzone compensation factor
 * @param curr_d1-d4 Output currents for motors (mA)
 */
void computeControl(const Eigen::Vector4d& q,
                    const Eigen::Vector4d& dq,
                    const Eigen::Vector4d& y,
                    const Eigen::Vector4d& dy,
                    const Eigen::Vector4d& yd,
                    const Eigen::Vector4d& dyd,
                    const Eigen::Vector4d& ddyd,
                    const Eigen::Matrix4d& J,
                    const Eigen::Matrix4d& Jinv,
                    const Eigen::Matrix4d& dJ,
                    float sf1, float sf2, float sf3, float sf4,
                    float bf, float DZ,
                    int16_t& curr_d1,
                    int16_t& curr_d2,
                    int16_t& curr_d3,
                    int16_t& curr_d4);

/**
 * Auto-generated Dynamics Function (Lagrangian).
 * Computes Inertia Matrix (M) and Coriolis/Gravity vector (phib).
 * DO NOT MODIFY MANUALLY.
 */
void OMDyn(const double* q, const double* dq, double* M, double* phib);

#endif // ROBOT_CONTROLLER_H
