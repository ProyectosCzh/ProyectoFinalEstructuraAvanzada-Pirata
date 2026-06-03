#include "renderizador.hpp"
#include "colores.hpp"
#include <cmath>
#include <cstdio>
#include <cstring>

#ifndef PI
#define PI 3.14159265f
#endif

Renderizador::Renderizador(Grafo* g, float ancho, float alto) {
    grafo = g;
    numNodos = g->getNumNodos();
    anchoPanel = ancho;
    altoPanel = alto;
    nodosVisuales = new NodoVisual[numNodos];
    offset = { 0, 0 };
    zoom = 1.0f;
    nodoHover = -1;
    tiempoInicio = GetTime();
    modoEdicion = false;
    nodoArrastrando = -1;

    fondoCargado = false;

    if (grafo->tieneCoordenadas()) {
        usarCoordenadasDesdeGrafo();
    } else {
        calcularLayoutRejilla();
    }
    resetearColores();
}

Renderizador::~Renderizador() {
    if (fondoCargado) UnloadTexture(texturaFondo);
    delete[] nodosVisuales;
}

void Renderizador::calcularLayoutRejilla() {
    if (numNodos == 0) return;

    float marginX = 60.0f;
    float marginY = 80.0f;
    float anchoDisp = anchoPanel - marginX * 2;
    float altoDisp  = altoPanel  - marginY * 2;

    int cols = (int)std::ceil(std::sqrt((float)numNodos));
    int rows = (numNodos + cols - 1) / cols;

    float cellW = anchoDisp / cols;
    float cellH = altoDisp  / rows;

    float radio = 28.0f;
    if (numNodos > 15) radio = 24.0f;
    if (numNodos > 30) radio = 20.0f;

    for (int i = 0; i < numNodos; i++) {
        int fila  = i / cols;
        int col   = i % cols;
        nodosVisuales[i].posicion.x = marginX + cellW * (col + 0.5f);
        nodosVisuales[i].posicion.y = marginY + cellH * (fila + 0.5f);
        nodosVisuales[i].radio = radio;
        nodosVisuales[i].seleccionado = false;
    }
}

void Renderizador::usarCoordenadasDesdeGrafo() {
    if (numNodos == 0) return;
    for (int i = 0; i < numNodos; i++) {
        float vx = grafo->getCoordX(i);
        float vy = grafo->getCoordY(i);
        nodosVisuales[i].posicion.x = (vx / 1000.0f) * anchoPanel;
        nodosVisuales[i].posicion.y = (vy / 1000.0f) * altoPanel;
        nodosVisuales[i].radio = 22.0f;
        nodosVisuales[i].seleccionado = false;
    }
}

void Renderizador::dibujarFondo() {
    if (!fondoCargado) {
        Image img = LoadImage("MAPAISLA2.jpg");
        if (img.data != nullptr) {
            texturaFondo = LoadTextureFromImage(img);
            UnloadImage(img);
            fondoCargado = (texturaFondo.id > 0);
            if (fondoCargado) {
                printf("Textura de fondo cargada: %dx%d\n", texturaFondo.width, texturaFondo.height);
            }
        }
    }
    if (!fondoCargado) return;

    float scaleX = anchoPanel / (float)texturaFondo.width;
    float scaleY = altoPanel / (float)texturaFondo.height;
    float scale = fminf(scaleX, scaleY);

    float destW = texturaFondo.width * scale;
    float destH = texturaFondo.height * scale;
    float ox = (anchoPanel - destW) * 0.5f;
    float oy = (altoPanel - destH) * 0.5f;

    DrawTexturePro(texturaFondo,
        { 0, 0, (float)texturaFondo.width, (float)texturaFondo.height },
        { ox, oy, destW, destH },
        { 0, 0 }, 0.0f, WHITE);
}

void Renderizador::aplicarTransform(Vector2& p) const {
    p.x = (p.x - anchoPanel * 0.5f) * zoom + anchoPanel * 0.5f + offset.x;
    p.y = (p.y - altoPanel  * 0.5f) * zoom + altoPanel  * 0.5f + offset.y;
}

void Renderizador::deshacerTransform(Vector2& p) const {
    p.x = (p.x - offset.x - anchoPanel * 0.5f) / zoom + anchoPanel * 0.5f;
    p.y = (p.y - offset.y - altoPanel  * 0.5f) / zoom + altoPanel  * 0.5f;
}

void Renderizador::dibujarFlecha(Vector2 desde, Vector2 hasta, float radioDestino, Color color) {
    float dx = hasta.x - desde.x;
    float dy = hasta.y - desde.y;
    float dist = std::sqrt(dx*dx + dy*dy);
    if (dist < 0.001f) return;
    float ux = dx / dist;
    float uy = dy / dist;

    float finX = hasta.x - ux * radioDestino;
    float finY = hasta.y - uy * radioDestino;

    DrawLineEx(desde, { finX, finY }, 2.5f, color);

    float tam = 10.0f;
    float angBase = std::atan2(uy, ux);
    Vector2 p1 = { finX - tam * std::cos(angBase - PI/7),
                   finY - tam * std::sin(angBase - PI/7) };
    Vector2 p2 = { finX - tam * std::cos(angBase + PI/7),
                   finY - tam * std::sin(angBase + PI/7) };
    DrawTriangle({ finX, finY }, p1, p2, color);
}

