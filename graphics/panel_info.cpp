#include "panel_info.hpp"
#include "colores.hpp"
#include "../include/grafo.hpp"
#include <cstdio>
#include <cstring>

PanelInfo::PanelInfo(Rectangle area) {
    this->area = area;
    this->fuenteTam = 18;
}

void PanelInfo::dibujar(const char* titulo, const char* pistaActual,
                         const char* nodoNombre, int costo,
                         const char* estadoStr, int visitados,
                         int pasoActual,
                         const Lista& ruta,
                         const Cola& cola,
                         const Pila& pila,
                         const Grafo* grafo,
                         const char* nodoProcesando) {
    DrawRectangleRec(area, COLOR_PANEL_FONDO);
    DrawRectangleLinesEx(area, 2, COLOR_MARCO);

    float y = area.y + 8;
    float x = area.x + 8;
    float ancho = area.width - 16;

    DrawText(titulo, (int)x, (int)y, 22, COLOR_DORADO);
    y += 32;

    char buffer[256];
    char etiqueta[64];

    if (nodoNombre != nullptr) {
        std::sprintf(etiqueta, "Nodo:");
        std::sprintf(buffer, " %s", nodoNombre);
        int anchoEtiq = MeasureText(etiqueta, fuenteTam);
        DrawText(etiqueta, (int)x, (int)y, fuenteTam, COLOR_TEXTO_OSCURO);
        DrawText(buffer, (int)x + anchoEtiq, (int)y, fuenteTam, COLOR_TEXTO);
        y += 24;
    }

    if (pistaActual != nullptr) {
        std::sprintf(etiqueta, "Pista:");
        std::sprintf(buffer, " %s", pistaActual);
        int anchoEtiq = MeasureText(etiqueta, fuenteTam);
        DrawText(etiqueta, (int)x, (int)y, fuenteTam, COLOR_TEXTO_OSCURO);
        DrawText(buffer, (int)x + anchoEtiq, (int)y, fuenteTam, COLOR_TEXTO);
        y += 24;
    }

    std::sprintf(etiqueta, "Costo:");
    std::sprintf(buffer, " %d", costo);
    int anchoEtiq = MeasureText(etiqueta, fuenteTam);
    DrawText(etiqueta, (int)x, (int)y, fuenteTam, COLOR_TEXTO_OSCURO);
    DrawText(buffer, (int)x + anchoEtiq, (int)y, fuenteTam, COLOR_TEXTO);
    y += 24;

    if (estadoStr != nullptr) {
        std::sprintf(etiqueta, "Estado:");
        std::sprintf(buffer, " %s", estadoStr);
        int anchoE = MeasureText(etiqueta, fuenteTam);
        DrawText(etiqueta, (int)x, (int)y, fuenteTam, COLOR_TEXTO_OSCURO);
        DrawText(buffer, (int)x + anchoE, (int)y, 16, COLOR_NODO_VISITADO);
        y += 24;
    }

    std::sprintf(etiqueta, "Visitados:");
    std::sprintf(buffer, " %d", visitados);
    int anchoV = MeasureText(etiqueta, fuenteTam);
    DrawText(etiqueta, (int)x, (int)y, fuenteTam, COLOR_TEXTO_OSCURO);
    DrawText(buffer, (int)x + anchoV, (int)y, fuenteTam, COLOR_TEXTO);
    y += 24;

    std::sprintf(etiqueta, "Paso:");
    std::sprintf(buffer, " %d", pasoActual);
    int anchoP = MeasureText(etiqueta, fuenteTam);
    DrawText(etiqueta, (int)x, (int)y, fuenteTam, COLOR_TEXTO_OSCURO);
    DrawText(buffer, (int)x + anchoP, (int)y, fuenteTam, COLOR_TEXTO);
    y += 24;

    if (nodoProcesando != nullptr) {
        std::sprintf(etiqueta, "Procesando:");
        std::sprintf(buffer, " %s", nodoProcesando);
        int anchoPr = MeasureText(etiqueta, fuenteTam);
        DrawText(etiqueta, (int)x, (int)y, fuenteTam, COLOR_TEXTO_OSCURO);
        DrawText(buffer, (int)x + anchoPr, (int)y, fuenteTam, COLOR_DORADO);
        y += 24;
    }

    if (ruta.longitud() > 0) {
        DrawText("Ruta:", (int)x, (int)y, fuenteTam, COLOR_DORADO);
        y += 22;

        char rutaStr[512] = "";
        for (int i = 0; i < ruta.longitud(); i++) {
            int idx = ruta.obtener(i);
            const char* nombre = (grafo != nullptr) ? grafo->getNombreNodo(idx) : nullptr;
            if (nombre == nullptr) {
                char num[10];
                std::sprintf(num, "%d", idx);
                std::strcat(rutaStr, num);
            } else {
                std::strcat(rutaStr, nombre);
            }
            if (i < ruta.longitud() - 1) {
                std::strcat(rutaStr, " -> ");
            }
        }
        DrawText(rutaStr, (int)x, (int)y, 16, COLOR_TEXTO);
    }

    float panelMedio = area.height * 0.50f;
    float margen = 6.0f;
    float anchoCola = (ancho - margen) / 2.0f;
    float altoCola = area.height - panelMedio - margen * 2;

    Rectangle areaCola = { x, area.y + panelMedio, anchoCola, altoCola };
    Rectangle areaPila = { x + anchoCola + margen, area.y + panelMedio, anchoCola, altoCola };

    dibujarColaVisual(cola, areaCola, grafo);
    dibujarPilaVisual(pila, areaPila, grafo);
}

