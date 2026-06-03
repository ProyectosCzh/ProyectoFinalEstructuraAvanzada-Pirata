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
    float pulse;
};

class Renderizador {
private:
    Grafo* grafo;
    NodoVisual* nodosVisuales;
    int numNodos;
    float anchoPanel;
    float altoPanel;

    Vector2 offset;
    float zoom;
    int nodoHover;
    double tiempoInicio;

    void calcularLayout();
    void calcularLayoutRejilla();
    void dibujarAristas(const Lista* ruta, bool dirigido);
    void dibujarFlecha(Vector2 desde, Vector2 hasta, float radioDestino, Color color);
    void dibujarNodos();
    void dibujarNombres();
    void dibujarPesos();
    void dibujarLeyenda();
    void aplicarTransform(Vector2& p) const;
    void deshacerTransform(Vector2& p) const;

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
    void actualizarInput(Vector2 mousePos);
    void dibujarTooltip(Vector2 mousePos, const Juego& juego);
    void pan(Vector2 delta) { offset.x += delta.x; offset.y += delta.y; }
    void setZoom(float z) { if (z >= 0.4f && z <= 2.5f) zoom = z; }
    void resetearVista() { offset = {0, 0}; zoom = 1.0f; }
    Vector2 getOffset() const { return offset; }
    float getZoom() const { return zoom; }
};

#endif
