#include "../include/grafo.hpp"
#include <cstring>
#include <cstdio>

Grafo::Grafo(int capacidad) {
    this->capacidad = capacidad;
    this->numNodos = 0;
    this->nodos = new NodoGrafo[capacidad];
    this->dirigido = false;
    this->coordsDefinidas = false;
    for (int i = 0; i < capacidad; i++) {
        nodos[i].aristas = nullptr;
        nodos[i].nombre[0] = '\0';
        nodos[i].x = 0.0f;
        nodos[i].y = 0.0f;
    }
}

Grafo::~Grafo() {
    for (int i = 0; i < numNodos; i++) {
        Arista* actual = nodos[i].aristas;
        while (actual != nullptr) {
            Arista* siguiente = actual->siguiente;
            delete actual;
            actual = siguiente;
        }
        nodos[i].aristas = nullptr;
    }
    delete[] nodos;
}

int Grafo::agregarNodo(const char* nombre) {
    if (numNodos >= capacidad) {
        printf("Error: grafo lleno\n");
        return -1;
    }
    std::strncpy(nodos[numNodos].nombre, nombre, 49);
    nodos[numNodos].nombre[49] = '\0';
    nodos[numNodos].aristas = nullptr;
    nodos[numNodos].x = 0.0f;
    nodos[numNodos].y = 0.0f;
    numNodos++;
    return numNodos - 1;
}

void Grafo::agregarArista(int origen, int destino, int peso) {
    if (origen < 0 || origen >= numNodos || destino < 0 || destino >= numNodos) {
        printf("Error: indice de nodo invalido\n");
        return;
    }
    Arista* nueva = new Arista;
    nueva->destino = destino;
    nueva->peso = peso;
    nueva->siguiente = nodos[origen].aristas;
    nodos[origen].aristas = nueva;
}

int Grafo::buscarNodo(const char* nombre) const {
    for (int i = 0; i < numNodos; i++) {
        if (std::strcmp(nodos[i].nombre, nombre) == 0) {
            return i;
        }
    }
    return -1;
}

const char* Grafo::getNombreNodo(int indice) const {
    if (indice < 0 || indice >= numNodos) {
        return nullptr;
    }
    return nodos[indice].nombre;
}

Arista* Grafo::getAristas(int nodo) const {
    if (nodo < 0 || nodo >= numNodos) {
        return nullptr;
    }
    return nodos[nodo].aristas;
}

int Grafo::getNumNodos() const {
    return numNodos;
}

void Grafo::setCoordenada(int indice, float x, float y) {
    if (indice >= 0 && indice < numNodos) {
        nodos[indice].x = x;
        nodos[indice].y = y;
    }
}

float Grafo::getCoordX(int indice) const {
    if (indice < 0 || indice >= numNodos) return 0.0f;
    return nodos[indice].x;
}

float Grafo::getCoordY(int indice) const {
    if (indice < 0 || indice >= numNodos) return 0.0f;
    return nodos[indice].y;
}

bool Grafo::tieneCoordenadas() const {
    return coordsDefinidas;
}

void Grafo::marcarCoordenadas(bool val) {
    coordsDefinidas = val;
}
