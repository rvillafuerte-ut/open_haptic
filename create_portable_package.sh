#!/bin/bash
# Script para crear paquete portable del proyecto Open Haptic
# Uso: ./create_portable_package.sh

set -e

# Nombre del paquete con fecha
PACKAGE_NAME="open_haptic_portable_$(date +%Y%m%d_%H%M%S)"
PACKAGE_FILE="$PACKAGE_NAME.tar.gz"

echo "=========================================="
echo "Creando Paquete Portable - Open Haptic"
echo "=========================================="
echo ""

# Verificar que estamos en el directorio correcto
if [ ! -f "CMakeLists.txt" ]; then
    echo "Error: Este script debe ejecutarse desde el directorio raíz del proyecto"
    exit 1
fi

echo "📦 Empaquetando archivos..."

# Crear tarball excluyendo archivos innecesarios
tar --exclude-from=.gitignore \
    --exclude='.git' \
    --exclude='.gitignore' \
    --exclude='build' \
    --exclude='build/*' \
    --exclude='.venv' \
    --exclude='venv' \
    --exclude='__pycache__' \
    --exclude='*.pyc' \
    --exclude='*.log' \
    --exclude='data_log.txt' \
    --exclude='data_log_joint.txt' \
    --exclude='analysis_summary.txt' \
    --exclude='plots/*.png' \
    --exclude='plots/*.pdf' \
    --exclude='plots/*.jpg' \
    --exclude='.vscode' \
    --exclude='.idea' \
    --exclude='*.swp' \
    --exclude='*~' \
    --exclude='.DS_Store' \
    -czf "$PACKAGE_FILE" \
    .

# Verificar creación exitosa
if [ ! -f "$PACKAGE_FILE" ]; then
    echo "✗ Error: No se pudo crear el paquete"
    exit 1
fi

# Mostrar información
echo ""
echo "=========================================="
echo "✓ Paquete Creado Exitosamente"
echo "=========================================="
echo ""
echo "Archivo: $PACKAGE_FILE"
echo "Tamaño: $(du -h "$PACKAGE_FILE" | cut -f1)"
echo "Ubicación: $(pwd)/$PACKAGE_FILE"
echo ""

# Listar contenido (primeros 30 archivos)
echo "Contenido del paquete (vista previa):"
echo "--------------------------------------"
tar -tzf "$PACKAGE_FILE" | head -30
TOTAL_FILES=$(tar -tzf "$PACKAGE_FILE" | wc -l)
echo "... (total: $TOTAL_FILES archivos)"
echo ""

# Instrucciones
echo "=========================================="
echo "📋 Instrucciones de Transferencia"
echo "=========================================="
echo ""
echo "1. Copiar archivo a la PC nueva:"
echo "   - USB: Copiar $PACKAGE_FILE a pendrive"
echo "   - Red: scp $PACKAGE_FILE usuario@pc-nueva:~"
echo "   - Nube: Subir a Drive/Dropbox"
echo ""
echo "2. En la PC nueva, extraer:"
echo "   tar -xzf $PACKAGE_FILE"
echo "   cd open_haptic"
echo ""
echo "3. Instalar dependencias:"
echo "   sudo ./install_dependencies.sh"
echo ""
echo "4. Cerrar y reabrir terminal, luego:"
echo "   cmake -B build -DCMAKE_BUILD_TYPE=Release"
echo "   cmake --build build -j4"
echo ""
echo "5. Configurar hardware:"
echo "   sudo cp 99-haptic-devices.rules /etc/udev/rules.d/"
echo "   sudo udevadm control --reload-rules"
echo ""
echo "Documentación completa: Ver PORTABILITY.md"
echo "=========================================="
echo ""

# Preguntar si mover a Desktop
read -p "¿Mover paquete a ~/Desktop? [y/N] " -n 1 -r
echo
if [[ $REPLY =~ ^[Yy]$ ]]; then
    mv "$PACKAGE_FILE" ~/Desktop/
    echo "✓ Paquete movido a ~/Desktop/$PACKAGE_FILE"
fi

echo ""
echo "¡Listo! 🎉"
