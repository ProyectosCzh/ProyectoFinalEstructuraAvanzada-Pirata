#ifndef EXPLORADOR_HPP
#define EXPLORADOR_HPP

#include "grafo.hpp"
#include "cola.hpp"
#include "pila.hpp"
#include "lista.hpp"

class Explorador {
private:
    Grafo* grafo;
    Cola* colaRef;
    Pila* pilaRef;

public:
    Explorador();
    void configurar(Grafo* g, Cola* c, Pila* p);

    bool bfsPaso(int nodoInicio, int nodoDestino, int* padres,
                 int& numVisitados, bool* visitados,
                 int& frenteActual, int& tamFrente, int* colaLocal,
                 int paso);

    bool dfsPaso(int nodoInicio, int nodoDestino, int* padres,
                 int& numVisitados, bool* visitados,
                 int& topeLocal, int* pilaLocal,
                 int paso);

    bool dijkstraPaso(int nodoInicio, int nodoDestino, int* padres, int* distancias,
                      bool* visitados, int& pasoActual, int& uActual,
                      int* colaLocal, int& tamCola);

    void reconstruirCamino(int destino, int* padres, Lista& camino);
};

#endif
