# Open Haptic - Control Teleoperado de Brazo Robótico

Sistema de control de brazo robótico de 4 DOF con dispositivo háptico Phantom Omni, usando Dynamixel servos y control de gripper.

## 📋 Requisitos del Sistema

### Hardware
- Brazo robótico con 4 servos Dynamixel (IDs: 11, 12, 13, 14)
- Gripper con servo Dynamixel (ID: 15)
- Dispositivo háptico Phantom Omni (3D Systems)
- Adaptador USB-Serial para Dynamixel (FTDI)
- Ubuntu 20.04 / 22.04 (64-bit)

### Software - Versiones Necesarias

#### 1. Sistema Base
```bash
# Ubuntu 20.04 LTS o 22.04 LTS
# Verificar versión
lsb_release -a
```

#### 2. Compilador y CMake
```bash
# GCC con soporte C++17
sudo apt update
sudo apt install -y build-essential cmake git

# Verificar versiones
gcc --version      # >= 9.0
g++ --version      # >= 9.0
cmake --version    # >= 3.16
```

#### 3. Eigen 3 (Álgebra Lineal)
```bash
# Instalar Eigen3 (header-only)
sudo apt install -y libeigen3-dev

# O instalar manualmente en ~/libs_utils/eigen
cd ~
mkdir -p libs_utils
cd libs_utils
git clone https://gitlab.com/libeigen/eigen.git
cd eigen
git checkout 3.4.0
```

#### 4. Dynamixel SDK
```bash
# Clonar repositorio oficial
cd ~
git clone https://github.com/ROBOTIS-GIT/DynamixelSDK.git
cd DynamixelSDK/c++/build/linux64

# Compilar e instalar
make
sudo make install

# Verificar instalación
ls /usr/local/lib/libdxl_x64_cpp.so
```

#### 5. OpenHaptics SDK (3D Systems)
```bash
# Descargar desde: https://support.3dsystems.com/s/article/OpenHaptics-for-Linux-Developer-Edition-v34
# Archivo: openhaptics_3.4-0-developer-edition-amd64.tar.gz

# Extraer e instalar
tar -xzf openhaptics_3.4-0-developer-edition-amd64.tar.gz
cd openhaptics_3.4-0-developer-edition-amd64
sudo ./install

# El SDK se instala en: /opt/OpenHaptics/Developer/3.4-0/

# Verificar instalación
ls /opt/OpenHaptics/Developer/3.4-0/lib/libHD.so
ls /opt/OpenHaptics/Developer/3.4-0/lib/libHL.so
```

#### 6. Configurar Variables de Entorno
Agregar al final de `~/.bashrc`:
```bash
# OpenHaptics
export OH_SDK_BASE=/opt/OpenHaptics/Developer/3.4-0
export LD_LIBRARY_PATH=$OH_SDK_BASE/lib:$LD_LIBRARY_PATH

# Dynamixel SDK (si es necesario)
export LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH
```

Luego recargar:
```bash
source ~/.bashrc
```

---

## 🔧 Configuración de Hardware

### Permisos y Latencia USB

#### 1. Configuración Automática (Recomendado)
```bash
# Copiar reglas udev
sudo cp 99-haptic-devices.rules /etc/udev/rules.d/

# Recargar reglas
sudo udevadm control --reload-rules
sudo udevadm trigger

# Verificar (desconectar y reconectar dispositivos)
ls -la /dev/ttyUSB0  # Dynamixel (debe mostrar permisos 0666)
ls -la /dev/ttyACM0  # Haptic device (debe mostrar permisos 0666)
```

#### 2. Configuración Manual (cada vez que conectas)
```bash
# Dar permisos a ambos puertos
sudo chmod 777 /dev/ttyUSB0   # Dynamixel
sudo chmod 777 /dev/ttyACM0   # Haptic device

# Configurar latencia ultra-baja
sudo ./setup_usb_low_latency.sh /dev/ttyUSB0 /dev/ttyACM0
```

### Verificar Dispositivos
```bash
# Listar dispositivos USB conectados
lsusb

# Verificar puertos serie
ls -la /dev/ttyUSB*
ls -la /dev/ttyACM*

# Ver información del dispositivo háptico
lsusb | grep -i "3D Systems\|Phantom"
```

---

## 🏗️ Compilación

```bash
cd ~/open_haptic

# Compilar
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j4

# Verificar binarios
ls build/test_arm_control
ls build/open_haptic_demo
```

---

## 🚀 Uso

### Control Teleoperado con Haptics

```bash
# Sintaxis
./build/test_arm_control <lambda> <dz> <bf> <vf>

# Ejemplo típico
./build/test_arm_control 25 0 10 0.9
```

**Parámetros:**
- `lambda`: Ganancia del controlador (ej: 25)
- `dz`: Dead zone (normalmente 0)
- `bf`: Factor de bias (ej: 10)
- `vf`: Factor de velocidad (ej: 0.9)

### Controles del Dispositivo Háptico

#### Botón 1 (Teleoperación)
- **Primera pulsación corta**: Arma teleoperación
  - Captura posición actual del brazo como "home"
  - Inicia control teleoperado
- **Pulsaciones posteriores**: Apaga de forma segura

#### Botón 2 (Gripper)
- **Pulsación corta**: Toggle abrir/cerrar
  - Cerrado: Posición calibrada al inicio
  - Abierto: ~120° desde cerrado
- **Pulsación larga (>0.8s)**: Invierte dirección de apertura
  - Útil si la mecánica está al revés

