#include "../include/explorador.hpp"
#include <cstdio>
#include <cstring>

Explorador::Explorador() {
    grafo = nullptr;
    pistas = nullptr;
    arbolPistas = nullptr;
    colaRef = nullptr;
    pilaRef = nullptr;
    rutaRef = nullptr;
}

void Explorador::configurar(Grafo* g, Diccionario* d, Arbol* a, Cola* c, Pila* p, Lista* r) {
    grafo = g;
    pistas = d;
    arbolPistas = a;
    colaRef = c;
    pilaRef = p;
    rutaRef = r;
}

bool Explorador::bfs(int nodoInicio, int nodoDestino, int* padres, int& numVisitados, bool* visitados) {
    if (grafo == nullptr) return false;

    int n = grafo->getNumNodos();
    for (int i = 0; i < n; i++) {
        visitados[i] = false;
        padres[i] = -1;
    }

    Cola colaLocal(n);
    visitados[nodoInicio] = true;
    colaLocal.encolar(nodoInicio);
    numVisitados = 1;

    if (colaRef != nullptr) {
        colaRef->vaciar();
        colaRef->encolar(nodoInicio);
    }

    while (!colaLocal.estaVacia()) {
        int actual = colaLocal.desencolar();

        if (actual == nodoDestino) {
            return true;
        }

        Arista* arista = grafo->getAristas(actual);
        while (arista != nullptr) {
            int vecino = arista->destino;
            if (!visitados[vecino]) {
                visitados[vecino] = true;
                padres[vecino] = actual;
                colaLocal.encolar(vecino);
                numVisitados++;

                if (colaRef != nullptr) {
                    colaRef->encolar(vecino);
                }
            }
            arista = arista->siguiente;
        }
    }

    return false;
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

bool Explorador::dfs(int nodoInicio, int nodoDestino, int* padres, int& numVisitados, bool* visitados) {
    if (grafo == nullptr) return false;

    int n = grafo->getNumNodos();
    for (int i = 0; i < n; i++) {
        visitados[i] = false;
        padres[i] = -1;
    }

    Pila pilaLocal(n);
    visitados[nodoInicio] = true;
    pilaLocal.push(nodoInicio);
    numVisitados = 1;

    if (pilaRef != nullptr) {
        pilaRef->vaciar();
        pilaRef->push(nodoInicio);
    }

    bool* expandidos = new bool[n]();

    while (!pilaLocal.estaVacia()) {
        int actual = pilaLocal.top();

        if (actual == nodoDestino) {
            delete[] expandidos;
            return true;
        }

        if (!expandidos[actual]) {
            expandidos[actual] = true;
        }

        Arista* arista = grafo->getAristas(actual);
        bool encontradoVecino = false;

        while (arista != nullptr) {
            int vecino = arista->destino;
            if (!visitados[vecino]) {
                visitados[vecino] = true;
                padres[vecino] = actual;
                pilaLocal.push(vecino);
                numVisitados++;
                encontradoVecino = true;

                if (pilaRef != nullptr) {
                    pilaRef->push(vecino);
                }
                break;
            }
            arista = arista->siguiente;
        }

        if (!encontradoVecino) {
            pilaLocal.pop();
        }
    }

    delete[] expandidos;
    return false;
}

bool Explorador::dfsPaso(int nodoInicio, int nodoDestino, int* padres,
                          int& numVisitados, bool* visitados,
                          int& topeLocal, int* pilaLocal, bool* expandidos,
                          int paso) {
    if (grafo == nullptr || paso < 0) return false;
    int n = grafo->getNumNodos();

    if (paso == 0) {
        for (int i = 0; i < n; i++) {
            visitados[i] = false;
            padres[i] = -1;
            expandidos[i] = false;
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
        return true;
    }

    if (!expandidos[actual]) {
        expandidos[actual] = true;
    }

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

int Explorador::dijkstra(int nodoInicio, int nodoDestino, int* padres, int* distancias) {
    if (grafo == nullptr) return -1;

    int n = grafo->getNumNodos();
    bool* visitados = new bool[n]();

    for (int i = 0; i < n; i++) {
        distancias[i] = 999999;
        padres[i] = -1;
        visitados[i] = false;
    }

    distancias[nodoInicio] = 0;

    for (int count = 0; count < n - 1; count++) {
        int minDist = 999999;
        int u = -1;

        for (int i = 0; i < n; i++) {
            if (!visitados[i] && distancias[i] < minDist) {
                minDist = distancias[i];
                u = i;
            }
        }

        if (u == -1 || u == nodoDestino) break;

        visitados[u] = true;

        Arista* arista = grafo->getAristas(u);
        while (arista != nullptr) {
            int v = arista->destino;
            int peso = arista->peso;
            if (!visitados[v] && distancias[u] + peso < distancias[v]) {
                distancias[v] = distancias[u] + peso;
                padres[v] = u;
            }
            arista = arista->siguiente;
        }
    }

    int costo = distancias[nodoDestino];
    delete[] visitados;

    if (costo >= 999999) return -1;
    return costo;
}

int Explorador::navegarPorPistas(int nodoInicio, int* caminoPistas, int& numPasos) {
    if (grafo == nullptr || pistas == nullptr || arbolPistas == nullptr) return -1;

    int nodoActual = nodoInicio;
    numPasos = 0;

    while (nodoActual >= 0 && numPasos < grafo->getNumNodos()) {
        caminoPistas[numPasos] = nodoActual;
        numPasos++;

        const char* nombreNodo = grafo->getNombreNodo(nodoActual);
        if (nombreNodo == nullptr) break;

        const char* pista = pistas->obtener(nombreNodo);
        if (pista == nullptr) break;

        if (std::strcmp(pista, "!Lo has encontrado!") == 0) {
            return nodoActual;
        }

        const char* destinoStr = arbolPistas->interpretar(pista);
        if (destinoStr == nullptr) break;

        nodoActual = grafo->buscarNodo(destinoStr);
        if (nodoActual < 0) break;
    }

    return nodoActual;
}

void Explorador::reconstruirCamino(int destino, int* padres, Lista& camino) {
    camino.vaciar();

    int stack[100];
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
