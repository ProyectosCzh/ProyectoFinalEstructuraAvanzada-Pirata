#include "mapa_manager.hpp"
#include <cstdio>
#include <cstring>
#include <cctype>
#include <dirent.h>

MapaManager::MapaManager() {
    totalMapas = 0;
    actual = 0;
}

void MapaManager::derivarNombre(const char* dirName, char* out) {
    int i = 0;
    bool capitalizar = true;
    for (int j = 0; dirName[j] != '\0' && i < 58; j++) {
        if (dirName[j] == '_') {
            out[i++] = ' ';
            capitalizar = true;
        } else {
            out[i++] = capitalizar ? std::toupper(dirName[j]) : dirName[j];
            capitalizar = false;
        }
    }
    out[i] = '\0';
}

bool MapaManager::escanearMapas() {
    DIR* dir = opendir("maps");
    if (dir == nullptr) {
        printf("Error: no se pudo abrir el directorio 'maps/'\n");
        return false;
    }

    totalMapas = 0;
    struct dirent* entrada;
    char ruta[128];

    while ((entrada = readdir(dir)) != nullptr && totalMapas < 20) {
        if (entrada->d_name[0] == '.') continue;

        // Verificar que exista grafo.txt dentro (valida que sea un directorio con mapa)
        std::sprintf(ruta, "maps/%s/grafo.txt", entrada->d_name);
        FILE* f = std::fopen(ruta, "r");
        if (f == nullptr) continue;
        std::fclose(f);

        std::strncpy(mapas[totalMapas].dirName, entrada->d_name, 29);
        mapas[totalMapas].dirName[29] = '\0';
        derivarNombre(entrada->d_name, mapas[totalMapas].nombre);
        totalMapas++;
    }

    closedir(dir);
    printf("Mapas encontrados: %d\n", totalMapas);
    for (int i = 0; i < totalMapas; i++) {
        printf("  [%d] %s\n", i, mapas[i].nombre);
    }
    return totalMapas > 0;
}

bool MapaManager::setActual(int idx) {
    if (idx < 0 || idx >= totalMapas) return false;
    actual = idx;
    return true;
}

const InfoMapa& MapaManager::getMapa(int idx) const {
    return mapas[idx];
}

const InfoMapa& MapaManager::getActualMapa() const {
    return mapas[actual];
}

void MapaManager::getRutaBase(char* out) const {
    std::sprintf(out, "maps/%s/", mapas[actual].dirName);
}

void MapaManager::getRutaGrafo(char* out) const {
    std::sprintf(out, "maps/%s/grafo.txt", mapas[actual].dirName);
}

void MapaManager::getRutaPistas(char* out) const {
    std::sprintf(out, "maps/%s/pistas.txt", mapas[actual].dirName);
}

void MapaManager::getRutaCoords(char* out) const {
    std::sprintf(out, "maps/%s/coords.txt", mapas[actual].dirName);
}

void MapaManager::getRutaFondo(char* out) const {
    std::sprintf(out, "maps/%s/fondo.jpg", mapas[actual].dirName);
}

void MapaManager::getRutaResultado(char* out) const {
    std::sprintf(out, "maps/%s/resultado.txt", mapas[actual].dirName);
}
