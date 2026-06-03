#include "ui_arbol.hpp"
#include "colores.hpp"
#include <cstring>
#include <cstdio>

UIArbol::UIArbol(Arbol* a, Rectangle area) {
    arbol = a;
    this->area = area;
    pistaActiva = nullptr;
    scroll = { 0, 0 };
}

void UIArbol::dibujar() {
    DrawRectangleRec(area, COLOR_PANEL_FONDO);
    DrawRectangleLinesEx(area, 2, COLOR_MARCO);

    DrawText("ARBOL DE DECISION", (int)area.x + 8, (int)area.y + 5, 18, COLOR_DORADO);
    DrawText("Rueda mouse: scroll", (int)area.x + 8, (int)(area.y + area.height - 18), 11, COLOR_TEXTO_OSCURO);

    if (arbol == nullptr || arbol->getRaiz() == nullptr) {
        DrawText("(sin datos)", (int)area.x + 20, (int)area.y + 40, 14, COLOR_TEXTO_OSCURO);
        return;
    }

    NodoArbol* raiz = arbol->getRaiz();
    if (raiz->numHijos == 0) return;

    float margenX = 14;
    float margenY = 40;
    float anchoDisp = area.width - margenX * 2;
    float altoDisp = area.height - margenY - 28;

    BeginScissorMode((int)(area.x + margenX), (int)(area.y + margenY),
                     (int)anchoDisp, (int)altoDisp);

    int numHijosRaiz = raiz->numHijos;
    for (int i = 0; i < numHijosRaiz; i++) {
        int niveles = contarNiveles(raiz->hijos[i]);
        float dx = anchoDisp / (numHijosRaiz + 1);
        float x = area.x + margenX + dx * (i + 1) + scroll.x;
        float y = area.y + margenY + 20 + scroll.y;
        float dy = (altoDisp - 30) / (niveles > 1 ? niveles : 1);

        dibujarNodo(raiz->hijos[i], x, y, dx * 0.5f, dy, 1, niveles);
    }

    EndScissorMode();
}

void UIArbol::dibujarNodo(NodoArbol* nodo, float x, float y, float dx, float dy, int nivel, int maxNivel) {
    if (nodo == nullptr) return;

    Color colorNodo = COLOR_ARBOL_LINEA;
    Color colorTexto = COLOR_TEXTO;
    bool activo = false;
    bool enCamino = false;

    if (pistaActiva != nullptr && std::strcmp(nodo->pista, pistaActiva) == 0) {
        colorNodo = COLOR_ARBOL_ACTIVO;
        colorTexto = BLACK;
        activo = true;
    }

    float radio = activo ? 18.0f : 16.0f;

    if (activo) {
        DrawCircle((int)x, (int)y, radio + 4, ColorAlpha(COLOR_ARBOL_ACTIVO, 0.25f));
    }

    DrawCircle((int)x, (int)y, radio, colorNodo);
    DrawCircleLines((int)x, (int)y, radio, COLOR_MARCO_BRILLO);

    const char* texto = nodo->destino;
    if (texto != nullptr) {
        int tamF = (std::strlen(texto) > 8) ? 11 : 13;
        int anchoTexto = MeasureText(texto, tamF);
        DrawText(texto, (int)x - anchoTexto / 2, (int)y - tamF / 2, tamF, colorTexto);
    }

    if (nodo->numHijos > 0) {
        const char* pistaTxt = nodo->pista;
        if (pistaTxt != nullptr && pistaTxt[0] != '\0') {
            char truncado[80];
            int max = (int)std::strlen(pistaTxt);
            if (max > 60) max = 60;
            std::strncpy(truncado, pistaTxt, max);
            truncado[max] = '\0';
            int anchoP = MeasureText(truncado, 10);
            int xp = (int)x - anchoP / 2;
            int yp = (int)y + (int)radio + 3;
            DrawRectangle(xp - 2, yp - 1, anchoP + 4, 13, COLOR_PANEL_INTERNO);
            DrawText(truncado, xp, yp, 10, COLOR_TEXTO_OSCURO);
        }
    }

    for (int i = 0; i < nodo->numHijos; i++) {
        float hx = x - dx + 2 * dx * i / (nodo->numHijos > 1 ? nodo->numHijos - 1 : 1);
        float hy = y + dy;

        Color colorLinea = COLOR_ARBOL_LINEA;
        if (activo) {
            colorLinea = COLOR_ARBOL_ACTIVO;
        }

        DrawLineEx({ (float)x, (float)(y + radio + 2) },
                   { (float)hx, (float)(hy - radio) },
                   activo ? 2.5f : 1.5f, colorLinea);

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
