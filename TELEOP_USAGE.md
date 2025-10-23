# Uso del Control Teleoperado con Haptics

## Cambios Importantes (Última Actualización)

### Problema Resuelto: "Unsafe" Inmediato al Armar
**Causa:** El sistema usaba una posición de referencia fija que no coincidía con la posición actual del brazo, causando errores grandes → corrientes excesivas.

**Solución:** Ahora `yd_home` se captura automáticamente como la **posición actual del brazo** en el primer ciclo, garantizando error cero al inicio.

## Flujo de Operación

### 1. Inicio del Programa
```bash
cd /home/utec/open_haptic/build
./test_arm_control 10 5 10 0.5
```

### 2. Antes de Armar
- El programa mantiene `yd = y` (posición actual) continuamente
- **Error siempre = 0 → brazo completamente quieto**
- No hay movimientos inesperados al iniciar

### 3. Armado del Sistema (Botón Haptics)
- **Sostén el brazo con una mano**
- Presiona **botón 1** del Phantom Omni
- **EN ESE MOMENTO se captura `yd_home = y` (error = 0 garantizado)**
- El programa imprime:
  - `"Home captured at arming: [x, y, z, theta]"`
  - `"Teleop armed via haptic button. Press any button again to stop safely."`
  - `"Haptic base captured: [x, y, z, roll]"`
- A partir de este momento, el brazo sigue **variaciones (deltas)** del haptics

### 4. Control Teleoperado Activo
- Mueve el Phantom Omni
- El brazo sigue: `yd = yd_home + scale * (haptic_actual - haptic_base)`
- **Escalas aplicadas:**
  - Traslación: 0.5× (más suave)
  - Rotación: 0.2× (más suave, era 1.0)
- **Límites de seguridad:**
  - Desplazamiento máximo: ±20 cm en x,y,z
  - Rotación máxima: ±0.5 rad

### 5. Detención Segura
- **Presiona cualquier botón del Phantom** (incluido el mismo botón 1)
- El programa imprime: `"Button pressed - initiating safe shutdown..."`
- El loop termina, los servos se deshabilitan
- **Sostén el brazo durante el shutdown**

## Parámetros de Seguridad

### Corrientes Límite
- `curr_max = 290` (límite de operación normal)
- `curr_peak = 330` (límite de seguridad; al superarlo → unsafe shutdown)

### Escalas de Control
Editables en línea ~563 de `CNL_L6_act_2.cpp`:
```cpp
const double pos_scale = 0.5;   // escala traslación
const double ang_scale = 0.20;  // escala rotación
```

### Límites de Delta
Editables en línea ~566:
```cpp
Eigen::Vector4d max_delta; 
max_delta<<0.20,0.20,0.20,0.5; // [m,m,m,rad]
```

## Diagnóstico de Problemas

### "Unsafe" Inmediato al Armar
- **Problema anterior:** yd_home capturado al inicio → brazo se movía un poco al prender
- **Solución final:** 
  - Antes de armar: `yd = y` continuamente (error siempre 0)
  - Al armar: captura `yd_home = y` en ese instante exacto
  - **Resultado: cero movimiento inesperado, error = 0 al armar**

### Motor con Corriente Excesiva
1. Verifica que el brazo no esté en singularidad
2. Reduce las escalas (`pos_scale`, `ang_scale`)
3. Reduce los límites de delta (`max_delta`)
4. Verifica las ganancias del controlador (lambda, k3)

### "Double Free" o Crashes
- **Solucionado:** Añadida limpieza apropiada de dispositivo HD en shutdown
- Si persiste, verifica que no haya múltiples instancias corriendo

## Generación de Gráficas

Después de un experimento:
```bash
cd /home/utec/open_haptic
python3 tools/plot_log.py
```

Las gráficas en `plots/` mostrarán:
- `yd` real (home o home+deltas haptics)
- `y` medido
- Comparación para x, y, z, theta

## Archivo de Log

Formato actual (17 columnas):
```
t, y0, y1, y2, y3, yd0, yd1, yd2, yd3, dq0, dq1, dq2, dq3, cur1, cur2, cur3, cur4
```

Donde:
- `y0-3`: posición medida [x, y, z, theta]
- `yd0-3`: posición deseada [x, y, z, theta] (incluye deltas haptics cuando está armado)
- `dq0-3`: velocidades articulares
- `cur1-4`: corrientes de los 4 motores

## Notas de Seguridad

⚠️ **SIEMPRE:**
- Sostén el brazo al armar/desarmar
- Ten listo el botón de parada
- Verifica espacio libre de obstáculos
- Ajusta escalas conservadoramente al inicio
- Monitorea las corrientes durante operación

✅ **Funcionamiento Normal:**
- Brazo quieto hasta armar (error ~0)
- Transición suave al armar
- Seguimiento proporcional de haptics
- Shutdown limpio al desarmar