void PanelInfo::dibujarColaVisual(const Cola& cola, Rectangle areaCola, const Grafo* grafo) {
    DrawRectangleRec(areaCola, COLOR_PANEL_INTERNO);
    DrawRectangleLinesEx(areaCola, 1, COLOR_MARCO);
    DrawText("COLA (BFS)", (int)areaCola.x + 5, (int)areaCola.y + 3, 14, COLOR_DORADO);

    int tamCola = cola.obtenerTam();
    float barraX0 = areaCola.x + 10;
    float barraY  = areaCola.y + 26;
    float barraW  = areaCola.width - 60;
    float barraH  = 22;
    float sep     = 4;

    DrawText("frente ->", (int)(areaCola.x + 5), (int)(areaCola.y + areaCola.height - 18), 12, COLOR_TEXTO_OSCURO);
    DrawText("<- final", (int)(areaCola.x + areaCola.width - 55), (int)(areaCola.y + areaCola.height - 18), 12, COLOR_TEXTO_OSCURO);

    int maxVisibles = (int)((areaCola.height - 50) / (barraH + sep));
    int aMostrar = (tamCola < maxVisibles) ? tamCola : maxVisibles;

    for (int i = 0; i < aMostrar; i++) {
        int valor = cola.getDatoEn(i);
        const char* nombre = (grafo != nullptr) ? grafo->getNombreNodo(valor) : nullptr;

        float intensidad = 1.0f - i * 0.08f;
        if (intensidad < 0.4f) intensidad = 0.4f;
        Color color = {
            (unsigned char)(COLOR_COLA_BARRA.r * intensidad),
            (unsigned char)(COLOR_COLA_BARRA.g * intensidad),
            (unsigned char)(COLOR_COLA_BARRA.b * intensidad),
            255
        };

        DrawRectangle((int)barraX0, (int)barraY, (int)barraW, (int)barraH, color);
        DrawRectangleLines((int)barraX0, (int)barraY, (int)barraW, (int)barraH, COLOR_MARCO);

        Color txtColor = (intensidad > 0.65f) ? BLACK : COLOR_TEXTO_NODO;
        if (nombre != nullptr) {
            DrawText(nombre, (int)barraX0 + 8, (int)barraY + 3, 16, txtColor);
        } else {
            char texto[10];
            std::sprintf(texto, "%d", valor);
            DrawText(texto, (int)barraX0 + 8, (int)barraY + 3, 16, txtColor);
        }

        if (i == 0) {
            DrawText("FRENTE", (int)(areaCola.x + 5), (int)barraY + 3, 12, COLOR_DORADO);
        }

        barraY += barraH + sep;
    }

    if (tamCola > maxVisibles) {
        char mas[20];
        std::sprintf(mas, "+%d mas...", tamCola - maxVisibles);
        DrawText(mas, (int)barraX0, (int)barraY + 2, 12, COLOR_TEXTO_OSCURO);
    }

    if (tamCola == 0) {
        DrawText("(vacia)", (int)(areaCola.x + 30), (int)(areaCola.y + 40), 14, COLOR_TEXTO_OSCURO);
    }
}

