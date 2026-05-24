# cambiar_todo.sh

# /bin/bash

# 1. Gestión del Wallpaper
if [ -n "$1" ]; then
    WALL="$1"
else
    WALL=$(find ~/walpaper -type f | shuf -n 1)
fi
paplay Downloads/wave-defeated.mp3 &
# 2. Aplicar Wallpaper y Generar Colores1
awww img "$WALL" --transition-type wave --transition-duration 2  --transition-angle $(( ( RANDOM % 360) + 1 ))
wal -i "$WALL" -q
ln -sf "$WALL" ~/.cache/wal/target_wallpaper

# 5. Refrescar Componentes de la UI
# ... (tu código anterior igual)

# 5. Refrescar Componentes de la UI
swaync-client -rs 2>/dev/null || (swaync &)
# Recarga de Kitty
kitty @ --allow-remote-control set-colors -a -c ~/.cache/wal/colors-kitty.conf 2>/dev/null
# Recarga de Waybar
killall -SIGUSR2 waybar 2>/dev/null

# RECARGA DE EWW (Forzada para aplicar colores nuevos)
 eww r
# Si el proceso se queda trabado, descomenta las siguientes 2 líneas:
# killall eww
# eww open panel_v
