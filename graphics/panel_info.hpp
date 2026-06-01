#ifndef PANEL_INFO_HPP
#define PANEL_INFO_HPP

#include "raylib.h"
#include "../include/lista.hpp"
#include "../include/cola.hpp"
#include "../include/pila.hpp"

class PanelInfo {
private:
    Rectangle area;
    int fuenteTam;

public:
    PanelInfo(Rectangle area);
    void dibujar(const char* titulo, const char* pistaActual,
                 const char* nodoNombre, int costo,
                 const char* estadoStr, int visitados,
                 int pasoActual,
                 const Lista& ruta,
                 const Cola& cola,
                 const Pila& pila);
    void dibujarColaVisual(const Cola& cola, Rectangle areaCola);
    void dibujarPilaVisual(const Pila& pila, Rectangle areaPila);
};

#endif
