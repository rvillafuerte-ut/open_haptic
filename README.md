# Modular Robot Control System

This project implements a teleoperation control system for a robotic arm using Dynamixel motors and a Phantom Omni haptic device. The code has been refactored into modular components for better maintainability and extensibility.

## Project Structure

- **src/**: Contains implementation files (.cpp)
- **include/**: Contains header files (.h) with function declarations and documentation
- **CMakeLists.txt**: Build configuration

## Modules

1. **Dynamixel Controller** (`dynamixel_controller.h`)
   - Handles low-level motor communication (read/write)
   - Configures operating modes and limits
   - Converts between ticks and radians

2. **Haptic Controller** (`haptic_controller.h`)
   - Interfaces with the Phantom Omni device
   - Runs a high-frequency servo loop (1kHz)
   - Provides position and button data

3. **Robot Controller** (`robot_controller.h`)
   - Implements Forward Kinematics and Jacobian
   - Computes control torques (Computed Torque Control)
   - Contains the dynamics model (OMDyn)

4. **Trajectory Generator** (`trajectory_generator.h`)
   - Generates desired paths from haptic input
   - Supports circular trajectories and keyboard control
   - Applies filtering and workspace limits

5. **Gripper Controller** (`gripper_controller.h`)
   - Controls the end-effector (ID 15)
   - Handles open/close logic and calibration

6. **Safety Monitor** (`safety_monitor.h`)
   - Checks for current overloads and workspace violations
   - Triggers emergency stops

7. **Utilities** (`utilities.h`)
   - Helper functions for keyboard input and display

## How to Build

```bash
mkdir build
cd build
cmake ..
make
```

## How to Run

```bash
./build/modular_arm_control 25 0 10 0.9
```

## Controls

- **Button 1**: Enable/Disable Teleoperation
- **Button 2 (Short)**: Toggle Gripper Open/Close
- **Button 2 (Long)**: Flip Gripper Direction
- **Ctrl+C**: Emergency Stop and Exit
