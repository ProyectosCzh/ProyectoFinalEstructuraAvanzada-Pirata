#include "panel_info.hpp"
#include "colores.hpp"
#include <cstdio>
#include <cstring>

PanelInfo::PanelInfo(Rectangle area) {
    this->area = area;
    this->fuenteTam = 14;
}

void PanelInfo::dibujar(const char* titulo, const char* pistaActual,
                         const char* nodoNombre, int costo,
                         const char* estadoStr, int visitados,
                         int pasoActual,
                         const Lista& ruta,
                         const Cola& cola,
                         const Pila& pila) {
    DrawRectangleRec(area, COLOR_PANEL_FONDO);
    DrawRectangleLinesEx(area, 2, DARKGRAY);

    float y = area.y + 5;
    float x = area.x + 5;
    float ancho = area.width - 10;

    DrawText(titulo, (int)x, (int)y, 18, YELLOW);
    y += 25;

    char buffer[256];

    if (nodoNombre != nullptr) {
        std::sprintf(buffer, "Nodo: %s", nodoNombre);
        DrawText(buffer, (int)x, (int)y, fuenteTam, COLOR_TEXTO);
        y += 20;
    }

    if (pistaActual != nullptr) {
        std::sprintf(buffer, "Pista: %s", pistaActual);
        DrawText(buffer, (int)x, (int)y, fuenteTam, COLOR_TEXTO);
        y += 20;
    }

    std::sprintf(buffer, "Costo: %d", costo);
    DrawText(buffer, (int)x, (int)y, fuenteTam, COLOR_TEXTO);
    y += 20;

    if (estadoStr != nullptr) {
        std::sprintf(buffer, "Estado: %s", estadoStr);
        DrawText(buffer, (int)x, (int)y, fuenteTam, COLOR_TEXTO);
        y += 20;
    }

    std::sprintf(buffer, "Visitados: %d", visitados);
    DrawText(buffer, (int)x, (int)y, fuenteTam, COLOR_TEXTO);
    y += 20;

    std::sprintf(buffer, "Paso: %d", pasoActual);
    DrawText(buffer, (int)x, (int)y, fuenteTam, COLOR_TEXTO);
    y += 25;

    if (ruta.longitud() > 0) {
        DrawText("Ruta:", (int)x, (int)y, fuenteTam, YELLOW);
        y += 18;

        char rutaStr[512] = "";
        for (int i = 0; i < ruta.longitud(); i++) {
            char num[10];
            std::sprintf(num, "%d", ruta.obtener(i));
            std::strcat(rutaStr, num);
            if (i < ruta.longitud() - 1) {
                std::strcat(rutaStr, " -> ");
            }
        }
        DrawText(rutaStr, (int)x, (int)y, 12, COLOR_TEXTO_OSCURO);
    }

    float panelMedio = area.height * 0.55f;
    float margen = 5.0f;
    float anchoCola = (ancho - margen) / 2.0f;
    float altoCola = area.height - panelMedio - margen * 2;

    Rectangle areaCola = { x, area.y + panelMedio, anchoCola, altoCola };
    Rectangle areaPila = { x + anchoCola + margen, area.y + panelMedio, anchoCola, altoCola };

    dibujarColaVisual(cola, areaCola);
    dibujarPilaVisual(pila, areaPila);
}

void PanelInfo::dibujarColaVisual(const Cola& cola, Rectangle areaCola) {
    DrawRectangleLinesEx(areaCola, 1, DARKGRAY);
    DrawText("COLA (BFS)", (int)areaCola.x + 5, (int)areaCola.y + 2, 12, YELLOW);

    int tamCola = cola.obtenerTam();
    float barraAncho = areaCola.width - 10;
    float barraAlto = 18;
    float y = areaCola.y + 20;

    for (int i = 0; i < tamCola && i < 8; i++) {
        int valor = cola.getDatoEn(i);
        Color color = (i == 0) ? COLOR_COLA_BARRA :
                      ColorAlpha(COLOR_COLA_BARRA, 1.0f - i * 0.1f);
        char texto[10];
        std::sprintf(texto, "%d", valor);
        DrawRectangle((int)areaCola.x + 5, (int)y, (int)barraAncho, (int)barraAlto, color);
        DrawRectangleLines((int)areaCola.x + 5, (int)y, (int)barraAncho, (int)barraAlto, DARKGRAY);
        DrawText(texto, (int)areaCola.x + 10, (int)y + 2, 12, BLACK);
        y += barraAlto + 2;
    }

    if (tamCola == 0) {
        DrawText("(vacia)", (int)areaCola.x + 20, (int)y + 2, 12, COLOR_TEXTO_OSCURO);
    }
}

void PanelInfo::dibujarPilaVisual(const Pila& pila, Rectangle areaPila) {
    DrawRectangleLinesEx(areaPila, 1, DARKGRAY);
    DrawText("PILA (DFS)", (int)areaPila.x + 5, (int)areaPila.y + 2, 12, YELLOW);

    int tamPila = pila.obtenerTam();
    float barraAncho = areaPila.width - 10;
    float barraAlto = 18;
    float y = areaPila.y + 20;

    for (int i = tamPila - 1; i >= 0 && (tamPila - 1 - i) < 8; i--) {
        int valor = (pila.getDatos() != nullptr) ? pila.getDatos()[i] : -1;
        float intensidad = 1.0f - (tamPila - 1 - i) * 0.1f;
        if (intensidad < 0.3f) intensidad = 0.3f;

        Color color = {
            (unsigned char)(COLOR_PILA_BARRA.r * intensidad),
            (unsigned char)(COLOR_PILA_BARRA.g * intensidad),
            (unsigned char)(COLOR_PILA_BARRA.b * intensidad),
            255
        };
        char texto[10];
        std::sprintf(texto, "%d", valor);
        DrawRectangle((int)areaPila.x + 5, (int)y, (int)barraAncho, (int)barraAlto, color);
        DrawRectangleLines((int)areaPila.x + 5, (int)y, (int)barraAncho, (int)barraAlto, DARKGRAY);
        DrawText(texto, (int)areaPila.x + 10, (int)y + 2, 12, BLACK);
        y += barraAlto + 2;
    }

    if (tamPila == 0) {
        DrawText("(vacia)", (int)areaPila.x + 20, (int)y + 2, 12, COLOR_TEXTO_OSCURO);
    }
}
