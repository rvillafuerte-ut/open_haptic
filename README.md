# open_haptic

Minimal C++ project integrating **Eigen**, **OpenHaptics**, **DynamixelSDK**, and **Pinocchio**.

## Dependencies

- CMake >= 3.16
- C++17 compiler (g++)
- **Eigen 3** (header-only): `~/libs_utils/eigen`
- **OpenHaptics SDK**: `/opt/OpenHaptics/Developer/3.4-0`
- **DynamixelSDK**: `/usr/local/lib/libdxl_x64_cpp.so`
- **Pinocchio**: `/usr/local/lib/libpinocchio_default.so`

## Build

```bash
cmake -B build && make -C build -j
```

## Run

```bash
./build/open_haptic_demo --duration 5 --print-rate 20
```

Outputs: timestamp (s), position (mm), orientation roll/pitch/yaw (rad) at 1kHz servo rate.
