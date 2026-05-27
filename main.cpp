#include <iostream>
#include <unordered_map>
#include <string>
#include <sstream>

using namespace std;

class PercliShell {
private:
    unordered_map<string, unordered_map<string, string>> base_datos_comandos;
    string contexto_actual;
    bool ejecutandose;
    string dir = "walpaper";

public:
    PercliShell() {
        contexto_actual = "global";
        ejecutandose = true;
        cargarComandosPorDefecto();
    }

    void cargarComandosPorDefecto() {
        // Módulo Wallpaper (Los dejamos genéricos para inyectarles 'dir' al vuelo)
        base_datos_comandos["wallpaper"]["set"] = "$HOME/Percli/script/cambiar_todo.sh $HOME/";
        base_datos_comandos["wallpaper"]["opcion"] = "eza --icons --group-directories-first $HOME/";
        base_datos_comandos["wallpaper"]["clear"] = "awww kill";
        base_datos_comandos["wallpaper"]["random"] = "$HOME/Percli/script/cambiar_todo.sh"; // Quitamos la palabra estática "dir"
        base_datos_comandos["wallpaper"]["daemon"] = "setsid awww-daemon > /dev/null 2>&1 &";
        base_datos_comandos["wallpaper"]["view"] = "chafa $HOME/walpaper/";
        base_datos_comandos["wallpaper"]["setdir"] = ""; // Solo se registra para pasar el filtro del .find()
        base_datos_comandos["wallpaper"]["help"] = "Comandos de Wallpaper:\n  set [img]     -> Cambia el fondo\n  opcion        -> Lista wallpapers\n  setdir [path] -> Cambia la carpeta de wallpapers\n  clear         -> Mata el daemon\n  random        -> Fondo aleatorio\n  daemon        -> Inicia awww en segundo plano\n  view          -> Previsualiza con chafa";

        // Módulo Waybar
        base_datos_comandos["waybar"]["toggle"] = "pkill -USR1 waybar";
        base_datos_comandos["waybar"]["reload"] = "pkill waybar && setsid waybar -c ~/.config/waybar/top.json > /dev/null 2>&1 &";
        base_datos_comandos["waybar"]["open"] = "setsid waybar -c ~/.config/waybar/top.json > /dev/null 2>&1 &";
        base_datos_comandos["waybar"]["kill"] = "pkill waybar";
        base_datos_comandos["waybar"]["conf_top"] = "micro ~/.config/waybar/top.json";
        base_datos_comandos["waybar"]["conf_bottom"] = "micro ~/.config/waybar/bottom.json";
        base_datos_comandos["waybar"]["help"] = "Comandos de Waybar:\n  toggle      -> Oculta/Muestra la barra\n  reload      -> Reinicia la barra superior\n  open        -> Lanza top.json\n  kill        -> Mata el proceso waybar\n  conf_top    -> Edita barra de arriba\n  conf_bottom -> Edita barra de abajo";
        
        // Módulo Hyprland
        base_datos_comandos["hypr"]["general"] = "micro ~/.config/hypr/general.conf";
        base_datos_comandos["hypr"]["animation"] = "micro ~/.config/hypr/animation.conf";
        base_datos_comandos["hypr"]["bind"] = "micro ~/.config/hypr/bind.conf";
        base_datos_comandos["hypr"]["windows"] = "micro ~/.config/hypr/windowsrules.conf";
        base_datos_comandos["hypr"]["reload"] = "hyprctl reload";
        base_datos_comandos["hypr"]["conf"] = "micro ~/.config/hypr/hyprland.conf";
        base_datos_comandos["hypr"]["help"] = "Comandos de Hyprland:\n  general   -> Configuración base\n  animation -> Curvas y velocidades\n  bind      -> Atajos de teclado fijos\n  windows   -> Reglas de ventanas (flotación/blur)\n  reload    -> Aplica cambios con hyprctl\n  conf      -> Archivo hyprland.conf central";
    }

    void iniciar() {
        cout << "================= PERCLI ==================" << endl;
        cout << "Terminal de interaccion." << endl;
        cout << "===========================================" << endl;

        while (ejecutandose) {
            cout << "Percli [" << contexto_actual << "] > ";
            string linea;
            getline(cin, linea);
            if (linea.empty()) continue;

            procesarEntrada(linea);
        }
    }

private:
    void procesarEntrada(const string& linea) {
        stringstream ss(linea);
        string accion, argumento;
        ss >> accion;
        ss >> argumento;

        if (accion == "exit") {
            manejadorSalida();
            return;
        }

        if (contexto_actual == "global" && accion == "help") {
            cout << "\n--- MÓDULOS DISPONIBLES EN PERCLI ---" << endl;
            cout << "  wallpaper -> Ajustes de fondos de pantalla y daemons" << endl;
            cout << "  waybar    -> Control de barras superior e inferior" << endl;
            cout << "  hypr      -> Configuración modular del compositor" << endl;
            cout << "  exit      -> Cerrar Percli" << endl;
            cout << "-------------------------------------\n" << endl;
            return;
        }

        if (contexto_actual == "global") {
            if (base_datos_comandos.find(accion) != base_datos_comandos.end()) {
                contexto_actual = accion;
                cout << "Cambiaste de modulo: [" << accion << "]" << endl;
            } else {
                cout << "Los modulos disponibles son: wallpaper, waybar y hypr" << endl;
            }
        } 
        else {
            auto& modulo = base_datos_comandos[contexto_actual];
            if (modulo.find(accion) != modulo.end()) {
                if (accion == "help") {
                    cout << "\n-------------------------------------------" << endl;
                    cout << modulo["help"] << endl;
                    cout << "  Directorio actual: $HOME/" << dir << endl;
                    cout << "  exit      -> Regresar al menú global" << endl;
                    cout << "-------------------------------------------\n" << endl;
                    return;
                }

                if (accion == "setdir" && contexto_actual == "wallpaper") {
                    if (!argumento.empty()) {
                        dir = argumento;
                        cout << "[+] Directorio de wallpapers cambiado a: $HOME/" << dir << endl;
                    } else {
                        cout << "[!] Error: Especifica un nombre de carpeta. Ej: setdir mis_fondos" << endl;
                    }
                    return;
                }

                string comando_final = modulo[accion];
        
                // --- INYECCIÓN EN CALIENTE DE LA VARIABLE DIR ---
                if (contexto_actual == "wallpaper") {
                    if (accion == "set" || accion == "opcion") {
                        comando_final += dir;
                    }
                    else if (accion == "random") {
                        comando_final += " " + dir; 
                    }
                }

                // --- MANEJO INTELIGENTE DE ARGUMENTOS ---
                if (!argumento.empty() && accion != "set") {
                    comando_final += " " + argumento;
                } 
                else if (!argumento.empty() && accion == "set") {
                    comando_final += "/" + argumento; // Cierra la ruta para mandar el archivo: carpeta/imagen.jpg
                }
        
                system(comando_final.c_str());
            } 
            else {
                cout << "El comando '" << accion << "' no existe en este modulo. escribe help para ver los comandos disponibles" << endl;
            }
        }
    }

    void manejadorSalida() {
        if (contexto_actual != "global") {
            contexto_actual = "global";
        } else {
            cout << "Saliendo de Percli" << endl;
            ejecutandose = false;
        }
    }
};

int main() {

    PercliShell miTerminal;
    miTerminal.iniciar();
    
    return 0;
}