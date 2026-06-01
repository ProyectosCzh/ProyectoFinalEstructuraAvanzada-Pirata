#include "renderizador.hpp"
#include "colores.hpp"
#include <cmath>
#include <cstdio>
#include <cstring>

Renderizador::Renderizador(Grafo* g, float ancho, float alto) {
    grafo = g;
    numNodos = g->getNumNodos();
    anchoPanel = ancho;
    altoPanel = alto;
    nodosVisuales = new NodoVisual[numNodos];
    calcularLayout();
    resetearColores();
}

Renderizador::~Renderizador() {
    delete[] nodosVisuales;
}

void Renderizador::calcularLayout() {
    if (numNodos == 0) return;

    float cx = anchoPanel * 0.45f;
    float cy = altoPanel * 0.40f;
    float radioCirculo = (anchoPanel < altoPanel ? anchoPanel : altoPanel) * 0.32f;

    for (int i = 0; i < numNodos; i++) {
        float angulo = 2.0f * 3.14159265f * i / numNodos - 3.14159265f / 2.0f;
        nodosVisuales[i].posicion.x = cx + radioCirculo * cosf(angulo);
        nodosVisuales[i].posicion.y = cy + radioCirculo * sinf(angulo);
        nodosVisuales[i].radio = 22.0f;
        nodosVisuales[i].seleccionado = false;
    }
}

void Renderizador::dibujarAristas() {
    for (int i = 0; i < numNodos; i++) {
        Arista* arista = grafo->getAristas(i);
        while (arista != nullptr) {
            int j = arista->destino;
            Vector2 p1 = nodosVisuales[i].posicion;
            Vector2 p2 = nodosVisuales[j].posicion;

            DrawLineEx(p1, p2, 2.0f, COLOR_ARISTA);

            float medioX = (p1.x + p2.x) / 2.0f;
            float medioY = (p1.y + p2.y) / 2.0f;

            char pesoStr[10];
            std::sprintf(pesoStr, "%d", arista->peso);
            DrawText(pesoStr, (int)medioX - 10, (int)medioY - 10, 15, COLOR_TEXTO_OSCURO);

            arista = arista->siguiente;
        }
    }
}

void Renderizador::dibujarNodos() {
    for (int i = 0; i < numNodos; i++) {
        NodoVisual& nv = nodosVisuales[i];
        float r = nv.radio;

        if (nv.seleccionado) {
            DrawCircleLines((int)nv.posicion.x, (int)nv.posicion.y, r + 4, WHITE);
        }

        DrawCircle((int)nv.posicion.x, (int)nv.posicion.y, r, nv.color);
        DrawCircleLines((int)nv.posicion.x, (int)nv.posicion.y, r, DARKGRAY);
    }
}

void Renderizador::dibujarNombres() {
    for (int i = 0; i < numNodos; i++) {
        const char* nombre = grafo->getNombreNodo(i);
        if (nombre != nullptr) {
            int anchoTexto = MeasureText(nombre, 15);
            DrawText(nombre,
                     (int)nodosVisuales[i].posicion.x - anchoTexto / 2,
                     (int)nodosVisuales[i].posicion.y + nodosVisuales[i].radio + 5,
                     15, COLOR_TEXTO);
        }
    }
}

void Renderizador::dibujarPesos() {
}

void Renderizador::dibujarLeyenda() {
    int y = 10;
    DrawRectangle(5, y, 180, 110, { 0, 0, 0, 150 });
    y += 5;

    DrawCircle(20, y + 5, 5, COLOR_NODO_NO_VIS);
    DrawText("No visitado", 30, y, 12, COLOR_TEXTO);
    y += 18;

    DrawCircle(20, y + 5, 5, COLOR_NODO_EN_COLA);
    DrawText("En cola/pila", 30, y, 12, COLOR_TEXTO);
    y += 18;

    DrawCircle(20, y + 5, 5, COLOR_NODO_VISITADO);
    DrawText("Visitado", 30, y, 12, COLOR_TEXTO);
    y += 18;

    DrawCircle(20, y + 5, 5, COLOR_NODO_RUTA);
    DrawText("Ruta final", 30, y, 12, COLOR_TEXTO);
    y += 18;

    DrawCircle(20, y + 5, 5, COLOR_TESORO);
    DrawText("Tesoro", 30, y, 12, COLOR_TEXTO);
}

void Renderizador::dibujar() {
    dibujarAristas();
    dibujarNodos();
    dibujarNombres();
    dibujarLeyenda();
}

void Renderizador::dibujarConEstado(Juego& juego) {
    sincronizarConJuego(juego);
    dibujar();
}

void Renderizador::sincronizarConJuego(Juego& juego) {
    for (int i = 0; i < numNodos; i++) {
        if (juego.getNodoSeleccionado() == i) {
            nodosVisuales[i].color = COLOR_NODO_SELECCION;
        } else if (juego.getEnRuta(i)) {
            nodosVisuales[i].color = COLOR_NODO_RUTA;
        } else if (juego.getVisitado(i)) {
            const char* nombre = grafo->getNombreNodo(i);
            if (nombre != nullptr && std::strcmp(nombre, "tesoro") == 0) {
                nodosVisuales[i].color = COLOR_TESORO;
            } else {
                nodosVisuales[i].color = COLOR_NODO_VISITADO;
            }
        } else {
            nodosVisuales[i].color = COLOR_NODO_NO_VIS;
        }
    }
}

int Renderizador::nodoBajoMouse(Vector2 mousePos) const {
    for (int i = 0; i < numNodos; i++) {
        float dx = mousePos.x - nodosVisuales[i].posicion.x;
        float dy = mousePos.y - nodosVisuales[i].posicion.y;
        float dist = sqrtf(dx * dx + dy * dy);
        if (dist <= nodosVisuales[i].radio + 5.0f) {
            return i;
        }
    }
    return -1;
}

void Renderizador::setColorNodo(int indice, Color color) {
    if (indice >= 0 && indice < numNodos) {
        nodosVisuales[indice].color = color;
    }
}

void Renderizador::setSeleccionado(int indice, bool sel) {
    if (indice >= 0 && indice < numNodos) {
        nodosVisuales[indice].seleccionado = sel;
    }
}

void Renderizador::resetearColores() {
    for (int i = 0; i < numNodos; i++) {
        nodosVisuales[i].seleccionado = false;
        const char* nombre = grafo->getNombreNodo(i);
        if (nombre != nullptr && std::strcmp(nombre, "tesoro") == 0) {
            nodosVisuales[i].color = COLOR_TESORO;
        } else {
            nodosVisuales[i].color = COLOR_NODO_NO_VIS;
        }
    }
}
