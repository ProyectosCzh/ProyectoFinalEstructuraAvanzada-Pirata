#include "../include/arbol.hpp"
#include <cstring>
#include <cstdio>

Arbol::Arbol() {
    raiz = nullptr;
}

Arbol::~Arbol() {
    if (raiz != nullptr) {
        limpiarRecursivo(raiz);
    }
}

void Arbol::limpiarRecursivo(NodoArbol* nodo) {
    if (nodo == nullptr) return;
    for (int i = 0; i < nodo->numHijos; i++) {
        limpiarRecursivo(nodo->hijos[i]);
    }
    delete[] nodo->hijos;
    delete nodo;
}

void Arbol::crearRaiz(const char* pista, const char* destino) {
    if (raiz != nullptr) {
        limpiarRecursivo(raiz);
    }
    raiz = new NodoArbol;
    std::strncpy(raiz->pista, pista, 199);
    raiz->pista[199] = '\0';
    std::strncpy(raiz->destino, destino, 49);
    raiz->destino[49] = '\0';
    raiz->hijos = nullptr;
    raiz->numHijos = 0;
    raiz->padre = nullptr;
}

NodoArbol* Arbol::agregarHijo(NodoArbol* padreNodo, const char* pista, const char* destino) {
    if (padreNodo == nullptr) return nullptr;

    NodoArbol** nuevosHijos = new NodoArbol*[padreNodo->numHijos + 1];
    for (int i = 0; i < padreNodo->numHijos; i++) {
        nuevosHijos[i] = padreNodo->hijos[i];
    }
    delete[] padreNodo->hijos;
    padreNodo->hijos = nuevosHijos;

    NodoArbol* hijo = new NodoArbol;
    std::strncpy(hijo->pista, pista, 199);
    hijo->pista[199] = '\0';
    std::strncpy(hijo->destino, destino, 49);
    hijo->destino[49] = '\0';
    hijo->hijos = nullptr;
    hijo->numHijos = 0;
    hijo->padre = padreNodo;

    padreNodo->hijos[padreNodo->numHijos] = hijo;
    padreNodo->numHijos++;
    return hijo;
}

NodoArbol* Arbol::buscarNodoRecursivo(NodoArbol* nodo, const char* pista) const {
    if (nodo == nullptr) return nullptr;
    if (std::strcmp(nodo->pista, pista) == 0) {
        return nodo;
    }
    for (int i = 0; i < nodo->numHijos; i++) {
        NodoArbol* encontrado = buscarNodoRecursivo(nodo->hijos[i], pista);
        if (encontrado != nullptr) {
            return encontrado;
        }
    }
    return nullptr;
}

NodoArbol* Arbol::getRaiz() const {
    return raiz;
}

const char* Arbol::interpretar(const char* pista) const {
    NodoArbol* nodo = buscarNodoRecursivo(raiz, pista);
    if (nodo != nullptr) {
        return nodo->destino;
    }
    return nullptr;
}


