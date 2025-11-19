// ==============================================================================
// HAPTIC CONTROLLER HEADER
// Functions for Phantom Omni interface using OpenHaptics
// ==============================================================================

#ifndef HAPTIC_CONTROLLER_H
#define HAPTIC_CONTROLLER_H

#include <HD/hd.h>
#include <atomic>

// ==============================================================================
// DATA STRUCTURES
// ==============================================================================

struct HapticSample {
    double position[3];   // X, Y, Z position in mm
    double transform[16]; // 4x4 transformation matrix
    double timestamp;     // Sample timestamp
};

// ==============================================================================
// GLOBAL STATE
// ==============================================================================

extern HHD g_hDevice;
extern std::atomic<bool> g_hSampleReady;
extern HapticSample g_hSample;
extern std::atomic<int> g_hButtons;

// ==============================================================================
// FUNCTION DECLARATIONS
// ==============================================================================

/**
 * Initializes the Phantom Omni haptic device.
 * Starts the scheduler and registers the servo callback.
 * Returns true if successful.
 */
bool initializeHapticDevice();

/**
 * Stops the haptic scheduler and disables the device.
 */
void shutdownHapticDevice();

/**
 * Main haptic servo loop callback (runs at ~1kHz).
 * Reads position, transform, and button state.
 * DO NOT put heavy computation or blocking calls here.
 */
HDCallbackCode HDCALLBACK hapticsServoCallback(void* user);

/**
 * Returns the current button state bitmask.
 * Bit 0: Button 1, Bit 1: Button 2.
 */
int getHapticButtons();

/**
 * Checks if new haptic data is available.
 */
bool isHapticDataReady();

/**
 * Copies the latest haptic position to the provided array.
 * @param pos Array of 3 doubles [x, y, z]
 */
void getHapticPosition(double* pos);

/**
 * Copies the latest haptic transform matrix to the provided array.
 * @param transform Array of 16 doubles (4x4 matrix)
 */
void getHapticTransform(double* transform);

#endif // HAPTIC_CONTROLLER_H
