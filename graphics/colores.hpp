#ifndef COLORES_HPP
#define COLORES_HPP

#include "raylib.h"

const Color COLOR_FONDO           = { 24,  35,  60, 255};
const Color COLOR_FONDO_SUPERIOR  = { 35,  50,  85, 255};
const Color COLOR_FONDO_INFERIOR  = { 10,  15,  30, 255};

const Color COLOR_NODO_NO_VIS     = {130, 130, 150, 255};
const Color COLOR_NODO_EN_COLA    = {255, 220,  60, 255};
const Color COLOR_NODO_VISITADO   = { 80, 200, 120, 255};
const Color COLOR_NODO_RUTA       = {220,  60,  60, 255};
const Color COLOR_NODO_SELECCION  = { 80, 150, 255, 255};
const Color COLOR_NODO_HOVER      = {180, 220, 255, 255};
const Color COLOR_NODO_INICIO     = {255, 255, 255, 255};

const Color COLOR_TEXTO           = {240, 240, 250, 255};
const Color COLOR_TEXTO_OSCURO    = {180, 190, 210, 255};
const Color COLOR_TEXTO_SOMBRA    = { 10,  10,  20, 200};

const Color COLOR_ARISTA          = { 90, 110, 140, 255};
const Color COLOR_ARISTA_RUTA     = {255, 100, 100, 255};
const Color COLOR_TESORO          = {255, 215,   0, 255};
const Color COLOR_TESORO_BRILLO   = {255, 240, 120, 255};

const Color COLOR_DORADO          = {255, 200,  60, 255};
const Color COLOR_MARCO           = {120,  90,  40, 255};
const Color COLOR_MARCO_BRILLO    = {200, 150,  60, 255};
const Color COLOR_PANEL_FONDO     = { 20,  28,  50, 255};
const Color COLOR_PANEL_INTERNO   = { 30,  40,  70, 255};

const Color COLOR_BOTON           = { 60,  60,  90, 255};
const Color COLOR_BOTON_HOVER     = { 80,  80, 120, 255};

const Color COLOR_ARBOL_LINEA     = {150, 150, 180, 255};
const Color COLOR_ARBOL_ACTIVO    = {255, 200,  60, 255};

const Color COLOR_COLA_BARRA      = {255, 200,   0, 255};
const Color COLOR_PILA_BARRA      = {200, 100,   0, 255};

const Color COLOR_TOOLTIP_FONDO   = { 10,  10,  20, 230};

#endif
