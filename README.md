# Percli

Percli es una interfaz de línea de comandos diseñada para optimizar y automatizar la personalización de entornos Linux. Su objetivo principal es simplificar la gestión del sistema, permitiendo cambiar fondos de pantalla, recargar componentes de la interfaz de usuario o editar archivos de configuración mediante comandos cortos y directos, evitando la necesidad de introducir secuencias largas en la terminal.

## Plan de desarrollo (Roadmap)
* **Gestión avanzada de paneles:** Control y alternancia de barras de estado como Waybar y EWW de forma eficiente.
* **Manejo dinámico de wallpapers:** Administración de directorios variables, daemons de fondos (awww) y control de transiciones.
* **Sincronización de temas:** Automatización para que el esquema de colores generado por Pywal se aplique en armonía a componentes como Kitty, Rofi y el resto del sistema.
* **Control modular del Window Manager:** Modificación rápida de atajos de teclado, reglas de ventanas y animaciones en Hyprland.

---

## Arquitectura interna: Análisis del mapa de comandos

La organización de los comandos en Percli se divide entre la persistencia estática en memoria y el formateo dinámico en tiempo de ejecución (inyección en caliente). Gracias al uso de estructuras std::unordered_map, el acceso y la búsqueda de cualquier instrucción se realiza en tiempo constante.

### Tabla de comandos disponibles

| Módulo | Comando | Tipo de Estructura | Función / Comando del Sistema | Coste |
| :--- | :--- | :--- | :--- | :--- |
| **Global** | `help` | Estático | Muestra los módulos principales disponibles en la shell. | $O(1)$ |
| **Wallpaper** | `help` | Estático | Muestra la guía visual contextual del módulo de fondos. | $O(1)$ |
| | `setdir` | Dinámico (Memoria) | Actualiza el valor de la variable de la carpeta de trabajo (`dir`). | $O(1)$ |
| | `set` | Dinámico (Inyección) | Inyecta la ruta y ejecuta `./cambiar_todo.sh $HOME/[dir]/[img]` | $O(1)$ |
| | `opcion` | Dinámico (Inyección) | Lista los fondos usando `eza --icons ... $HOME/[dir]` | $O(1)$ |
| | `random` | Dinámico (Inyección) | Lanza un fondo aleatorio inyectando la carpeta: `./cambiar_todo.sh [dir]` | $O(1)$ |
| | `clear` | Estático (Directo) | Detiene el daemon de fondos ejecutando `awww kill`. | $O(1)$ |
| | `daemon` | Estático (Directo) | Inicializa el proceso `awww-daemon` en segundo plano. | $O(1)$ |
| | `view` | Estático (Directo) | Previsualiza el contenido de la carpeta fija de imágenes con `chafa`. | $O(1)$ |
| **Waybar** | `help` | Estático | Muestra los comandos de control para los paneles CSS. | $O(1)$ |
| | `toggle` | Estático (Directo) | Oculta o muestra la barra enviando la señal `pkill -USR1 waybar`. | $O(1)$ |
| | `reload` | Estático (Directo) | Reinicia Waybar cargando la configuración superior (`top.json`). | $O(1)$ |
| | `open` | Estático (Directo) | Lanza una nueva instancia de la barra superior en segundo plano. | $O(1)$ |
| | `kill` | Estático (Directo) | Termina el proceso activo de `waybar`. | $O(1)$ |
| | `conf_top` | Estático (Directo) | Abre el archivo de configuración superior en el editor `micro`. | $O(1)$ |
| | `conf_bottom` | Estático (Directo) | Abre el archivo de configuración inferior en el editor `micro`. | $O(1)$ |
| **Hyprland** | `help` | Estático | Muestra las opciones de edición modular del compositor gráfico. | $O(1)$ |
| | `general` | Estático (Directo) | Abre la configuración de layout, gaps y bordes (`general.conf`) con `micro`. | $O(1)$ |
| | `animation` | Estático (Directo) | Abre la configuración de curvas de animación (`animation.conf`). | $O(1)$ |
| | `bind` | Estático (Directo) | Abre el archivo central de atajos de teclado (`bind.conf`). | $O(1)$ |
| | `windows` | Estático (Directo) | Abre las reglas específicas para ventanas y difuminados (`windowsrules.conf`).| $O(1)$ |
| | `reload` | Estático (Directo) | Fuerza al compositor a actualizar los cambios con `hyprctl reload`. | $O(1)$ |
| | `conf` | Estático (Directo) | Abre el archivo de configuración maestro `hyprland.conf`. | $O(1)$ |

---
Desarrollado en C++ y Bash para sistemas Linux.