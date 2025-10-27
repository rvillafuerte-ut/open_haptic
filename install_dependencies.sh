#!/bin/bash
# Script de instalación automática de dependencias para Open Haptic
# Ubuntu 20.04 / 22.04
# Uso: sudo ./install_dependencies.sh

set -e  # Salir si hay error

echo "=========================================="
echo "Instalador de Dependencias - Open Haptic"
echo "=========================================="
echo ""

# Verificar que se ejecute como root
if [ "$EUID" -ne 0 ]; then 
    echo "Error: Este script debe ejecutarse con sudo"
    echo "Uso: sudo ./install_dependencies.sh"
    exit 1
fi

# Obtener el usuario real (no root)
REAL_USER=${SUDO_USER:-$USER}
REAL_HOME=$(eval echo ~$REAL_USER)

echo "Usuario: $REAL_USER"
echo "Home: $REAL_HOME"
echo ""

# 1. Actualizar sistema
echo "[1/7] Actualizando sistema..."
apt update
apt upgrade -y

# 2. Instalar herramientas base
echo ""
echo "[2/7] Instalando compilador y CMake..."
apt install -y build-essential cmake git wget curl
apt install -y gcc g++ make

# 3. Instalar Eigen3
echo ""
echo "[3/7] Instalando Eigen3..."
apt install -y libeigen3-dev

# Verificar instalación
if [ -f "/usr/include/eigen3/Eigen/Core" ]; then
    echo "✓ Eigen3 instalado correctamente"
else
    echo "⚠ Eigen3 no encontrado, instalando manualmente..."
    mkdir -p $REAL_HOME/libs_utils
    cd $REAL_HOME/libs_utils
    if [ ! -d "eigen" ]; then
        sudo -u $REAL_USER git clone https://gitlab.com/libeigen/eigen.git
        cd eigen
        sudo -u $REAL_USER git checkout 3.4.0
    fi
    chown -R $REAL_USER:$REAL_USER $REAL_HOME/libs_utils
    echo "✓ Eigen3 instalado en $REAL_HOME/libs_utils/eigen"
fi

# 4. Instalar Dynamixel SDK
echo ""
echo "[4/7] Instalando Dynamixel SDK..."
cd $REAL_HOME
if [ ! -d "DynamixelSDK" ]; then
    sudo -u $REAL_USER git clone https://github.com/ROBOTIS-GIT/DynamixelSDK.git
fi
cd DynamixelSDK/c++/build/linux64
make clean
make
make install
ldconfig

# Verificar instalación
if [ -f "/usr/local/lib/libdxl_x64_cpp.so" ]; then
    echo "✓ Dynamixel SDK instalado correctamente"
else
    echo "✗ Error: Dynamixel SDK no se instaló correctamente"
    exit 1
fi

# 5. Verificar OpenHaptics
echo ""
echo "[5/6] Verificando OpenHaptics SDK..."
if [ -d "/opt/OpenHaptics/Developer/3.4-0" ]; then
    echo "✓ OpenHaptics SDK encontrado"
else
    echo "✗ OpenHaptics SDK NO encontrado"
    echo ""
    echo "ACCIÓN REQUERIDA:"
    echo "1. Descargar desde: https://support.3dsystems.com/s/article/OpenHaptics-for-Linux-Developer-Edition-v34"
    echo "2. Extraer: tar -xzf openhaptics_3.4-0-developer-edition-amd64.tar.gz"
    echo "3. Instalar: sudo ./install"
    echo ""
    echo "Presiona Enter para continuar con el resto de la instalación..."
    read
fi

# 6. Configurar permisos USB
echo ""
echo "[6/6] Configurando permisos USB..."
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

if [ -f "$SCRIPT_DIR/99-haptic-devices.rules" ]; then
    cp "$SCRIPT_DIR/99-haptic-devices.rules" /etc/udev/rules.d/
    udevadm control --reload-rules
    udevadm trigger
    echo "✓ Reglas udev instaladas"
else
    echo "⚠ Archivo 99-haptic-devices.rules no encontrado"
fi

# Configurar variables de entorno
echo ""
echo "Configurando variables de entorno..."
ENV_SETUP="
# OpenHaptics SDK
export OH_SDK_BASE=/opt/OpenHaptics/Developer/3.4-0
export LD_LIBRARY_PATH=\$OH_SDK_BASE/lib:\$LD_LIBRARY_PATH

# Dynamixel SDK
export LD_LIBRARY_PATH=/usr/local/lib:\$LD_LIBRARY_PATH
"

# Agregar a .bashrc si no existe
if ! grep -q "OH_SDK_BASE" "$REAL_HOME/.bashrc"; then
    sudo -u $REAL_USER bash -c "echo '$ENV_SETUP' >> $REAL_HOME/.bashrc"
    echo "✓ Variables de entorno agregadas a .bashrc"
else
    echo "✓ Variables de entorno ya configuradas"
fi

# Resumen
echo ""
echo "=========================================="
echo "Instalación Completada"
echo "=========================================="
echo ""
echo "Versiones instaladas:"
gcc --version | head -1
g++ --version | head -1
cmake --version | head -1
echo ""
echo "Bibliotecas:"
echo "  Eigen3: $(dpkg -l | grep libeigen3-dev | awk '{print $3}')"
[ -f "/usr/local/lib/libdxl_x64_cpp.so" ] && echo "  ✓ Dynamixel SDK instalado"
[ -d "/opt/OpenHaptics/Developer/3.4-0" ] && echo "  ✓ OpenHaptics SDK instalado"
echo ""
echo "Próximos pasos:"
echo "1. Cerrar y reabrir la terminal (para cargar variables de entorno)"
echo "2. Conectar dispositivos USB (Dynamixel y Haptic)"
echo "3. Ejecutar: sudo ./setup_usb_low_latency.sh"
echo "4. Compilar: cmake -B build && cmake --build build -j4"
echo "5. Ejecutar: ./build/test_arm_control 25 0 10 0.9"
echo ""
echo "Documentación completa: README.md"
echo "=========================================="
