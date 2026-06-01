#include "../include/grafo.hpp"
#include <cstring>
#include <cstdio>

Grafo::Grafo(int capacidad) {
    this->capacidad = capacidad;
    this->numNodos = 0;
    this->nodos = new NodoGrafo[capacidad];
    for (int i = 0; i < capacidad; i++) {
        nodos[i].aristas = nullptr;
        nodos[i].nombre[0] = '\0';
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

void Grafo::limpiar() {
    for (int i = 0; i < numNodos; i++) {
        Arista* actual = nodos[i].aristas;
        while (actual != nullptr) {
            Arista* siguiente = actual->siguiente;
            delete actual;
            actual = siguiente;
        }
        nodos[i].aristas = nullptr;
        nodos[i].nombre[0] = '\0';
    }
    numNodos = 0;
}
