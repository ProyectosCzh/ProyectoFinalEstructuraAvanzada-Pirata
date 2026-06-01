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
    DrawRectangleRec(area, { 20, 20, 40, 200 });
    DrawRectangleLinesEx(area, 1, DARKGRAY);

    float x = area.x + 5;
    float y = area.y + 5;
    float btnAncho = 60;
    float btnAlto = 25;
    float espaciado = 30;

    const char* playLabel = reproduciendo ? "Pausa" : "Play";
    Rectangle btnPlay = { x, y, btnAncho, btnAlto };
    DrawRectangleRec(btnPlay, COLOR_BOTON);
    DrawRectangleLinesEx(btnPlay, 1, DARKGRAY);
    DrawText(playLabel, (int)x + 10, (int)y + 5, 12, COLOR_TEXTO);

    y += espaciado;
    Rectangle btnStep = { x, y, btnAncho, btnAlto };
    DrawRectangleRec(btnStep, COLOR_BOTON);
    DrawRectangleLinesEx(btnStep, 1, DARKGRAY);
    DrawText("Paso", (int)x + 12, (int)y + 5, 12, COLOR_TEXTO);

    y += espaciado;
    Rectangle btnMas = { x, y, btnAncho / 2 - 2, btnAlto };
    DrawRectangleRec(btnMas, COLOR_BOTON);
    DrawRectangleLinesEx(btnMas, 1, DARKGRAY);
    DrawText("+", (int)x + btnAncho / 4 - 4, (int)y + 3, 14, COLOR_TEXTO);

    Rectangle btnMenos = { x + btnAncho / 2 + 2, y, btnAncho / 2 - 2, btnAlto };
    DrawRectangleRec(btnMenos, COLOR_BOTON);
    DrawRectangleLinesEx(btnMenos, 1, DARKGRAY);
    DrawText("-", (int)x + btnAncho / 2 + btnAncho / 4 - 4, (int)y + 3, 14, COLOR_TEXTO);

    y += espaciado;
    char velStr[20];
    std::sprintf(velStr, "Vel: %dms", velocidad);
    DrawText(velStr, (int)x, (int)y, 11, COLOR_TEXTO_OSCURO);
}
