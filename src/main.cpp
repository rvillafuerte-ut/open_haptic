#include <iostream>
#include <cstring>

// Eigen
#include <Eigen/Dense>

// OpenHaptics HD (device) API
#include <HD/hd.h>

int main(int argc, char** argv) {
    // Simple Eigen usage (no output)
    Eigen::Vector3d a(1.0, 2.0, 3.0);
    Eigen::Vector3d b(4.0, 5.0, 6.0);
    auto c = a.cross(b);
    (void)c;

    // Initialize OpenHaptics device (minimal messages)
    HDErrorInfo error;
    HHD hHD = hdInitDevice(HD_DEFAULT_DEVICE);
    if (HD_DEVICE_ERROR(error = hdGetError())) {
        return 0; // sin dispositivo: salir OK
    }

    hdMakeCurrentDevice(hHD);
    hdEnable(HD_FORCE_OUTPUT);
    hdStartScheduler();

    if (HD_DEVICE_ERROR(error = hdGetError())) {
        hdStopScheduler();
        hdDisableDevice(hHD);
        return 0; // si falla aquí y no hay dispositivo, salir OK
    }

    // Query current position as a quick smoke test
    double position[3] = {0.0, 0.0, 0.0};
    hdBeginFrame(hHD);
    hdGetDoublev(HD_CURRENT_POSITION, position);
    hdEndFrame(hHD);

    (void)error;

    // Shutdown
    hdStopScheduler();
    hdDisableDevice(hHD);

    return 0;
}
