# Guía de Inicio Rápido - Open Haptic

## 🚀 Instalación Rápida (5 minutos)

### Opción A: Instalación Automática (Recomendado)
```bash
cd ~/open_haptic
sudo ./install_dependencies.sh
```

**Espera ~10-15 minutos** mientras se instala todo. Al finalizar:
```bash
# Cerrar y reabrir terminal, luego:
cmake -B build && cmake --build build -j4
```

### Opción B: Instalación Manual
Sigue las instrucciones detalladas en [README.md](README.md#-requisitos-del-sistema).

---

## ⚙️ Configuración Inicial (2 minutos)

### 1. Conectar Hardware
1. Conecta el adaptador USB Dynamixel → Puerto USB (ttyUSB0)
2. Conecta el Phantom Omni → Puerto USB (ttyACM0)
3. Enciende los servos Dynamixel

### 2. Configurar Permisos USB
```bash
cd ~/open_haptic

# Configuración permanente (hacer una vez)
sudo cp 99-haptic-devices.rules /etc/udev/rules.d/
sudo udevadm control --reload-rules
sudo udevadm trigger

# Desconectar y reconectar dispositivos USB

# Verificar permisos
ls -la /dev/ttyUSB0  # Debe mostrar: crw-rw-rw-
ls -la /dev/ttyACM0  # Debe mostrar: crw-rw-rw-
```

### 3. Configurar Latencia Baja
```bash
# Ejecutar cada vez que conectes los dispositivos
# (o configurar como servicio systemd)
sudo ./setup_usb_low_latency.sh
```

---

## 🎮 Primer Uso (3 minutos)

### 1. Compilar
```bash
cd ~/open_haptic
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j4
```

### 2. Ejecutar
```bash
# Sostén el brazo con una mano antes de ejecutar
./build/test_arm_control 25 0 10 0.9
```

### 3. Usar el Sistema

#### Paso a Paso
1. **Mantén sostenido el brazo** con una mano
2. **Presiona botón 1** del Phantom Omni → Se arma la teleoperación
3. **Mueve el dispositivo háptico** suavemente → El brazo sigue
4. **Presiona botón 2** (corto) → Abre/cierra gripper
5. **Presiona botón 1** de nuevo → Apaga de forma segura

#### Controles Básicos
- **Botón 1**: Armar/Apagar teleoperación
- **Botón 2 (corto)**: Toggle gripper abrir/cerrar
- **Botón 2 (largo >0.8s)**: Invertir dirección gripper

---

## 📊 Verificación

### Check de Funcionamiento Correcto
Al ejecutar, deberías ver:
```
✓ Succeeded to open the port!
✓ Succeeded to change the baudrate!
✓ Dynamixel#11-14 has been successfully connected
✓ Gripper#15 has been successfully connected
✓ Gripper calibrated: closed=XXXX, open=YYYY (ticks)
```

### Al Armar Teleoperación (Botón 1)
```
✓ Home captured at arming: [x, y, z, theta]
✓ Teleop armed via haptic button 1
✓ [ARMING] Initial error should be zero: err=[0, 0, 0, 0]
```

### Al Usar Gripper (Botón 2)
```
✓ Gripper OPENED (goal_pos: YYYY, present_pos: ~YYYY, goal_current: 240)
✓ Gripper CLOSED (goal_pos: XXXX, present_pos: ~XXXX, goal_current: 180)
```

---

## ⚠️ Problemas Comunes y Soluciones Rápidas

### "Permission denied" en /dev/ttyUSB0 o /dev/ttyACM0
```bash
# Solución rápida
sudo chmod 777 /dev/ttyUSB0 /dev/ttyACM0

# Solución permanente (hacer una vez)
sudo cp 99-haptic-devices.rules /etc/udev/rules.d/
sudo udevadm control --reload-rules
```

### "Failed to open the port"
```bash
# Verificar que los dispositivos estén conectados
ls /dev/ttyUSB*
ls /dev/ttyACM*

# Si no aparecen, reconectar dispositivos USB
```

### El brazo se mueve bruscamente al armar
```bash
# Reducir escalas de sensibilidad
# Editar src/CNL_L6_act_2.cpp líneas ~767-770:
const double x_scale = 0.0005;  // Reducir de 0.001 a 0.0005
const double y_scale = 0.0005;
const double z_scale = 0.0005;

# Recompilar
cmake --build build -j4
```

### Gripper no se mueve
```bash
# 1. Verificar calibración en los logs
#    Debe mostrar: "Gripper calibrated: closed=XXXX, open=YYYY"

# 2. Intentar invertir dirección
#    Mantener presionado botón 2 por 1 segundo

# 3. Si persiste, aumentar corriente
#    Editar src/CNL_L6_act_2.cpp línea ~66:
#    #define GRIPPER_CURR_OPEN  260  // Aumentar de 240 a 260
```

### Corrientes muy altas (>300 mA)
```bash
# 1. Verificar que el brazo esté en posición inicial
# 2. Reducir ganancias del controlador:
./build/test_arm_control 15 0 10 0.9  # Reducir de 25 a 15

# 3. Sostener el brazo al arrancar
```

---

## 📖 Documentación Completa

- **README.md**: Instalación detallada y referencia completa
- **TELEOP_USAGE.md**: Guía de uso de teleoperación
- **README_teleop_joint.md**: Control en espacio articular

---

## 🆘 Ayuda Rápida

### Comandos Útiles
```bash
# Ver dispositivos USB conectados
lsusb

# Ver puertos serie
ls -la /dev/tty*

# Verificar permisos
ls -la /dev/ttyUSB0 /dev/ttyACM0

# Configurar latencia
sudo ./setup_usb_low_latency.sh

# Recompilar
cmake --build build -j4

# Ver logs en tiempo real
./build/test_arm_control 25 0 10 0.9 | tee output.log
```

### Parámetros Comunes
```bash
# Conservador (para pruebas iniciales)
./build/test_arm_control 15 0 10 0.9

# Normal (uso diario)
./build/test_arm_control 25 0 10 0.9

# Agresivo (mayor respuesta)
./build/test_arm_control 35 0 10 0.9
```

---

## ✅ Checklist de Inicio

- [ ] Dependencias instaladas (`sudo ./install_dependencies.sh`)
- [ ] Código compilado (`cmake --build build -j4`)
- [ ] Dispositivos USB conectados
- [ ] Permisos configurados (`99-haptic-devices.rules`)
- [ ] Latencia configurada (`setup_usb_low_latency.sh`)
- [ ] Variables de entorno cargadas (cerrar/abrir terminal)
- [ ] Brazo en posición inicial
- [ ] Espacio libre de obstáculos

---

**¡Listo para usar!** 🎉

Si tienes problemas, consulta el README.md completo o la sección de Troubleshooting.
