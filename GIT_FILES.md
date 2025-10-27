# Gestión de Archivos - Git

## 📋 ¿Qué se Versiona en Git?

### ✅ Archivos Incluidos (Código Fuente)

#### Código
- `src/CNL_L6_act_2.cpp` - Control principal
- `CMakeLists.txt` - Configuración de compilación

#### Documentación
- `README.md` - Documentación principal
- `QUICKSTART.md` - Guía de inicio rápido
- `TELEOP_USAGE.md` - Guía de uso
- `README_teleop_joint.md` - Control articular
- `PORTABILITY.md` - Guía de transferencia
- Todos los `CHANGELOG_*.md`

#### Scripts y Herramientas
- `install_dependencies.sh` - Instalador de dependencias
- `setup_usb_low_latency.sh` - Configuración USB
- `create_portable_package.sh` - Empaquetador
- `99-haptic-devices.rules` - Reglas udev
- `tools/plot_log.py` - Análisis de datos

#### Configuración
- `.gitignore` - Qué NO versionar
- `plots/.gitkeep` - Mantener carpeta vacía

---

## ❌ Archivos NO Versionados (Generados/Temporales)

### Build y Compilación
```
build/               # Directorio de compilación completo
CMakeFiles/          # Archivos generados por CMake
CMakeCache.txt
cmake_install.cmake
Makefile
*.o, *.so, *.a       # Binarios compilados
```

### Datos de Ejecución
```
data_log.txt         # Logs del brazo robótico
data_log_joint.txt   # Logs de control articular
analysis_summary.txt # Resúmenes de análisis
*.log                # Todos los logs
output.log
```

### Gráficas Generadas
```
plots/*.png          # Gráficas generadas por plot_log.py
plots/*.pdf
plots/*.jpg
plots/*.svg
```
**Nota**: La carpeta `plots/` existe, pero está vacía (solo `.gitkeep`)

### Python
```
.venv/               # Virtual environment
__pycache__/         # Cache de Python
*.pyc, *.pyo         # Bytecode compilado
.ipynb_checkpoints   # Jupyter notebooks
```

### IDEs y Editores
```
.vscode/             # Configuración de VSCode
.idea/               # JetBrains IDEs
*.swp, *~            # Vim/Emacs temporales
.DS_Store            # macOS
```

---

## 🔍 Verificar Estado de Git

### Ver qué se subirá
```bash
cd ~/open_haptic
git status
```

### Ver archivos ignorados
```bash
git status --ignored
```

### Ver qué está trackeado
```bash
git ls-files
```

### Probar gitignore
```bash
# Verificar si un archivo sería ignorado
git check-ignore -v build/test_arm_control
git check-ignore -v data_log.txt
```

---

## 📊 Tamaños Aproximados

### Repositorio Git (Solo código fuente)
```
Código fuente:     ~15 KB
Documentación:     ~50 KB
Scripts:           ~10 KB
Total:             ~75 KB
```

### Con Archivos Generados (NO versionados)
```
build/:            ~2-5 MB   (binarios compilados)
.venv/:            ~50 MB    (si usas Python venv)
data_log.txt:      Variable  (depende de experimentos)
plots/:            ~1-2 MB   (si generas muchas gráficas)
```

---

## 🚀 Comandos Útiles

### Limpiar archivos no versionados
```bash
# Ver qué se borraría (simulación)
git clean -nxd

# Borrar build/ y archivos ignorados (¡CUIDADO!)
git clean -fxd

# Borrar solo build/
rm -rf build/
```

### Actualizar gitignore retroactivamente
```bash
# Si agregaste archivos a .gitignore después de commitearlos
git rm -r --cached build/
git rm --cached data_log.txt
git commit -m "Actualizar .gitignore"
```

### Ver diferencias
```bash
# Ver cambios no commiteados
git diff

# Ver diferencias de archivos específicos
git diff CMakeLists.txt
```

---

## 📦 Exportar Sin Git

### Método 1: Tarball Solo Código (Portable)
```bash
# Usar el script automático
./create_portable_package.sh

# O manualmente
tar --exclude-from=.gitignore --exclude='.git' \
    -czf open_haptic_source.tar.gz .
```

### Método 2: Git Archive (Solo commiteados)
```bash
# Exportar solo archivos en el último commit
git archive -o open_haptic_git.tar.gz HEAD

# Ver contenido
tar -tzf open_haptic_git.tar.gz
```

### Método 3: Clonar Sin Historial
```bash
# En otra PC
git clone --depth 1 https://github.com/rvillafuerte-ut/open_haptic.git
```

---

## ✅ Buenas Prácticas

### Antes de Commit
```bash
# 1. Verificar qué cambiaste
git status

# 2. Ver diferencias
git diff

# 3. Agregar solo lo necesario
git add src/CNL_L6_act_2.cpp
git add README.md

# O agregar todo (gitignore protege)
git add .

# 4. Commit con mensaje descriptivo
git commit -m "Descripción clara de los cambios"

# 5. Push
git push origin main
```

### Nunca Commitear
- ❌ Binarios compilados (build/)
- ❌ Logs de experimentos (data_log.txt)
- ❌ Virtual environments (.venv/)
- ❌ Archivos temporales del IDE
- ❌ Datos sensibles o configuraciones locales

### Siempre Commitear
- ✅ Código fuente (.cpp, .h)
- ✅ Configuración de build (CMakeLists.txt)
- ✅ Scripts útiles (.sh, .py)
- ✅ Documentación (.md)
- ✅ Configuración de proyecto (.rules)

---

## 🔧 Solución de Problemas

### "Accidentalmente commiteé build/"
```bash
# Remover de git pero mantener en disco
git rm -r --cached build/
git commit -m "Remover build/ del repo"
git push
```

### "Git dice que hay cambios pero no veo nada"
```bash
# Probablemente son archivos ignorados
git status --ignored

# Limpiar
git clean -fd
```

### "Quiero resetear todo al último commit"
```bash
# ¡CUIDADO! Esto borra cambios no commiteados
git reset --hard HEAD
```

---

## 📝 Resumen

- **Git versiona**: Solo código fuente, docs y scripts (~75 KB)
- **Git ignora**: build/, logs, .venv, gráficas, etc.
- **Portable**: Usa `./create_portable_package.sh` para transferir
- **Seguro**: .gitignore actualizado protege archivos temporales

**Ahora puedes**:
1. ✅ Subir a GitHub sin archivos innecesarios
2. ✅ Comprimir y pasar a otra PC
3. ✅ Mantener repo limpio (~75 KB)
4. ✅ Regenerar build/ en cualquier PC

---

**Última actualización**: Octubre 2025
