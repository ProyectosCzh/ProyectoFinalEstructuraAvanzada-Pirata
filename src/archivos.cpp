#include "../include/archivos.hpp"
#include <cstdio>
#include <cstring>
#include <cstdlib>

bool cargarGrafo(const char* ruta, Grafo& grafo) {
    FILE* archivo = std::fopen(ruta, "r");
    if (archivo == nullptr) {
        printf("Error: no se pudo abrir %s\n", ruta);
        return false;
    }

    char linea[512];
    int numLinea = 0;

    while (std::fgets(linea, sizeof(linea), archivo) != nullptr) {
        if (linea[0] == '\n' || linea[0] == '\0' || linea[0] == '#') {
            continue;
        }

        size_t len = std::strlen(linea);
        if (len > 0 && linea[len - 1] == '\n') {
            linea[len - 1] = '\0';
        }

        if (numLinea == 0) {
            if (std::strncmp(linea, "nodos:", 6) == 0) {
                char* token = std::strtok(linea + 6, " ,");
                while (token != nullptr) {
                    grafo.agregarNodo(token);
                    token = std::strtok(nullptr, " ,");
                }
            }
        } else {
            char* origenStr = std::strtok(linea, ":");
            if (origenStr == nullptr) continue;

            int origen = grafo.buscarNodo(origenStr);
            if (origen < 0) {
                printf("Warning: nodo '%s' no encontrado\n", origenStr);
                continue;
            }

            char* destinos = std::strtok(nullptr, ":");
            if (destinos == nullptr) continue;

            char* token = std::strtok(destinos, ",");
            while (token != nullptr) {
                char destinoStr[50];
                int peso = 0;
                if (std::sscanf(token, " %49[^(](%d)", destinoStr, &peso) >= 2) {
                    int destino = grafo.buscarNodo(destinoStr);
                    if (destino >= 0) {
                        grafo.agregarArista(origen, destino, peso);
                    }
                }
                token = std::strtok(nullptr, ",");
            }
        }
        numLinea++;
    }

    std::fclose(archivo);
    printf("Grafo cargado: %d nodos desde %s\n", grafo.getNumNodos(), ruta);
    return true;
}

bool cargarPistas(const char* ruta, Diccionario& dicc) {
    FILE* archivo = std::fopen(ruta, "r");
    if (archivo == nullptr) {
        printf("Error: no se pudo abrir %s\n", ruta);
        return false;
    }

    char linea[512];
    while (std::fgets(linea, sizeof(linea), archivo) != nullptr) {
        if (linea[0] == '\n' || linea[0] == '\0' || linea[0] == '#') {
            continue;
        }

        size_t len = std::strlen(linea);
        if (len > 0 && linea[len - 1] == '\n') {
            linea[len - 1] = '\0';
        }

        char* clave = std::strtok(linea, ":");
        if (clave == nullptr) continue;

        char* valor = std::strtok(nullptr, ":");
        if (valor == nullptr) continue;

        dicc.insertar(clave, valor);
    }

    std::fclose(archivo);
    printf("Pistas cargadas desde %s\n", ruta);
    return true;
}

bool cargarArbolPistas(const char* ruta, Arbol& arbol) {
    FILE* archivo = std::fopen(ruta, "r");
    if (archivo == nullptr) {
        printf("Error: no se pudo abrir %s\n", ruta);
        return false;
    }

    char linea[512];
    bool primera = true;

    while (std::fgets(linea, sizeof(linea), archivo) != nullptr) {
        if (linea[0] == '\n' || linea[0] == '\0' || linea[0] == '#') {
            continue;
        }

        size_t len = std::strlen(linea);
        if (len > 0 && linea[len - 1] == '\n') {
            linea[len - 1] = '\0';
        }

        char* clave = std::strtok(linea, ":");
        if (clave == nullptr) continue;

        char* valor = std::strtok(nullptr, ":");
        if (valor == nullptr) continue;

        if (primera) {
            arbol.crearRaiz("inicio", clave);
            arbol.agregarHijo(arbol.getRaiz(), valor, clave);
            primera = false;
        } else {
            NodoArbol* nodoRaiz = arbol.getRaiz();
            for (int i = 0; i < nodoRaiz->numHijos; i++) {
                if (std::strcmp(nodoRaiz->hijos[i]->destino, clave) == 0) {
                    arbol.agregarHijo(nodoRaiz->hijos[i], valor, clave);
                    break;
                }
            }
        }
    }

    std::fclose(archivo);
    printf("Arbol de pistas cargado desde %s\n", ruta);
    return true;
}

bool guardarResultado(const char* ruta, const Lista& camino, const Grafo& grafo, const char* algoritmo, int costoTotal, int visitados) {
    FILE* archivo = std::fopen(ruta, "w");
    if (archivo == nullptr) {
        printf("Error: no se pudo crear %s\n", ruta);
        return false;
    }

    std::fprintf(archivo, "RUTA AL TESORO\n");
    std::fprintf(archivo, "==============\n\n");

    if (camino.longitud() > 0) {
        int inicio = camino.obtener(0);
        const char* nombreInicio = grafo.getNombreNodo(inicio);
        std::fprintf(archivo, "Inicio: %s\n", nombreInicio ? nombreInicio : "?");
    }

    std::fprintf(archivo, "Ruta: ");
    for (int i = 0; i < camino.longitud(); i++) {
        int idx = camino.obtener(i);
        const char* nombre = grafo.getNombreNodo(idx);
        std::fprintf(archivo, "%s", nombre ? nombre : "?");
        if (i < camino.longitud() - 1) {
            std::fprintf(archivo, " -> ");
        }
    }
    std::fprintf(archivo, "\n");

    std::fprintf(archivo, "Costo total: %d\n", costoTotal);
    std::fprintf(archivo, "Visitados: %d nodos\n", visitados);
    std::fprintf(archivo, "Algoritmo usado: %s\n", algoritmo);
    std::fprintf(archivo, "\n");

    std::fclose(archivo);
    printf("Resultado guardado en %s\n", ruta);
    return true;
}
