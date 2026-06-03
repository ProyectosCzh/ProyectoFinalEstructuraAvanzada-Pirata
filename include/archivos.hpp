#ifndef ARCHIVOS_HPP
#define ARCHIVOS_HPP

#include "grafo.hpp"
#include "diccionario.hpp"
#include "arbol.hpp"
#include "lista.hpp"

bool cargarGrafo(const char* ruta, Grafo& grafo);
bool cargarPistas(const char* ruta, Diccionario& dicc);
bool cargarArbolPistas(const char* ruta, Arbol& arbol);
bool guardarResultado(const char* ruta, const Lista& camino, const Grafo& grafo, const char* algoritmo, int costoTotal, int visitados);
bool cargarCoordenadas(const char* ruta, Grafo& grafo);
bool guardarCoordenadas(const char* ruta, const Grafo& grafo);

#endif
