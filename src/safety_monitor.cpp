// ==============================================================================
// SAFETY MONITOR IMPLEMENTATION
// ==============================================================================

#include "safety_monitor.h"
#include "dynamixel_controller.h"
#include <cstdio>
#include <cmath>

bool checkSafety(int16_t dxl1_cur, int16_t dxl2_cur, int16_t dxl3_cur, int16_t dxl4_cur,
                 const Eigen::Vector4d& y, const Eigen::Vector4d& yd) {
    
    // Check current limits
    if (abs(dxl1_cur) > CURR_PEAK || abs(dxl2_cur) > CURR_PEAK ||
        abs(dxl3_cur) > CURR_PEAK || abs(dxl4_cur) > CURR_PEAK) {
        
        printf("\n [UNSAFE] Current limit exceeded!\n");
        printf("   Currents: [%d, %d, %d, %d]\n", dxl1_cur, dxl2_cur, dxl3_cur, dxl4_cur);
        printf("   Position: [%.4f, %.4f, %.4f, %.4f]\n", y(0), y(1), y(2), y(3));
        printf("   Desired:  [%.4f, %.4f, %.4f, %.4f]\n", yd(0), yd(1), yd(2), yd(3));
        printf("   Error:    [%.4f, %.4f, %.4f, %.4f]\n",
               y(0)-yd(0), y(1)-yd(1), y(2)-yd(2), y(3)-yd(3));
        
        return false;
    }
    
    return true;
}

bool checkWorkspaceLimits(const Eigen::Vector4d& y) {
    // Check if within safe workspace
    if (y(0) < 0.08 || y(0) > 0.30 ||
        y(1) < -0.20 || y(1) > 0.20 ||
        y(2) < 0.03 || y(2) > 0.25) {
        
        printf("\n⚠ [WARNING] Position outside workspace limits\n");
        printf("   Position: [%.4f, %.4f, %.4f]\n", y(0), y(1), y(2));
        return false;
    }
    
    return true;
}

void emergencyStop(const char* reason) {
    printf("\n[EMERGENCY STOP] %s\n", reason);
    // Additional emergency handling can be added here
    // (e.g., save state to file, send alert, etc.)
}
