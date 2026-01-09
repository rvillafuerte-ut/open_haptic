# Modular Robot Control System

This project implements a teleoperation control system for a robotic arm using Dynamixel motors and a Phantom Omni haptic device.

## Dependencies and Installation

### System Requirements
- Ubuntu 20.04 or later
- CMake 3.10 or higher
- GCC/G++ compiler with C++11 support

### Required Libraries

#### 1. Dynamixel SDK
Install the official Dynamixel SDK for motor communication:
```bash
git clone https://github.com/ROBOTIS-GIT/DynamixelSDK.git
cd DynamixelSDK/c++/build/linux64
make
sudo make install
```

#### 2. OpenHaptics SDK
Download and install the OpenHaptics SDK (HD API) from 3D Systems:
- Visit: https://support.3dsystems.com/s/article/OpenHaptics-for-Linux-Developer-Edition-v34
- Extract and install:
```bash
tar -xzf openhaptics_*.tar.gz
cd openhaptics_*
sudo ./install
```

Set environment variables:
```bash
export OPENHAPTICS_INCLUDE_DIRS=/usr/include/HD
export OPENHAPTICS_LIB_DIR=/usr/lib64
```

#### 3. Eigen3
Install the Eigen linear algebra library:
```bash
sudo apt-get update
sudo apt-get install libeigen3-dev
export EIGEN3_INCLUDE_DIR=/usr/include/eigen3
```

#### 4. Additional Dependencies
```bash
sudo apt-get install build-essential cmake git
```

### USB Configuration
Configure USB permissions for Dynamixel and haptic device:
```bash
sudo cp 99-haptic-devices.rules /etc/udev/rules.d/
sudo udevadm control --reload-rules
sudo udevadm trigger
```

## How to Build

```bash
cmake -S . -B build
cmake --build build -j
```

## How to Run

```bash
./build/open_haptic 25 0 10 0.9
```

## Controls

- **Button 1**: Enable/Disable Teleoperation
- **Button 2 (Short)**: Toggle Gripper Open/Close
- **Button 2 (Long)**: Flip Gripper Direction
- **Ctrl+C**: Emergency Stop and Exit