void PanelInfo::dibujarPilaVisual(const Pila& pila, Rectangle areaPila, const Grafo* grafo) {
    DrawRectangleRec(areaPila, COLOR_PANEL_INTERNO);
    DrawRectangleLinesEx(areaPila, 1, COLOR_MARCO);
    DrawText("PILA (DFS)", (int)areaPila.x + 5, (int)areaPila.y + 3, 14, COLOR_DORADO);

    int tamPila = pila.obtenerTam();
    float sep = 4;
    float barraH = 22;
    float barraW = areaPila.width - 20;
    float x = areaPila.x + 10;

    float yBase = areaPila.y + areaPila.height - 20;
    int maxVisibles = (int)((areaPila.height - 60) / (barraH + sep));
    int aMostrar = (tamPila < maxVisibles) ? tamPila : maxVisibles;

    int* datos = pila.getDatos();

    for (int i = 0; i < aMostrar; i++) {
        int idxEnPila = tamPila - 1 - i;
        int valor = (datos != nullptr && idxEnPila >= 0) ? datos[idxEnPila] : -1;
        const char* nombre = (grafo != nullptr) ? grafo->getNombreNodo(valor) : nullptr;

        float yBloque = yBase - (i + 1) * (barraH + sep);

        float intensidad = 1.0f - i * 0.10f;
        if (intensidad < 0.4f) intensidad = 0.4f;
        Color color = {
            (unsigned char)(COLOR_PILA_BARRA.r * intensidad),
            (unsigned char)(COLOR_PILA_BARRA.g * intensidad),
            (unsigned char)(COLOR_PILA_BARRA.b * intensidad),
            255
        };

        DrawRectangle((int)x, (int)yBloque, (int)barraW, (int)barraH, color);
        DrawRectangleLines((int)x, (int)yBloque, (int)barraW, (int)barraH, COLOR_MARCO);

        Color txtColor = (intensidad > 0.65f) ? BLACK : COLOR_TEXTO_NODO;
        if (nombre != nullptr) {
            DrawText(nombre, (int)x + 8, (int)yBloque + 3, 16, txtColor);
        } else {
            char texto[10];
            std::sprintf(texto, "%d", valor);
            DrawText(texto, (int)x + 8, (int)yBloque + 3, 16, txtColor);
        }

        if (i == 0) {
            const char* tag = "TOP";
            int anchoTag = MeasureText(tag, 12);
            DrawText(tag, (int)(x + barraW - anchoTag - 8), (int)yBloque + 4, 12, COLOR_DORADO);
        }
    }

    if (tamPila > maxVisibles) {
        char mas[20];
        std::sprintf(mas, "+%d mas...", tamPila - maxVisibles);
        DrawText(mas, (int)x, (int)(areaPila.y + 25), 12, COLOR_TEXTO_OSCURO);
    }

    if (tamPila == 0) {
        DrawText("(vacia)", (int)(areaPila.x + 30), (int)(areaPila.y + 40), 14, COLOR_TEXTO_OSCURO);
    }
}
