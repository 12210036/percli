# Percli

Una terminal de interacción modular e interactiva escrita en C++ para gestionar configuraciones, daemons de fondos de pantalla (`awww`), barras de estado (`Waybar`) y el compositor `Hyprland`.

## Características
- **Menú contextual:** Navegación dinámica entre módulos (`global`, `wallpaper`, `waybar`, `hypr`).
- **Comando Help integrado:** Sistema de ayuda directamente mapeado en memoria usando `std::unordered_map`.
- **Ejecución nativa:** Interacción fluida con comandos del sistema mediante `std::system`.
