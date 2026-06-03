#ifndef UI_ARBOL_HPP
#define UI_ARBOL_HPP

#include "raylib.h"
#include "../include/arbol.hpp"

class UIArbol {
private:
    Arbol* arbol;
    const char* pistaActiva;
    Rectangle area;
    Vector2 scroll;

    void dibujarNodo(NodoArbol* nodo, float x, float y, float dx, float dy, int nivel);
    int contarNiveles(NodoArbol* nodo);

public:
    UIArbol(Arbol* a, Rectangle area);
    void dibujar();
    void setNodoActivo(const char* pista);
    void limpiarNodoActivo() { pistaActiva = nullptr; }
    void actualizarScroll(Vector2 delta) { scroll.x += delta.x; scroll.y += delta.y; }
    void resetearScroll() { scroll = { 0, 0 }; }
};

#endif