void Renderizador::dibujarAristas(const Lista* ruta, bool dirigido) {
    for (int i = 0; i < numNodos; i++) {
        Arista* arista = grafo->getAristas(i);
        while (arista != nullptr) {
            int j = arista->destino;
            Vector2 p1 = nodosVisuales[i].posicion;
            Vector2 p2 = nodosVisuales[j].posicion;
            aplicarTransform(p1);
            aplicarTransform(p2);

            bool enRuta = (ruta != nullptr && ruta->contiene(i) && ruta->contiene(j));
            Color color = enRuta ? COLOR_ARISTA_RUTA : COLOR_ARISTA;
            float grosor = enRuta ? 3.5f : 2.0f;

            if (dirigido) {
                float radioJ = nodosVisuales[j].radio * zoom;
                dibujarFlecha(p1, p2, radioJ, color);
            } else {
                DrawLineEx(p1, p2, grosor, color);
            }

            Vector2 medio = { (p1.x + p2.x) / 2.0f, (p1.y + p2.y) / 2.0f };
            char pesoStr[12];
            std::sprintf(pesoStr, "%d", arista->peso);
            int anchoTxt = MeasureText(pesoStr, 18);
            DrawRectangle((int)medio.x - anchoTxt/2 - 4, (int)medio.y - 11,
                          anchoTxt + 8, 22, COLOR_PANEL_INTERNO);
            DrawText(pesoStr, (int)medio.x - anchoTxt/2, (int)medio.y - 9, 18, COLOR_DORADO);

            arista = arista->siguiente;
        }
    }
}

void Renderizador::dibujarNodos() {
    double t = GetTime() - tiempoInicio;
    for (int i = 0; i < numNodos; i++) {
        NodoVisual& nv = nodosVisuales[i];
        Vector2 p = nv.posicion;
        aplicarTransform(p);
        float r = nv.radio * zoom;

        if (i == nodoHover) {
            DrawCircle((int)p.x, (int)p.y, r + 6, ColorAlpha(COLOR_NODO_HOVER, 0.25f));
        }

        if (nv.seleccionado) {
            for (int k = 0; k < 3; k++) {
                float radioAnillo = r + 4 + k * 2;
                DrawCircleLines((int)p.x, (int)p.y, radioAnillo, COLOR_NODO_INICIO);
            }
        }

        float pulseExtra = 0.0f;
        if (i == nodoHover) {
            pulseExtra = 3.0f * std::sin((float)t * 4.0f);
        }

        DrawCircle((int)p.x, (int)p.y, r + pulseExtra, nv.color);
        DrawCircleLines((int)p.x, (int)p.y, r, COLOR_MARCO_BRILLO);
    }
}

void Renderizador::dibujarNombres() {
    for (int i = 0; i < numNodos; i++) {
        const char* nombre = grafo->getNombreNodo(i);
        if (nombre == nullptr) continue;

        Vector2 p = nodosVisuales[i].posicion;
        aplicarTransform(p);
        float r = nodosVisuales[i].radio * zoom;

        int tamFuente = 18;
        if (std::strlen(nombre) > 7) tamFuente = 15;
        if (std::strlen(nombre) > 12) tamFuente = 13;

        int anchoTexto = MeasureText(nombre, tamFuente);
        int tx = (int)p.x - anchoTexto / 2;
        int ty = (int)p.y - tamFuente / 2;

        if (r >= tamFuente * 1.1f) {
            DrawText(nombre, tx, ty, tamFuente, BLACK);
        } else {
            ty = (int)p.y + (int)r + 4;
            DrawRectangle(tx - 3, ty - 2, anchoTexto + 6, tamFuente + 4, COLOR_PANEL_INTERNO);
            DrawText(nombre, tx, ty, tamFuente, COLOR_TEXTO);
        }
    }
}

void Renderizador::dibujarLeyenda() {
    float x = 10;
    float y = 10;
    float w = 230;
    float h = 175;
    DrawRectangle((int)x, (int)y, (int)w, (int)h, COLOR_TOOLTIP_FONDO);
    DrawRectangleLinesEx({ x, y, w, h }, 2, COLOR_MARCO_BRILLO);

    DrawText("MAPA DEL TESORO", (int)(x + 8), (int)(y + 6), 14, COLOR_DORADO);
    float ly = y + 28;

    auto item = [&](Color c, const char* txt) {
        DrawCircle((int)(x + 18), (int)(ly + 8), 6, c);
        DrawText(txt, (int)(x + 32), (int)(ly + 2), 14, COLOR_TEXTO);
        ly += 20;
    };

    item(COLOR_NODO_NO_VIS,    "No visitado");
    item(COLOR_NODO_EN_COLA,   "En cola/pila");
    item(COLOR_NODO_VISITADO,  "Visitado");
    item(COLOR_NODO_RUTA,      "Ruta final");
    item(COLOR_NODO_SELECCION, "Seleccionado");
    item(COLOR_TESORO,         "Tesoro");

    ly = y + h + 8;
    DrawRectangle((int)x, (int)ly, (int)w, 50, COLOR_TOOLTIP_FONDO);
    DrawRectangleLinesEx({ x, ly, w, 50 }, 1, COLOR_MARCO);
    DrawText("Click izq: seleccionar", (int)(x + 8), (int)(ly + 5), 12, COLOR_TEXTO);
    DrawText("Click der: arrastrar mapa", (int)(x + 8), (int)(ly + 22), 12, COLOR_TEXTO);
    DrawText("Rueda: zoom", (int)(x + 8), (int)(ly + 39), 12, COLOR_TEXTO);
}

