#include "ui_arbol.hpp"
#include "colores.hpp"
#include <cstring>
#include <cstdio>

UIArbol::UIArbol(Arbol* a, Rectangle area) {
    arbol = a;
    this->area = area;
    pistaActiva = nullptr;
}

void UIArbol::dibujar() {
    DrawRectangleRec(area, COLOR_PANEL_FONDO);
    DrawRectangleLinesEx(area, 2, DARKGRAY);

    DrawText("ARBOL DE PISTAS", (int)area.x + 5, (int)area.y + 3, 14, YELLOW);

    if (arbol == nullptr || arbol->getRaiz() == nullptr) {
        DrawText("(sin datos)", (int)area.x + 20, (int)area.y + 30, 12, COLOR_TEXTO_OSCURO);
        return;
    }

    NodoArbol* raiz = arbol->getRaiz();
    if (raiz->numHijos == 0) return;

    float margenX = 10;
    float margenY = 30;
    float anchoDisp = area.width - margenX * 2;
    float altoDisp = area.height - margenY - 10;

    for (int i = 0; i < raiz->numHijos; i++) {
        int niveles = contarNiveles(raiz->hijos[i]);
        float dx = anchoDisp / (raiz->numHijos + 1);
        float x = area.x + margenX + dx * (i + 1);
        float y = area.y + margenY + 15;
        float dy = (altoDisp - 30) / (niveles > 1 ? niveles : 1);

        dibujarNodo(raiz->hijos[i], x, y, dx * 0.4f, dy, 1, niveles);
    }
}

void UIArbol::dibujarNodo(NodoArbol* nodo, float x, float y, float dx, float dy, int nivel, int maxNivel) {
    if (nodo == nullptr) return;

    Color colorNodo = COLOR_ARBOL_LINEA;
    Color colorTexto = COLOR_TEXTO;
    bool activo = false;

    if (pistaActiva != nullptr && std::strcmp(nodo->pista, pistaActiva) == 0) {
        colorNodo = YELLOW;
        colorTexto = BLACK;
        activo = true;
    }

    if (activo) {
        DrawCircle((int)x, (int)y, 16, colorNodo);
    } else {
        DrawCircle((int)x, (int)y, 14, colorNodo);
    }
    DrawCircleLines((int)x, (int)y, activo ? 16 : 14, DARKGRAY);

    const char* texto = nodo->destino;
    if (texto != nullptr) {
        int anchoTexto = MeasureText(texto, 10);
        DrawText(texto, (int)x - anchoTexto / 2, (int)y - 5, 10, colorTexto);
    }

    for (int i = 0; i < nodo->numHijos; i++) {
        float hx = x - dx + 2 * dx * i / (nodo->numHijos > 1 ? nodo->numHijos - 1 : 1);
        float hy = y + dy;

        DrawLine((int)x, (int)y + 14, (int)hx, (int)hy - 14, COLOR_ARBOL_LINEA);

        dibujarNodo(nodo->hijos[i], hx, hy, dx / 2, dy, nivel + 1, maxNivel);
    }
}

int UIArbol::contarNiveles(NodoArbol* nodo) {
    if (nodo == nullptr || nodo->numHijos == 0) return 1;
    int maxHijo = 0;
    for (int i = 0; i < nodo->numHijos; i++) {
        int h = contarNiveles(nodo->hijos[i]);
        if (h > maxHijo) maxHijo = h;
    }
    return 1 + maxHijo;
}

void UIArbol::setNodoActivo(const char* pista) {
    pistaActiva = pista;
}
