#ifndef ANIMADOR_HPP
#define ANIMADOR_HPP

#include "raylib.h"

class Animador {
private:
    bool reproduciendo;
    int velocidad;
    double tiempoUltimoPaso;
    int pasoActual;
    int pasosTotales;

public:
    Animador();
    void iniciar(int totalPasos);
    void pausar();
    void reanudar();
    void setVelocidadRelativa(int delta);
    bool actualizar();
    bool estaReproduciendo() const { return reproduciendo; }
    void dibujarControles(Rectangle area);
};

#endif
