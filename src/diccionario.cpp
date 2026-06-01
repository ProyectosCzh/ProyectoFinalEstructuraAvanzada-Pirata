#include "../include/diccionario.hpp"
#include <cstring>
#include <cstdio>

Diccionario::Diccionario() {
    for (int i = 0; i < TAM_TABLA; i++) {
        tabla[i] = nullptr;
    }
}

Diccionario::~Diccionario() {
    for (int i = 0; i < TAM_TABLA; i++) {
        ParClaveValor* actual = tabla[i];
        while (actual != nullptr) {
            ParClaveValor* siguiente = actual->siguiente;
            delete actual;
            actual = siguiente;
        }
        tabla[i] = nullptr;
    }
}

int Diccionario::hash(const char* clave) const {
    int suma = 0;
    for (int i = 0; clave[i] != '\0'; i++) {
        suma += clave[i];
    }
    return suma % TAM_TABLA;
}

void Diccionario::insertar(const char* clave, const char* valor) {
    int indice = hash(clave);
    ParClaveValor* nuevo = new ParClaveValor;
    std::strncpy(nuevo->clave, clave, 49);
    nuevo->clave[49] = '\0';
    std::strncpy(nuevo->valor, valor, 199);
    nuevo->valor[199] = '\0';
    nuevo->siguiente = tabla[indice];
    tabla[indice] = nuevo;
}

const char* Diccionario::obtener(const char* clave) const {
    int indice = hash(clave);
    ParClaveValor* actual = tabla[indice];
    while (actual != nullptr) {
        if (std::strcmp(actual->clave, clave) == 0) {
            return actual->valor;
        }
        actual = actual->siguiente;
    }
    return nullptr;
}

bool Diccionario::contiene(const char* clave) const {
    int indice = hash(clave);
    ParClaveValor* actual = tabla[indice];
    while (actual != nullptr) {
        if (std::strcmp(actual->clave, clave) == 0) {
            return true;
        }
        actual = actual->siguiente;
    }
    return false;
}

void Diccionario::eliminar(const char* clave) {
    int indice = hash(clave);
    ParClaveValor* actual = tabla[indice];
    ParClaveValor* anterior = nullptr;
    while (actual != nullptr) {
        if (std::strcmp(actual->clave, clave) == 0) {
            if (anterior == nullptr) {
                tabla[indice] = actual->siguiente;
            } else {
                anterior->siguiente = actual->siguiente;
            }
            delete actual;
            return;
        }
        anterior = actual;
        actual = actual->siguiente;
    }
}
