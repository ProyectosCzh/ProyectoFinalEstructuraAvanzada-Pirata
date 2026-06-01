#ifndef EXPLORADOR_HPP
#define EXPLORADOR_HPP

#include "grafo.hpp"
#include "diccionario.hpp"
#include "arbol.hpp"
#include "cola.hpp"
#include "pila.hpp"
#include "lista.hpp"

class Explorador {
private:
    Grafo* grafo;
    Diccionario* pistas;
    Arbol* arbolPistas;
    Cola* colaRef;
    Pila* pilaRef;
    Lista* rutaRef;

public:
    Explorador();
    void configurar(Grafo* g, Diccionario* d, Arbol* a, Cola* c, Pila* p, Lista* r);

    bool bfs(int nodoInicio, int nodoDestino, int* padres, int& numVisitados, bool* visitados);
    bool bfsPaso(int nodoInicio, int nodoDestino, int* padres,
                 int& numVisitados, bool* visitados,
                 int& frenteActual, int& tamFrente, int* colaLocal,
                 int paso);

    bool dfs(int nodoInicio, int nodoDestino, int* padres, int& numVisitados, bool* visitados);
    bool dfsPaso(int nodoInicio, int nodoDestino, int* padres,
                 int& numVisitados, bool* visitados,
                 int& topeLocal, int* pilaLocal, bool* expandidos,
                 int paso);

    int dijkstra(int nodoInicio, int nodoDestino, int* padres, int* distancias);

    int navegarPorPistas(int nodoInicio, int* caminoPistas, int& numPasos);

    void reconstruirCamino(int destino, int* padres, Lista& camino);
};

#endif
