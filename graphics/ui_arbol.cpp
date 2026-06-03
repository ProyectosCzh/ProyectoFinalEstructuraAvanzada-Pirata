#include "ui_arbol.hpp"
#include "colores.hpp"
#include <cstring>
#include <cstdio>

static const float RADIO_NODO = 15.0f;
static const float ALTO_FILA  = 38.0f;
static const float ESPACIO_HORIZONTAL = 8.0f;

UIArbol::UIArbol(Arbol* a, Rectangle area) {
    arbol = a;
    this->area = area;
    pistaActiva = nullptr;
    scroll = { 0, 0 };
}

void UIArbol::dibujar() {
    DrawRectangleRec(area, COLOR_PANEL_FONDO);
    DrawRectangleLinesEx(area, 2, COLOR_MARCO);

    DrawText("ARBOL DE PISTAS", (int)area.x + 8, (int)area.y + 5, 18, COLOR_DORADO);
    DrawText("Rueda mouse: scroll", (int)area.x + 8, (int)(area.y + area.height - 18), 11, COLOR_TEXTO_OSCURO);
    DrawText("Cada pista lleva a la siguiente ubicacion", (int)area.x + 8, (int)(area.y + area.height - 34), 11, COLOR_TEXTO_OSCURO);

    if (arbol == nullptr || arbol->getRaiz() == nullptr) {
        DrawText("(sin datos)", (int)area.x + 20, (int)area.y + 40, 14, COLOR_TEXTO_OSCURO);
        return;
    }

    NodoArbol* raiz = arbol->getRaiz();
    if (raiz->numHijos == 0) return;

    float margenX = 30;
    float margenY = 34;
    float anchoClip = area.width - margenX * 2;
    float altoClip  = area.height - margenY - 44;

    int maxNiveles = 0;
    for (int i = 0; i < raiz->numHijos; i++) {
        int n = contarNiveles(raiz->hijos[i]);
        if (n > maxNiveles) maxNiveles = n;
    }
    float contenidoTotal = maxNiveles * ALTO_FILA;
    float scrollMin = altoClip - contenidoTotal;
    if (scrollMin > 0) scrollMin = 0;
    if (scroll.y > 0) scroll.y = 0;
    if (scroll.y < scrollMin) scroll.y = scrollMin;

    BeginScissorMode((int)(area.x + margenX), (int)(area.y + margenY),
                     (int)anchoClip, (int)altoClip);

    float xBase = area.x + margenX + RADIO_NODO + 30;

    for (int i = 0; i < raiz->numHijos; i++) {
        float x = xBase + scroll.x;
        float y = area.y + margenY + ALTO_FILA * 0.5f + scroll.y;
        float dy = ALTO_FILA;

        if (raiz->numHijos == 1) {
            dibujarCadena(raiz->hijos[i], x, y, dy);
        } else {
            float xRama = x + i * (anchoClip - 30) / (raiz->numHijos - 1);
            dibujarCadena(raiz->hijos[i], xRama, y, dy);
        }
    }

    EndScissorMode();
}

void UIArbol::dibujarCadena(NodoArbol* nodo, float x, float y, float dy) {
    int idx = 0;
    NodoArbol* actual = nodo;
    while (actual != nullptr) {
        float cy = y + idx * dy;
        float cx = x;

        bool activo = (pistaActiva != nullptr &&
                       std::strcmp(actual->pista, pistaActiva) == 0);

        Color colorNodo = activo ? COLOR_ARBOL_ACTIVO : COLOR_ARBOL_LINEA;
        float radio = activo ? RADIO_NODO + 2.0f : RADIO_NODO;

        if (activo) {
            DrawCircle((int)cx, (int)cy, radio + 5, ColorAlpha(COLOR_ARBOL_ACTIVO, 0.20f));
        }

        DrawCircle((int)cx, (int)cy, radio, colorNodo);
        DrawCircleLines((int)cx, (int)cy, radio, COLOR_MARCO_BRILLO);

        const char* destino = actual->destino;
        if (destino != nullptr) {
            int tamF = (std::strlen(destino) > 7) ? 10 : 12;
            int anchoT = MeasureText(destino, tamF);
            DrawText(destino, (int)cx - anchoT / 2, (int)cy - tamF / 2, tamF,
                     activo ? BLACK : COLOR_TEXTO_NODO);
        }

        float iniLineaX = cx + radio + 3;
        float finLineaX = cx + radio + ESPACIO_HORIZONTAL;
        DrawLineEx({ iniLineaX, cy }, { finLineaX, cy }, 1.5f, COLOR_ARBOL_LINEA);

        const char* pistaTxt = actual->pista;
        if (pistaTxt != nullptr && pistaTxt[0] != '\0') {
            float anchoPistaDisp = (area.x + area.width - 32) - finLineaX;
            if (anchoPistaDisp > 20) {
                int tamP = 11;
                int anchoP = MeasureText(pistaTxt, tamP);
                char buf[160];
                std::strncpy(buf, pistaTxt, 159);
                buf[159] = '\0';

                if (anchoP > anchoPistaDisp) {
                    int maxChars = (int)(anchoPistaDisp / (tamP * 0.55f)) - 2;
                    if (maxChars < 3) maxChars = 3;
                    if (maxChars > 159) maxChars = 159;
                    buf[maxChars - 1] = '.';
                    buf[maxChars - 2] = '.';
                    buf[maxChars - 3] = '.';
                    buf[maxChars] = '\0';
                }

                Color colorPista = activo ? COLOR_DORADO : COLOR_ARBOL_TEXTO_PISTA;
                DrawText(buf, (int)finLineaX + 4, (int)cy - 6, tamP, colorPista);
            }
        }

        if (actual->numHijos > 0) {
            float sigCy = y + (idx + 1) * dy;
            DrawLineEx({ cx, cy + radio + 2 },
                       { cx, sigCy - radio - 2 },
                       activo ? 2.5f : 1.5f,
                       activo ? COLOR_ARBOL_ACTIVO : COLOR_ARBOL_LINEA);
        }

        if (actual->numHijos > 0) {
            actual = actual->hijos[0];
            idx++;
        } else {
            actual = nullptr;
        }
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
