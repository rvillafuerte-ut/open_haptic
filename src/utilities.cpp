// ==============================================================================
// UTILITIES IMPLEMENTATION
// ==============================================================================

#include "utilities.h"
#include <Eigen/Dense>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstdio>

#define STDIN_FILENO 0

// Global state definitions
volatile sig_atomic_t shutdown_requested = 0;
bool teleop_active = false;

void signalHandler(int signum) {
    shutdown_requested = 1;
}

int getch() {
    struct termios oldt, newt;
    int ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}

int kbhit(void) {
    struct termios oldt, newt;
    int ch, oldf;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);
    if (ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }
    return 0;
}

void displayStatus(const Eigen::Vector4d& y, 
                   const Eigen::Vector4d& yd,
                   int counter) {
    
    const int DISPLAY_INTERVAL = 10;
    
    if (counter % DISPLAY_INTERVAL == 0) {
        printf("\r\033[K");  // Clear line
        printf("X: %+.3f→%+.3f  Y: %+.3f→%+.3f  Z: %+.3f→%+.3f  θ: %+.2f→%+.2f",
               (float)y(0), (float)yd(0),
               (float)y(1), (float)yd(1),
               (float)y(2), (float)yd(2),
               (float)y(3), (float)yd(3));
        fflush(stdout);
    }
}

void printStartupBanner() {
    printf("\n");
    printf("╔════════════════════════════════════════════════════════════════╗\n");
    printf("║         ROBOTIC ARM TELEOPERATION CONTROL SYSTEM              ║\n");
    printf("║                    Modular Version 2.0                        ║\n");
    printf("╚════════════════════════════════════════════════════════════════╝\n");
    printf("\n");
}

void printControlInstructions() {
    printf("╔════════════════════════════════════════════════════════════════╗\n");
    printf("║                      CONTROL INSTRUCTIONS                      ║\n");
    printf("╠════════════════════════════════════════════════════════════════╣\n");
    printf("║  Button 1:        ARM/DISARM teleop control                   ║\n");
    printf("║  Button 2 (short): Toggle gripper open/close                  ║\n");
    printf("║  Button 2 (long):  Flip gripper direction                     ║\n");
    printf("║  Ctrl+C:          Emergency stop and quit                     ║\n");
    printf("╚════════════════════════════════════════════════════════════════╝\n");
    printf("\n");
}
