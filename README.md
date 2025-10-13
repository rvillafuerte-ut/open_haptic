# open_haptic

Minimal C++ scaffold to combine Eigen and OpenHaptics in this workspace using CMake.

## Dependencies

- CMake >= 3.16
- A C++17 compiler (g++/clang++)
- Eigen 3 (header-only)
  - Example local clone: `~/libs_utils/eigen` (use its `Eigen/` folder)
- OpenHaptics SDK (e.g., under `/opt/OpenHaptics/Developer/3.4-0`)

## Configure include/library paths

You can provide paths in multiple ways:

- Eigen:
  - CMake: `-DEIGEN3_INCLUDE_DIR=/home/utec/libs_utils/eigen`
  - or env var: `EIGEN3_INCLUDE_DIR=/home/utec/libs_utils/eigen`
- OpenHaptics:
  - CMake: `-DOPENHAPTICS_INCLUDE_DIRS=/opt/OpenHaptics/Developer/3.4-0/include`
  - CMake: `-DOPENHAPTICS_LIB_DIR=/opt/OpenHaptics/Developer/3.4-0/lib64` (or `lib`)
  - Optionally set `OPENHAPTICS_ROOT` env var so the build can auto-detect `include` and `lib`.

By default the project attempts `/opt/OpenHaptics/Developer/3.4-0/include` and `/opt/OpenHaptics/Developer/3.4-0/lib[64]`.

## Build

```bash
# From repo root
cmake -S . -B build \
  -DEIGEN3_INCLUDE_DIR=/home/utec/libs_utils/eigen \
  -DOPENHAPTICS_INCLUDE_DIRS=/opt/OpenHaptics/Developer/3.4-0/include \
  -DOPENHAPTICS_LIB_DIR=/opt/OpenHaptics/Developer/3.4-0/lib64

cmake --build build -j
```

## Run

```bash
./build/open_haptic_demo
```

Notes:
- Running may require device permissions (udev rules) depending on your haptic device. Consult OpenHaptics docs.
- If the scheduler fails to start, ensure the device is connected and vendor services are running.

## What the demo does

- Uses Eigen to compute a simple vector cross product.
- Initializes the OpenHaptics HD device, reads current position once, then shuts down.

## Troubleshooting

- Eigen not found:
  - Ensure `EIGEN3_INCLUDE_DIR` points to the directory that contains the `Eigen/` folder (e.g., your `eigen` repo root).
- OpenHaptics headers not found:
  - Provide `-DOPENHAPTICS_INCLUDE_DIRS=...` to the SDK `include` directory.
- OpenHaptics libraries not found:
  - Provide `-DOPENHAPTICS_LIB_DIR=...` to the SDK `lib` or `lib64` directory.
  - If linking still fails, check which libraries the SDK provides (e.g., `libHD.so`, `libHL.so`, `libHDU.so`) and adjust `-DOPENHAPTICS_LIBS=HD;HL;HDU` accordingly.
