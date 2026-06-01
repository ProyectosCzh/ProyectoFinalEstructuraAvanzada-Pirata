#ifndef JUEGO_HPP
#define JUEGO_HPP

#include "../include/grafo.hpp"
#include "../include/diccionario.hpp"
#include "../include/arbol.hpp"
#include "../include/cola.hpp"
#include "../include/pila.hpp"
#include "../include/lista.hpp"
#include "../include/explorador.hpp"

enum EstadoJuego {
    INICIO,
    EXPLORANDO_BFS,
    EXPLORANDO_DFS,
    RUTA_OPTIMA,
    COMPLETADO,
    PAUSADO
};

class Juego {
private:
    Grafo grafo;
    Diccionario pistas;
    Arbol arbolPistas;
    Cola colaAnimacion;
    Pila pilaAnimacion;
    Lista rutaOptima;
    Explorador explorador;

    EstadoJuego estado;
    int nodoSeleccionado;
    int nodoInicio;
    int nodoTesoro;
    int costoTotal;
    int numVisitados;
    char algoritmoUsado[50];

    int padres[100];
    bool visitadosAnim[100];
    int distanciasAnim[100];
    int colaLocal[100];
    int pilaLocal[100];
    bool expandidos[100];
    int frenteActual;
    int tamFrente;
    int topeLocal;
    int pasoActual;

    bool bfsEncontrado;

public:
    Juego();
    ~Juego();

    bool cargarDatos();
    void seleccionarNodo(int indice);
    void iniciarBFS();
    void iniciarDFS();
    void iniciarDijkstra();
    void navegarPorPistas();
    void pasoAnimacion();
    void limpiar();
    bool guardarResultado();

    EstadoJuego getEstado() const { return estado; }
    int getNodoSeleccionado() const { return nodoSeleccionado; }
    int getNodoInicio() const { return nodoInicio; }
    int getCostoTotal() const { return costoTotal; }
    int getNumVisitados() const { return numVisitados; }
    int getPasoActual() const { return pasoActual; }
    const char* getAlgoritmoUsado() const { return algoritmoUsado; }
    const char* getPistaNodo(int indice) const;
    bool getVisitado(int indice) const { return visitadosAnim[indice]; }
    bool getEnRuta(int indice) const;

    Grafo& getGrafo() { return grafo; }
    Arbol& getArbolPistas() { return arbolPistas; }
    Cola& getColaAnimacion() { return colaAnimacion; }
    Pila& getPilaAnimacion() { return pilaAnimacion; }
    Lista& getRutaOptima() { return rutaOptima; }
};

#endif
