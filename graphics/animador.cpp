#include "animador.hpp"
#include "colores.hpp"
#include <cstdio>

Animador::Animador() {
    reproduciendo = false;
    pasoAPaso = false;
    velocidad = 500;
    tiempoUltimoPaso = 0;
    pasoActual = 0;
    pasosTotales = 0;
}

void Animador::iniciar(int totalPasos) {
    reproduciendo = true;
    pasoAPaso = false;
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

void Animador::togglePasoAPaso() {
    pasoAPaso = !pasoAPaso;
    if (pasoAPaso) {
        reproduciendo = false;
    }
}

bool Animador::siguientePaso() {
    if (pasoActual < pasosTotales) {
        pasoActual++;
        return true;
    }
    return false;
}

void Animador::setVelocidad(int ms) {
    if (ms >= 50 && ms <= 3000) {
        velocidad = ms;
    }
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

    const char* titulo = reproduciendo ? ">> REPRODUCIENDO" :
                         pasoAPaso     ? "|| PASO A PASO" :
                                         "-- PAUSADO";
    Color colTit = reproduciendo ? COLOR_NODO_VISITADO : COLOR_DORADO;
    DrawText(titulo, (int)x, (int)y, 13, colTit);
    y += 20;

    char info[80];
    std::sprintf(info, "Paso %d / %d  -  Vel %dms", pasoActual, pasosTotales, velocidad);
    DrawText(info, (int)x, (int)y, 11, COLOR_TEXTO);
}
