#include "../include/pila.hpp"
#include <cstdio>

Pila::Pila(int capacidad) {
    this->capacidad = capacidad;
    this->datos = new int[capacidad];
    this->tope = -1;
}

Pila::~Pila() {
    delete[] datos;
}

void Pila::push(int valor) {
    if (tope >= capacidad - 1) {
        printf("Error: pila llena\n");
        return;
    }
    tope++;
    datos[tope] = valor;
}

int Pila::pop() {
    if (estaVacia()) {
        printf("Error: pila vacia\n");
        return -1;
    }
    int valor = datos[tope];
    tope--;
    return valor;
}

bool Pila::estaVacia() const {
    return tope == -1;
}

int Pila::obtenerTam() const {
    return tope + 1;
}

void Pila::vaciar() {
    tope = -1;
}
