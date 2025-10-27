# Guía de Portabilidad - Open Haptic

## 📦 Transferir el Proyecto a Otra PC

### Método 1: Usando Git (Recomendado)

#### En la PC Original (ya configurado)
```bash
cd ~/open_haptic

# Ver qué archivos se subirán (verificar .gitignore)
git status

# Agregar cambios
git add .

# Commit
git commit -m "Proyecto completo listo para transferir"

# Push a GitHub
git push origin main
```

#### En la PC Nueva
```bash
# Clonar repositorio
git clone https://github.com/rvillafuerte-ut/open_haptic.git
cd open_haptic

# ¡Listo! Ya tienes todo el código fuente
```

---

### Método 2: Archivo Comprimido (Sin Git)

#### En la PC Original

##### Opción A: Comprimir solo código fuente (RECOMENDADO - ~50 KB)
```bash
cd ~/open_haptic

# Crear tarball solo con archivos importantes (respeta .gitignore)
tar --exclude-from=.gitignore \
    --exclude='.git' \
    --exclude='build' \
    --exclude='.venv' \
    --exclude='*.log' \
    --exclude='data_log*.txt' \
    -czf open_haptic_source.tar.gz \
    .

# Mover a una ubicación accesible
mv open_haptic_source.tar.gz ~/Desktop/

# Ver tamaño
ls -lh ~/Desktop/open_haptic_source.tar.gz
```

##### Opción B: Usar script preparado (más fácil)
```bash
cd ~/open_haptic
./create_portable_package.sh
```

#### En la PC Nueva
```bash
# Copiar el archivo .tar.gz a la nueva PC (USB, red, etc)

# Extraer
cd ~
tar -xzf open_haptic_source.tar.gz
cd open_haptic

# ¡Listo para instalar dependencias!
```

---

## 🔧 Configurar en la PC Nueva

### 1. Instalar Dependencias

```bash
cd ~/open_haptic

# Opción A: Instalador automático
sudo ./install_dependencies.sh

# Opción B: Manual (siguiendo README.md)
```

**IMPORTANTE**: Necesitas instalar OpenHaptics manualmente:
1. Descargar desde: https://support.3dsystems.com/s/article/OpenHaptics-for-Linux-Developer-Edition-v34
2. Extraer: `tar -xzf openhaptics_3.4-0-developer-edition-amd64.tar.gz`
3. Instalar: `sudo ./install`

### 2. Actualizar Rutas de Eigen (si es necesario)

Si instalaste Eigen en una ruta diferente, editar `CMakeLists.txt`:
```cmake
# Cambiar esta línea según tu instalación
set(EIGEN3_INCLUDE_DIR "/home/TU_USUARIO/libs_utils/eigen")

# O usar la instalación del sistema
# set(EIGEN3_INCLUDE_DIR "/usr/include/eigen3")
```

### 3. Compilar

```bash
# Cerrar y reabrir terminal (para cargar variables de entorno)
cd ~/open_haptic

# Compilar
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j4
```

### 4. Configurar Hardware

```bash
# Conectar dispositivos USB

# Configurar permisos (una vez)
sudo cp 99-haptic-devices.rules /etc/udev/rules.d/
sudo udevadm control --reload-rules
sudo udevadm trigger

# Desconectar y reconectar USB

# Configurar latencia (cada vez, opcional)
sudo ./setup_usb_low_latency.sh
```

### 5. Ejecutar

```bash
./build/test_arm_control 25 0 10 0.9
```

---

## 📋 Checklist de Portabilidad

### ✅ Archivos Incluidos en el Paquete

- [x] Código fuente (`src/CNL_L6_act_2.cpp`)
- [x] CMakeLists.txt
- [x] Scripts de instalación y configuración
- [x] Reglas udev
- [x] Documentación completa
- [x] Scripts de análisis (Python)
- [x] .gitignore configurado

### ❌ Archivos NO Incluidos (se regeneran)

