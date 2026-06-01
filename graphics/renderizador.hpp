#ifndef RENDERIZADOR_HPP
#define RENDERIZADOR_HPP

#include "raylib.h"
#include "../include/grafo.hpp"
#include "../include/lista.hpp"
#include "../core/juego.hpp"

struct NodoVisual {
    Vector2 posicion;
    Color color;
    float radio;
    bool seleccionado;
};

class Renderizador {
private:
    Grafo* grafo;
    NodoVisual* nodosVisuales;
    int numNodos;
    float anchoPanel;
    float altoPanel;

    void calcularLayout();
    void dibujarAristas();
    void dibujarNodos();
    void dibujarNombres();
    void dibujarPesos();
    void dibujarLeyenda();

public:
    Renderizador(Grafo* g, float ancho, float alto);
    ~Renderizador();
    void dibujar();
    void dibujarConEstado(Juego& juego);
    int nodoBajoMouse(Vector2 mousePos) const;
    void setColorNodo(int indice, Color color);
    void setSeleccionado(int indice, bool sel);
    void resetearColores();
    void sincronizarConJuego(Juego& juego);
};

#endif
