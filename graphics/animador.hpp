#ifndef ANIMADOR_HPP
#define ANIMADOR_HPP

#include "raylib.h"

class Animador {
private:
    bool reproduciendo;
    bool pasoAPaso;
    int velocidad;
    double tiempoUltimoPaso;
    int pasoActual;
    int pasosTotales;

public:
    Animador();
    void iniciar(int totalPasos);
    void pausar();
    void reanudar();
    void togglePasoAPaso();
    bool siguientePaso();
    void setVelocidad(int ms);
    void setVelocidadRelativa(int delta);
    bool actualizar();
    bool estaReproduciendo() const { return reproduciendo; }
    int getPasoActual() const { return pasoActual; }
    void dibujarControles(Rectangle area);
};

#endif
