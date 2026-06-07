#include "../include/cola.hpp"
#include <cstdio>

Cola::Cola(int capacidad) {
    this->capacidad = capacidad;
    this->datos = new int[capacidad];
    this->frente = 0;
    this->final = -1;
    this->tam = 0;
}

Cola::~Cola() {
    delete[] datos;
}

void Cola::encolar(int valor) {
    if (estaLlena()) {
        printf("Error: cola llena\n");
        return;
    }
    final = (final + 1) % capacidad;
    datos[final] = valor;
    tam++;
}

bool Cola::estaLlena() const {
    return tam == capacidad;
}

int Cola::obtenerTam() const {
    return tam;
}

int Cola::getDatoEn(int indice) const {
    if (indice < 0 || indice >= tam) {
        return -1;
    }
    return datos[(frente + indice) % capacidad];
}

void Cola::vaciar() {
    frente = 0;
    final = -1;
    tam = 0;
}
