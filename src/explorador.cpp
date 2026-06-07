#include "../include/explorador.hpp"
#include <cstdio>
#include <cstring>

Explorador::Explorador() {
    grafo = nullptr;
    colaRef = nullptr;
    pilaRef = nullptr;
}

void Explorador::configurar(Grafo* g, Cola* c, Pila* p) {
    grafo = g;
    colaRef = c;
    pilaRef = p;
}

bool Explorador::bfsPaso(int nodoInicio, int nodoDestino, int* padres,
                          int& numVisitados, bool* visitados,
                          int& frenteActual, int& tamFrente, int* colaLocal,
                          int paso) {
    if (grafo == nullptr || paso < 0) return false;
    int n = grafo->getNumNodos();

    if (paso == 0) {
        for (int i = 0; i < n; i++) {
            visitados[i] = false;
            padres[i] = -1;
        }
        visitados[nodoInicio] = true;
        colaLocal[0] = nodoInicio;
        frenteActual = 0;
        tamFrente = 1;
        numVisitados = 1;

        if (colaRef != nullptr) {
            colaRef->vaciar();
            colaRef->encolar(nodoInicio);
        }
        return false;
    }

    if (frenteActual >= tamFrente) return false;

    int actual = colaLocal[frenteActual];
    frenteActual++;

    if (actual == nodoDestino) {
        return true;
    }

    Arista* arista = grafo->getAristas(actual);
    while (arista != nullptr) {
        int vecino = arista->destino;
        if (!visitados[vecino]) {
            visitados[vecino] = true;
            padres[vecino] = actual;
            colaLocal[tamFrente] = vecino;
            tamFrente++;
            numVisitados++;

            if (colaRef != nullptr) {
                colaRef->encolar(vecino);
            }
        }
        arista = arista->siguiente;
    }

    return false;
}

bool Explorador::dfsPaso(int nodoInicio, int nodoDestino, int* padres,
                          int& numVisitados, bool* visitados,
                          int& topeLocal, int* pilaLocal,
                          int paso) {
    if (grafo == nullptr || paso < 0) return false;
    int n = grafo->getNumNodos();

    if (paso == 0) {
        for (int i = 0; i < n; i++) {
            visitados[i] = false;
            padres[i] = -1;
        }
        visitados[nodoInicio] = true;
        pilaLocal[0] = nodoInicio;
        topeLocal = 0;
        numVisitados = 1;

        if (pilaRef != nullptr) {
            pilaRef->vaciar();
            pilaRef->push(nodoInicio);
        }
        return false;
    }

    if (topeLocal < 0) return false;

    int actual = pilaLocal[topeLocal];

    if (actual == nodoDestino) {
        return true;  // Treasure found
    }

    // Find first unvisited neighbor and push it
    Arista* arista = grafo->getAristas(actual);
    while (arista != nullptr) {
        int vecino = arista->destino;
        if (!visitados[vecino]) {
            visitados[vecino] = true;
            padres[vecino] = actual;
            topeLocal++;
            pilaLocal[topeLocal] = vecino;
            numVisitados++;

            if (pilaRef != nullptr) {
                pilaRef->push(vecino);
            }
            return false;
        }
        arista = arista->siguiente;
    }

    topeLocal--;

    if (pilaRef != nullptr) {
        pilaRef->pop();
    }

    return false;
}

bool Explorador::dijkstraPaso(int nodoInicio, int nodoDestino, int* padres, int* distancias,
                              bool* visitados, int& pasoActual, int& uActual,
                              int* colaLocal, int& tamCola) {
    if (grafo == nullptr) return false;
    int n = grafo->getNumNodos();

    if (pasoActual == 0) {
        for (int i = 0; i < n; i++) {
            distancias[i] = 999999;
            padres[i] = -1;
            visitados[i] = false;
        }
        distancias[nodoInicio] = 0;
        colaLocal[0] = nodoInicio;
        tamCola = 1;
        uActual = -1;
        pasoActual = 1;
        return false;
    }

    int minDist = 999999;
    int u = -1;
    for (int i = 0; i < tamCola; i++) {
        int candidato = colaLocal[i];
        if (!visitados[candidato] && distancias[candidato] < minDist) {
            minDist = distancias[candidato];
            u = candidato;
        }
    }

    if (u == -1) return visitados[nodoDestino];

    uActual = u;
    visitados[u] = true;

    if (u == nodoDestino) {
        return true;
    }

    Arista* arista = grafo->getAristas(u);
    while (arista != nullptr) {
        int v = arista->destino;
        int peso = arista->peso;
        if (!visitados[v]) {
            if (distancias[u] + peso < distancias[v]) {
                distancias[v] = distancias[u] + peso;
                padres[v] = u;
            }
            bool yaEnCola = false;
            for (int i = 0; i < tamCola; i++) {
                if (colaLocal[i] == v) { yaEnCola = true; break; }
            }
            if (!yaEnCola) {
                colaLocal[tamCola] = v;
                tamCola++;
            }
        }
        arista = arista->siguiente;
    }

    return false;
}

void Explorador::reconstruirCamino(int destino, int* padres, Lista& camino) {
    camino.vaciar();

    int stack[MAX_NODOS];
    int tope = -1;
    int actual = destino;

    while (actual != -1) {
        tope++;
        stack[tope] = actual;
        actual = padres[actual];
    }

    while (tope >= 0) {
        camino.insertar(stack[tope]);
        tope--;
    }
}
