// ==============================================================================
// TRAJECTORY GENERATOR IMPLEMENTATION
// ==============================================================================

#include "trajectory_generator.h"
#include "haptic_controller.h"
#include "utilities.h"
#include <cmath>
#include <fstream>
#include <sstream>

// Global filter state definitions
Eigen::Vector4d yd_filtered_state = Eigen::Vector4d::Zero();
Eigen::Vector4d yd_prev_state = Eigen::Vector4d::Zero();

Eigen::Vector4d getTrajectoryFromHaptic(double t, float vf) {
    Eigen::Vector4d yd = Eigen::Vector4d::Zero();
    
    if (!isHapticDataReady()) {
        return yd;
    }
    
    // Position scaling (mm to meters)
    // vf acts as a sensitivity factor (default ~1.0)
    double pos_scale_x = 0.001 * vf;
    double pos_scale_y = 0.001 * vf;
    double pos_scale_z = 0.001 * vf;
    
    // Workspace center offset
    double offset_x = 0.18;
    double offset_y = 0.0;
    double offset_z = 0.08;
    
    double hpos[3];
    getHapticPosition(hpos);
    
    yd(0) = hpos[0] * pos_scale_x + offset_x;
    yd(1) = hpos[1] * pos_scale_y + offset_y;
    yd(2) = hpos[2] * pos_scale_z + offset_z;
    
    // Orientation from transform
    double transform[16];
    getHapticTransform(transform);
    double yaw = atan2(transform[4], transform[0]);
    yd(3) = yaw;
    
    return yd;
}

Eigen::Vector4d getTrajectoryCircular(double t, float radius, float freq) {
    Eigen::Vector4d yd;
    
    // Circle center
    double center_x = 0.18;
    double center_y = 0.0;
    double center_z = 0.10;
    
    // Angular frequency
    double omega = 2.0 * M_PI * freq;
    
    // Circular position
    yd(0) = center_x + radius * cos(omega * t);
    yd(1) = center_y + radius * sin(omega * t);
    yd(2) = center_z;
    
    // Tangent orientation (or 0.0 for fixed)
    yd(3) = 0.0;
    
    return yd;
}

Eigen::Vector4d getTrajectoryFromKeyboard(double t) {
    static Eigen::Vector4d yd_current(0.18, 0.0, 0.10, 0.0);
    static double vel_linear = 0.001;   // 1 mm/step
    static double vel_angular = 0.01;    // ~0.57 deg/step
    
    if (kbhit()) {
        int ch = getch();
        switch (ch) {
            case 'w': case 'W': yd_current(0) += vel_linear; break;
            case 's': case 'S': yd_current(0) -= vel_linear; break;
            case 'a': case 'A': yd_current(1) -= vel_linear; break;
            case 'd': case 'D': yd_current(1) += vel_linear; break;
            case 'q': case 'Q': yd_current(2) += vel_linear; break;
            case 'e': case 'E': yd_current(2) -= vel_linear; break;
            case 'z': case 'Z': yd_current(3) -= vel_angular; break;
            case 'c': case 'C': yd_current(3) += vel_angular; break;
            case 'r': case 'R':
                yd_current << 0.18, 0.0, 0.10, 0.0;
                printf("\n[Reset] Returned to home position\n");
                break;
        }
        
        // Apply workspace limits
        applyWorkspaceLimits(yd_current);
    }
    
    return yd_current;
}

Eigen::Vector4d getTrajectoryFromFile(double t, 
                                      const std::vector<Eigen::VectorXd>& trajectory_data,
                                      double dt) {
    Eigen::Vector4d yd = Eigen::Vector4d::Zero();
    
    if (trajectory_data.empty()) {
        return yd;
    }
    
    // Calculate index based on time
    int idx = (int)(t / dt);
    
    // Bounds checking
    if (idx < 0) idx = 0;
    if (idx >= trajectory_data.size()) idx = trajectory_data.size() - 1;
    
    // Linear interpolation between points
    if (idx < trajectory_data.size() - 1) {
        double alpha = (t - idx * dt) / dt;
        yd = (1.0 - alpha) * trajectory_data[idx].head<4>() 
           + alpha * trajectory_data[idx + 1].head<4>();
    } else {
        yd = trajectory_data[idx].head<4>();
    }
    
    return yd;
}

Eigen::Vector4d filterTrajectory(const Eigen::Vector4d& yd_raw, float bf) {
    // First-order low-pass filter
    float alpha = bf;
    yd_filtered_state = alpha * yd_raw + (1.0f - alpha) * yd_filtered_state;
    return yd_filtered_state;
}

void resetTrajectoryFilter() {
    yd_filtered_state = Eigen::Vector4d::Zero();
    yd_prev_state = Eigen::Vector4d::Zero();
}

void applyWorkspaceLimits(Eigen::Vector4d& yd) {
    // X limits: 0.10 to 0.25 m
    if (yd(0) < 0.10) yd(0) = 0.10;
    if (yd(0) > 0.25) yd(0) = 0.25;
    
    // Y limits: -0.15 to +0.15 m
    if (yd(1) < -0.15) yd(1) = -0.15;
    if (yd(1) > 0.15) yd(1) = 0.15;
    
    // Z limits: 0.05 to 0.20 m
    if (yd(2) < 0.05) yd(2) = 0.05;
    if (yd(2) > 0.20) yd(2) = 0.20;
    
    // Theta limits: -π to +π
    while (yd(3) > M_PI) yd(3) -= 2.0 * M_PI;
    while (yd(3) < -M_PI) yd(3) += 2.0 * M_PI;
}
