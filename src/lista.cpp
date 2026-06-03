#include "../include/lista.hpp"
#include <cstdio>

Lista::Lista() {
    cabeza = nullptr;
    cola = nullptr;
    tam = 0;
}

Lista::~Lista() {
    vaciar();
}

void Lista::insertar(int dato) {
    NodoLista* nuevo = new NodoLista;
    nuevo->dato = dato;
    nuevo->anterior = nullptr;
    nuevo->siguiente = nullptr;

    if (cabeza == nullptr) {
        cabeza = nuevo;
        cola = nuevo;
    } else {
        cola->siguiente = nuevo;
        nuevo->anterior = cola;
        cola = nuevo;
    }
    tam++;
}

bool Lista::contiene(int dato) const {
    NodoLista* actual = cabeza;
    while (actual != nullptr) {
        if (actual->dato == dato) {
            return true;
        }
        actual = actual->siguiente;
    }
    return false;
}

int Lista::obtener(int indice) const {
    if (indice < 0 || indice >= tam) {
        printf("Error: indice %d fuera de rango (tam=%d)\n", indice, tam);
        return -1;
    }
    NodoLista* actual = cabeza;
    for (int i = 0; i < indice; i++) {
        actual = actual->siguiente;
    }
    return actual->dato;
}

int Lista::longitud() const {
    return tam;
}

void Lista::vaciar() {
    NodoLista* actual = cabeza;
    while (actual != nullptr) {
        NodoLista* siguiente = actual->siguiente;
        delete actual;
        actual = siguiente;
    }
    cabeza = nullptr;
    cola = nullptr;
    tam = 0;
}
