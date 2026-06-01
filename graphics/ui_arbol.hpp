#ifndef UI_ARBOL_HPP
#define UI_ARBOL_HPP

#include "raylib.h"
#include "../include/arbol.hpp"

class UIArbol {
private:
    Arbol* arbol;
    const char* pistaActiva;
    Rectangle area;

    void dibujarNodo(NodoArbol* nodo, float x, float y, float dx, float dy, int nivel, int maxNivel);
    int contarNiveles(NodoArbol* nodo);

public:
    UIArbol(Arbol* a, Rectangle area);
    void dibujar();
    void setNodoActivo(const char* pista);
};

#endif