- [ ] `build/` - Se compila en cada PC
- [ ] `.venv/` - Virtual env de Python se recrea
- [ ] `data_log.txt` - Logs de ejecución
- [ ] `plots/*.png` - Gráficas generadas
- [ ] `.vscode/` - Configuración local del IDE

### 🔧 Dependencias a Instalar en PC Nueva

1. **Sistema**: Ubuntu 20.04/22.04
2. **Compilador**: GCC >= 9.0, CMake >= 3.16
3. **Librerías**:
   - Eigen 3
   - Dynamixel SDK
   - OpenHaptics SDK 3.4

**Todo esto se instala con**: `sudo ./install_dependencies.sh`

---

## 🚀 Script Automático de Empaquetado

Crear archivo `create_portable_package.sh`:

```bash
#!/bin/bash
# Script para crear paquete portable del proyecto

PACKAGE_NAME="open_haptic_portable_$(date +%Y%m%d)"

echo "Creando paquete portable: $PACKAGE_NAME.tar.gz"

# Crear tarball excluyendo archivos innecesarios
tar --exclude-from=.gitignore \
    --exclude='.git' \
    --exclude='build' \
    --exclude='.venv' \
    --exclude='__pycache__' \
    --exclude='*.log' \
    --exclude='data_log*.txt' \
    --exclude='plots/*.png' \
    --exclude='plots/*.pdf' \
    --exclude='.vscode' \
    -czf "$PACKAGE_NAME.tar.gz" \
    .

# Mostrar información
echo ""
echo "✓ Paquete creado: $PACKAGE_NAME.tar.gz"
echo "Tamaño: $(du -h "$PACKAGE_NAME.tar.gz" | cut -f1)"
echo ""
echo "Para transferir a otra PC:"
echo "1. Copiar archivo .tar.gz"
echo "2. Extraer: tar -xzf $PACKAGE_NAME.tar.gz"
echo "3. Instalar dependencias: sudo ./install_dependencies.sh"
echo "4. Compilar: cmake -B build && cmake --build build -j4"
```

---

## 🔍 Verificar Paquete

Antes de transferir, verificar que el paquete es portable:

```bash
# Ver contenido del tarball
tar -tzf open_haptic_source.tar.gz | head -20

# Verificar tamaño (debería ser ~50-100 KB)
ls -lh open_haptic_source.tar.gz

# Probar extracción en directorio temporal
mkdir /tmp/test_extract
tar -xzf open_haptic_source.tar.gz -C /tmp/test_extract
ls -la /tmp/test_extract/open_haptic
```

---

## 💡 Consejos

### Para Transferencia Rápida
- **USB**: Copiar .tar.gz a pendrive
- **Red Local**: `scp open_haptic_source.tar.gz usuario@pc-nueva:~`
- **Nube**: Subir a Drive/Dropbox temporalmente

### Diferencias Entre PCs
- **Rutas de Eigen**: Ajustar en CMakeLists.txt
- **Puertos USB**: Pueden ser diferentes (/dev/ttyUSB0 vs /dev/ttyUSB1)
- **Variables de entorno**: El script install_dependencies.sh las configura

### Archivos Específicos de PC
Estos archivos pueden variar entre PCs:
- `data_log.txt` - Logs de experimentos
- `plots/*.png` - Gráficas específicas
- `.vscode/` - Configuración personal del editor

---

## ✅ Resumen Rápido

### En PC Original
```bash
cd ~/open_haptic
tar --exclude-from=.gitignore --exclude='.git' --exclude='build' \
    -czf ~/Desktop/open_haptic_source.tar.gz .
```

### En PC Nueva
```bash
tar -xzf open_haptic_source.tar.gz
cd open_haptic
sudo ./install_dependencies.sh
# Cerrar y reabrir terminal
cmake -B build && cmake --build build -j4
sudo cp 99-haptic-devices.rules /etc/udev/rules.d/
sudo udevadm control --reload-rules
```

**¡Listo para usar!** 🎉

---

**Fecha**: Octubre 2025
