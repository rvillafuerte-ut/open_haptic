// ==============================================================================
// HAPTIC CONTROLLER IMPLEMENTATION
// ==============================================================================

#include "haptic_controller.h"
#include <cstdio>

// Global state definitions
HHD g_hDevice = HD_INVALID_HANDLE;
std::atomic<bool> g_hSampleReady{false};
HapticSample g_hSample;
std::atomic<int> g_hButtons{0};

HDCallbackCode HDCALLBACK hapticsServoCallback(void* /*user*/) {
    hdBeginFrame(g_hDevice);
    hdGetDoublev(HD_CURRENT_POSITION, g_hSample.position);
    hdGetDoublev(HD_CURRENT_TRANSFORM, g_hSample.transform);
    g_hSample.timestamp = hdGetSchedulerTimeStamp();
    int buttons = 0;
    hdGetIntegerv(HD_CURRENT_BUTTONS, &buttons);
    g_hButtons.store(buttons, std::memory_order_release);
    hdEndFrame(g_hDevice);
    g_hSampleReady.store(true, std::memory_order_release);
    return HD_CALLBACK_CONTINUE;
}

bool initializeHapticDevice() {
    HDErrorInfo error;
    g_hDevice = hdInitDevice(HD_DEFAULT_DEVICE);
    if (HD_DEVICE_ERROR(error = hdGetError())) {
        printf("✗ Failed to initialize haptic device: %s\n", hdGetErrorString(error.errorCode));
        return false;
    }
    
    hdEnable(HD_FORCE_OUTPUT);
    hdStartScheduler();
    if (HD_DEVICE_ERROR(error = hdGetError())) {
        printf("✗ Failed to start haptic scheduler: %s\n", hdGetErrorString(error.errorCode));
        return false;
    }
    
    hdScheduleAsynchronous(hapticsServoCallback, nullptr, HD_MAX_SCHEDULER_PRIORITY);
    printf("✓ Haptic device initialized\n");
    return true;
}

void shutdownHapticDevice() {
    if (g_hDevice != HD_INVALID_HANDLE) {
        hdStopScheduler();
        hdDisableDevice(g_hDevice);
        g_hDevice = HD_INVALID_HANDLE;
        printf("✓ Haptic device shut down\n");
    }
}

int getHapticButtons() {
    return g_hButtons.load(std::memory_order_acquire);
}

bool isHapticDataReady() {
    return g_hSampleReady.load(std::memory_order_acquire);
}

void getHapticPosition(double pos[3]) {
    if (g_hSampleReady.load(std::memory_order_acquire)) {
        pos[0] = g_hSample.position[0];
        pos[1] = g_hSample.position[1];
        pos[2] = g_hSample.position[2];
    }
}

void getHapticTransform(double transform[16]) {
    if (g_hSampleReady.load(std::memory_order_acquire)) {
        for (int i = 0; i < 16; i++) {
            transform[i] = g_hSample.transform[i];
        }
    }
}
