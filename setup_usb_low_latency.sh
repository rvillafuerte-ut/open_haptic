#!/bin/bash
# Script para configurar latencia ultra-baja en puerto USB para Dynamixel y Haptics
# Uso: sudo ./setup_usb_low_latency.sh [puerto1] [puerto2] ...
# Ejemplo: sudo ./setup_usb_low_latency.sh /dev/ttyUSB0 /dev/ttyACM0

# Si no se pasan argumentos, usar puertos por defecto
if [ $# -eq 0 ]; then
    PORTS=("/dev/ttyUSB0" "/dev/ttyACM0")
else
    PORTS=("$@")
fi

for PORT in "${PORTS[@]}"; do
    echo ""
    echo "=========================================="
    echo "Procesando puerto: $PORT"
    echo "=========================================="

if [ ! -e "$PORT" ]; then
    echo "⚠ Puerto $PORT no existe, saltando..."
    continue
fi

echo "Configurando $PORT para latencia ultra-baja..."

# Obtener el dispositivo USB correspondiente
USB_DEV=$(udevadm info -q path -n $PORT | grep -oP 'usb\d+/\d+-\d+' | head -1)

if [ -z "$USB_DEV" ]; then
    echo "Error: No se pudo encontrar el dispositivo USB"
    exit 1
fi

# Reducir latencia del timer (default: 16ms → 1ms)
LATENCY_PATH="/sys/bus/usb-serial/devices/$(basename $PORT)/latency_timer"
if [ -e "$LATENCY_PATH" ]; then
    echo 1 > $LATENCY_PATH
    echo "✓ Latency timer configurado a 1ms (antes: 16ms)"
else
    echo "⚠ No se encontró latency_timer en $LATENCY_PATH"
fi

# Configurar permisos del puerto
chmod 666 $PORT
echo "✓ Permisos del puerto configurados"

# Configurar prioridad del proceso USB
USB_IRQ=$(cat /proc/interrupts | grep -i "usb" | grep -oP '^\s*\d+' | head -1)
if [ ! -z "$USB_IRQ" ]; then
    # Esto requiere permisos de root
    if [ -e "/proc/irq/$USB_IRQ/smp_affinity" ]; then
        echo 1 > /proc/irq/$USB_IRQ/smp_affinity
        echo "✓ Afinidad de CPU configurada para IRQ USB $USB_IRQ"
    fi
fi

# Deshabilitar power management USB
USB_DEVICE_PATH="/sys/bus/usb/devices/$USB_DEV"
if [ -e "$USB_DEVICE_PATH/power/control" ]; then
    echo on > $USB_DEVICE_PATH/power/control
    echo "✓ Power management USB deshabilitado"
fi

# Mostrar configuración actual
echo ""
echo "=== Configuración actual ==="
if [ -e "$LATENCY_PATH" ]; then
    echo "Latency timer: $(cat $LATENCY_PATH) ms"
fi
echo "Puerto: $PORT"
ls -la $PORT
echo ""

done

echo ""
echo "=========================================="
echo "✓ Configuración completada para todos los puertos"
echo "Los puertos están listos para control de alta frecuencia."
echo "=========================================="
echo "usar ./build/open_haptic 25 0 10 0.9"