void Renderizador::dibujarTooltip(Vector2 mousePos, const Juego& juego) {
    if (nodoHover < 0) return;
    const char* nombre = grafo->getNombreNodo(nodoHover);
    if (nombre == nullptr) return;

    const char* pista = juego.getPistaNodo(nodoHover);
    char linea1[64];
    std::sprintf(linea1, "Ubicacion: %s", nombre);

    int w = 260;
    int h = pista ? 60 : 36;
    int tx = (int)mousePos.x + 18;
    int ty = (int)mousePos.y + 18;
    if (tx + w > (int)anchoPanel) tx = (int)mousePos.x - w - 10;
    if (ty + h > (int)altoPanel)  ty = (int)mousePos.y - h - 10;

    DrawRectangle(tx, ty, w, h, COLOR_TOOLTIP_FONDO);
    DrawRectangleLinesEx({ (float)tx, (float)ty, (float)w, (float)h }, 1, COLOR_DORADO);
    DrawText(linea1, tx + 6, ty + 4, 14, COLOR_DORADO);
    if (pista != nullptr) {
        DrawText(pista, tx + 6, ty + 24, 12, COLOR_TEXTO);
        DrawText("[click para seleccionar]", tx + 6, ty + 42, 10, COLOR_TEXTO_OSCURO);
    }
}

void Renderizador::actualizarInput(Vector2 mousePos) {
    nodoHover = nodoBajoMouse(mousePos);
}

void Renderizador::dibujarConEstado(Juego& juego) {
    sincronizarConJuego(juego);
    dibujarFondo();
    const Lista& ruta = juego.getRutaOptima();
    bool dirFlag = grafo->esDirigido();
    dibujarAristas(&ruta, dirFlag);
    dibujarNodos();
    dibujarNombres();
    dibujarLeyenda();
}

void Renderizador::sincronizarConJuego(Juego& juego) {
    for (int i = 0; i < numNodos; i++) {
        const char* nombre = grafo->getNombreNodo(i);
        bool esTesoro = (nombre != nullptr && std::strcmp(nombre, "tesoro") == 0);

        if (juego.getEnRuta(i)) {
            nodosVisuales[i].color = COLOR_NODO_RUTA;
        } else if (juego.getNodoProcesandoIdx() == i && juego.getVisitado(i)) {
            nodosVisuales[i].color = COLOR_NODO_PROCESANDO;
        } else if (juego.getVisitado(i)) {
            nodosVisuales[i].color = esTesoro ? COLOR_TESORO : COLOR_NODO_VISITADO;
        } else if (juego.getEnCola(i)) {
            nodosVisuales[i].color = COLOR_NODO_EN_COLA;
        } else if (esTesoro) {
            nodosVisuales[i].color = COLOR_TESORO;
        } else {
            nodosVisuales[i].color = COLOR_NODO_NO_VIS;
        }
    }
}

int Renderizador::nodoBajoMouse(Vector2 mousePos) const {
    Vector2 pWorld = mousePos;
    deshacerTransform(pWorld);
    for (int i = 0; i < numNodos; i++) {
        float dx = pWorld.x - nodosVisuales[i].posicion.x;
        float dy = pWorld.y - nodosVisuales[i].posicion.y;
        float dist = std::sqrt(dx * dx + dy * dy);
        if (dist <= nodosVisuales[i].radio + 8.0f) {
            return i;
        }
    }
    return -1;
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

bool Renderizador::iniciarArrastreNodo(Vector2 mousePos) {
    if (!modoEdicion) return false;
    int idx = nodoBajoMouse(mousePos);
    if (idx >= 0) {
        nodoArrastrando = idx;
        return true;
    }
    return false;
}

void Renderizador::actualizarArrastreNodo(Vector2 mousePos) {
    if (nodoArrastrando < 0) return;
    Vector2 p = mousePos;
    deshacerTransform(p);
    nodosVisuales[nodoArrastrando].posicion = p;
    float virtX = (p.x / anchoPanel) * 1000.0f;
    float virtY = (p.y / altoPanel) * 1000.0f;
    grafo->setCoordenada(nodoArrastrando, virtX, virtY);
}

void Renderizador::finalizarArrastreNodo() {
    nodoArrastrando = -1;
}
