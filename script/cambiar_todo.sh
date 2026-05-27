# cambiar_todo.sh

#!/bin/bash

# Capturamos el argumento que manda C++
ARGUMENTO_1="$1"

# 1. Identificar si nos pasaron un archivo real o una carpeta
if [ -f "$ARGUMENTO_1" ]; then
    # Caso 'set': Nos pasaron el archivo directo
    WALL="$ARGUMENTO_1"
else
    # Caso 'random' o carpeta personalizada:
    if [ -n "$ARGUMENTO_1" ] && [ -d "$HOME/$ARGUMENTO_1" ]; then
        WALPAPER_DIR="$HOME/$ARGUMENTO_1"
    else
        WALPAPER_DIR="$HOME/walpaper" # Carpeta de fábrica por si acaso
    fi
    
    # Buscamos el archivo aleatorio DENTRO de la carpeta que eligió Percli (incluye gifs)
    WALL=$(find "$WALPAPER_DIR" -type f \( -name "*.gif" -o -name "*.jpg" -o -name "*.png" -o -name "*.jpeg" \) 2>/dev/null | shuf -n 1)
fi

# Validar que encontramos un wallpaper antes de continuar
if [ -z "$WALL" ]; then
    echo "[!] No se encontró ningún wallpaper válido en la ruta actual."
    exit 1
fi # <--- ¡ESTE FI HACÍA FALTA PARA CERRAR EL LLAVERO DE SEGURIDAD!

# Reproducir audio de transición en segundo plano de manera segura
paplay "$HOME/Downloads/wave-defeated.mp3" 2>/dev/null &

# 2. Aplicar Wallpaper y Generar Colores con Pywal
awww img "$WALL" --transition-type wave --transition-duration 2 --transition-angle $(( ( RANDOM % 360) + 1 ))
wal -i "$WALL" -q
ln -sf "$WALL" "$HOME/.cache/wal/target_wallpaper"

# 3. Refrescar Componentes de la UI
# Centro de notificaciones
swaync-client -rs 2>/dev/null || (swaync &)

# Recarga de Kitty (con remote control activado para cambiar colores dinámicos)
kitty @ --allow-remote-control set-colors -a -c "$HOME/.cache/wal/colors-kitty.conf" 2>/dev/null

# Recarga de Waybar (señal ligera para recargar CSS)
killall -SIGUSR2 waybar 2>/dev/null

# RECARGA DE EWW (Aplica tus variables de Pywal al dock/widgets)
eww reload 2>/dev/null

# Si notas que tus widgets de EWW se quedan trabados o bugeados con el color viejo, 
# puedes borrar el '#' de abajo para forzar el reinicio duro del panel:
# killall eww && setsid eww open panel_v >/dev/null 2>&1 &