### Mapeo de Coordenadas
- **Haptic X** (izq/der) → **Arm Y** (izq/der)
- **Haptic Y** (arriba/abajo) → **Arm Z** (arriba/abajo)
- **Haptic Z** (adelante/atrás) → **Arm X** (adelante/atrás)
- **Haptic Roll** → **Arm Yaw** (rotación)

### Escalas de Sensibilidad
Ajustables en `src/CNL_L6_act_2.cpp` (~líneas 767-770):
```cpp
const double x_scale = 0.0010;  // X axis (forward/back)
const double y_scale = 0.0011;  // Y axis (left/right)
const double z_scale = 0.0010;  // Z axis (up/down)
const double ang_scale = 0.80;  // Yaw rotation
```

### Parámetros del Gripper
Ajustables en `src/CNL_L6_act_2.cpp` (~líneas 65-69):
```cpp
#define GRIPPER_CURR_LIMIT      260   // Límite de corriente (seguridad)
#define GRIPPER_CURR_OPEN       240   // Corriente para abrir
#define GRIPPER_CURR_CLOSE      180   // Corriente para cerrar/sujetar
#define GRIPPER_OPEN_TICKS      1400  // Apertura (~120°)
#define GRIPPER_OPEN_DIR        (-1)  // Dirección: -1 o +1
```

---

## 📊 Análisis de Datos

### Visualización de Logs
```bash
# Generar gráficas de posición y tracking
python3 tools/plot_log.py

# Las gráficas se guardan en plots/
ls plots/*.png
```

### Formato del Log
Archivo `data_log.txt` con 17 columnas:
```
t, y0, y1, y2, y3, yd0, yd1, yd2, yd3, dq0, dq1, dq2, dq3, cur1, cur2, cur3, cur4
```
- `y0-3`: Posición medida [x, y, z, theta]
- `yd0-3`: Posición deseada [x, y, z, theta]
- `dq0-3`: Velocidades articulares
- `cur1-4`: Corrientes de los 4 motores

---

## ⚠️ Seguridad

### Límites de Corriente
- **Motores del brazo**: 290 mA (normal), 330 mA (pico seguro)
- **Gripper**: 220-260 mA (configurable)

### Procedimiento Seguro
1. **Siempre sostén el brazo** al armar/desarmar teleoperación
2. **Verifica espacio libre** de obstáculos
3. **Comienza con escalas bajas** (0.001 para posición)
4. **Monitorea corrientes** durante operación
5. **Ten el botón de parada listo** (Botón 1)

### Detección de Corrientes Peligrosas
El sistema detiene automáticamente si detecta corrientes > 330 mA.

---

## 🛠️ Troubleshooting

### Problemas Comunes

#### "Permission denied" en /dev/ttyUSB0 o /dev/ttyACM0
```bash
# Solución temporal
sudo chmod 777 /dev/ttyUSB0
sudo chmod 777 /dev/ttyACM0

# Solución permanente
sudo cp 99-haptic-devices.rules /etc/udev/rules.d/
sudo udevadm control --reload-rules
```

#### Gripper no se mueve
1. Verifica en los logs: "Gripper calibrated: closed=XXXX, open=YYYY"
2. Intenta pulsación larga (>0.8s) en botón 2 para invertir dirección
3. Aumenta `GRIPPER_CURR_OPEN` si es necesario

#### "Unsafe" corrientes altas al iniciar
1. Verifica que el brazo esté en posición home inicial
2. Reduce ganancias del controlador (lambda)
3. Reduce escalas de sensibilidad

#### Dispositivo háptico no detectado
```bash
# Verificar presencia
lsusb | grep -i phantom

# Verificar puerto
ls -la /dev/ttyACM*

# Reiniciar servicio OpenHaptics
sudo systemctl restart phantom
```

#### Lag o movimientos erráticos
```bash
# Configurar latencia baja
sudo ./setup_usb_low_latency.sh /dev/ttyUSB0 /dev/ttyACM0

# Verificar latency timer
cat /sys/bus/usb-serial/devices/ttyUSB0/latency_timer
# Debe ser: 1 (no 16)
```

---

## 📁 Estructura del Proyecto

```
open_haptic/
├── src/
│   ├── CNL_L6_act_2.cpp           # Control principal con haptics
│   ├── main.cpp                    # Demo básico haptics
│   └── CNL_L6_joint_teleop.cpp    # Control en espacio articular
├── tools/
│   └── plot_log.py                 # Visualización de datos
├── build/                          # Binarios compilados
├── plots/                          # Gráficas generadas
├── CMakeLists.txt                  # Configuración CMake
├── setup_usb_low_latency.sh        # Script de configuración USB
├── 99-haptic-devices.rules         # Reglas udev
├── README.md                       # Este archivo
└── TELEOP_USAGE.md                # Guía detallada de teleoperación
```

---

## 📚 Documentación Adicional

- **TELEOP_USAGE.md**: Guía detallada de uso de teleoperación
- **README_teleop_joint.md**: Control en espacio articular
- [Dynamixel SDK Docs](https://emanual.robotis.com/docs/en/software/dynamixel/dynamixel_sdk/overview/)
- [OpenHaptics Programmer's Guide](https://support.3dsystems.com/s/)
- [Eigen Documentation](https://eigen.tuxfamily.org/dox/)

---

## 🤝 Contribuciones

Para reportar problemas o sugerir mejoras, contacta al equipo de desarrollo.

---

## 📄 Licencia

Este proyecto utiliza componentes con diferentes licencias:
- **OpenHaptics**: Developer Edition (no comercial)
- **Dynamixel SDK**: Apache 2.0
- **Eigen**: MPL2

---

**Última actualización**: Octubre 2025
