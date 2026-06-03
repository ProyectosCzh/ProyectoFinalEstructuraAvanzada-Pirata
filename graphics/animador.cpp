#include "animador.hpp"
#include "colores.hpp"
#include <cstdio>

Animador::Animador() {
    reproduciendo = false;
    velocidad = 500;
    tiempoUltimoPaso = 0;
    pasoActual = 0;
    pasosTotales = 0;
}

void Animador::iniciar(int totalPasos) {
    reproduciendo = true;
    pasoActual = 0;
    pasosTotales = totalPasos;
    tiempoUltimoPaso = GetTime();
}

void Animador::pausar() {
    reproduciendo = false;
}

void Animador::reanudar() {
    reproduciendo = true;
    tiempoUltimoPaso = GetTime();
}

void Animador::setVelocidadRelativa(int delta) {
    int nueva = velocidad + delta;
    if (nueva >= 50 && nueva <= 3000) {
        velocidad = nueva;
    }
}

bool Animador::actualizar() {
    if (!reproduciendo) return false;

    double ahora = GetTime();
    if ((ahora - tiempoUltimoPaso) * 1000.0 >= velocidad) {
        tiempoUltimoPaso = ahora;
        pasoActual++;
        if (pasoActual >= pasosTotales) {
            reproduciendo = false;
        }
        return true;
    }
    return false;
}

void Animador::dibujarControles(Rectangle area) {
    DrawRectangleRec(area, COLOR_PANEL_FONDO);
    DrawRectangleLinesEx(area, 2, COLOR_MARCO);

    float x = area.x + 8;
    float y = area.y + 6;

    const char* modo = reproduciendo ? "AUTO" : "MANUAL";
    const char* estado = reproduciendo ? "REPRODUCIENDO" : "PAUSADO";
    Color colModo = reproduciendo ? COLOR_NODO_VISITADO : COLOR_DORADO;
    DrawText(modo, (int)x, (int)y, 14, colModo);
    DrawText(estado, (int)(x + 50), (int)y, 14, COLOR_TEXTO);
    y += 20;

    char info[80];
    std::sprintf(info, "Paso %d / %d  -  Vel %dms", pasoActual, pasosTotales, velocidad);
    DrawText(info, (int)x, (int)y, 11, COLOR_TEXTO);
}
