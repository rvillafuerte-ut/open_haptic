// ==============================================================================
// UTILITIES HEADER
// Helper functions for I/O, timing, and display
// ==============================================================================

#ifndef UTILITIES_H
#define UTILITIES_H

#include <csignal>
#include <Eigen/Dense>

// ==============================================================================
// GLOBAL STATE
// ==============================================================================

extern volatile sig_atomic_t shutdown_requested;
extern bool teleop_active;

// ==============================================================================
// FUNCTION DECLARATIONS
// ==============================================================================

/**
 * Signal handler for Ctrl+C (SIGINT).
 * Sets shutdown_requested flag to true.
 */
void signalHandler(int signum);

/**
 * Reads a single character from stdin without waiting for Enter.
 * Blocking call.
 */
int getch();

/**
 * Checks if a key has been pressed.
 * Non-blocking. Returns 1 if key pressed, 0 otherwise.
 */
int kbhit(void);

/**
 * Displays robot status in the terminal.
 * Updates periodically to avoid flooding output.
 * @param y Current position
 * @param yd Desired position
 * @param counter Loop counter for timing
 */
void displayStatus(const Eigen::Vector4d& y, 
                   const Eigen::Vector4d& yd,
                   int counter);

/**
 * Prints the startup banner.
 */
void printStartupBanner();

/**
 * Prints control instructions for the user.
 */
void printControlInstructions();

#endif // UTILITIES_H
