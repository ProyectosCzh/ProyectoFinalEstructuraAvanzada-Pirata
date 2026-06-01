#include "juego.hpp"
#include "../include/archivos.hpp"
#include <cstring>
#include <cstdio>

Juego::Juego()
    : colaAnimacion(100), pilaAnimacion(100) {
    estado = INICIO;
    nodoSeleccionado = -1;
    nodoInicio = -1;
    nodoTesoro = -1;
    costoTotal = 0;
    numVisitados = 0;
    pasoActual = 0;
    frenteActual = 0;
    tamFrente = 0;
    topeLocal = -1;
    bfsEncontrado = false;
    algoritmoUsado[0] = '\0';
}

Juego::~Juego() {
}

bool Juego::cargarDatos() {
    bool ok = true;

    if (!cargarGrafo("data/grafo.txt", grafo)) {
        printf("Error cargando grafo\n");
        ok = false;
    }

    if (!cargarPistas("data/pistas.txt", pistas)) {
        printf("Error cargando pistas\n");
        ok = false;
    }

    if (!cargarArbolPistas("data/pistas.txt", arbolPistas)) {
        printf("Error cargando arbol de pistas\n");
        ok = false;
    }

    explorador.configurar(&grafo, &pistas, &arbolPistas,
                          &colaAnimacion, &pilaAnimacion, &rutaOptima);

    nodoTesoro = grafo.buscarNodo("tesoro");

    if (nodoTesoro >= 0) {
        printf("Tesoro encontrado en nodo: %d ('%s')\n", nodoTesoro, grafo.getNombreNodo(nodoTesoro));
    }

    return ok;
}

void Juego::seleccionarNodo(int indice) {
    if (indice >= 0 && indice < grafo.getNumNodos()) {
        nodoSeleccionado = indice;
        const char* nombre = grafo.getNombreNodo(indice);
        const char* pista = pistas.obtener(nombre);
        printf("Nodo seleccionado: %d ('%s') - Pista: %s\n", indice, nombre, pista ? pista : "(sin pista)");
    }
}

void Juego::iniciarBFS() {
    if (nodoSeleccionado < 0) {
        printf("Seleccione un nodo primero\n");
        return;
    }

    estado = EXPLORANDO_BFS;
    nodoInicio = nodoSeleccionado;
    pasoActual = 0;
    frenteActual = 0;
    tamFrente = 0;
    bfsEncontrado = false;
    std::strcpy(algoritmoUsado, "BFS");

    int n = grafo.getNumNodos();
    for (int i = 0; i < n; i++) {
        visitadosAnim[i] = false;
        padres[i] = -1;
    }

    printf("Iniciando BFS desde nodo %d ('%s') hacia tesoro (%d)\n",
           nodoInicio, grafo.getNombreNodo(nodoInicio), nodoTesoro);
}

void Juego::iniciarDFS() {
    if (nodoSeleccionado < 0) {
        printf("Seleccione un nodo primero\n");
        return;
    }

    estado = EXPLORANDO_DFS;
    nodoInicio = nodoSeleccionado;
    pasoActual = 0;
    topeLocal = -1;
    std::strcpy(algoritmoUsado, "DFS");

    int n = grafo.getNumNodos();
    for (int i = 0; i < n; i++) {
        visitadosAnim[i] = false;
        padres[i] = -1;
        expandidos[i] = false;
    }

    printf("Iniciando DFS desde nodo %d ('%s') hacia tesoro (%d)\n",
           nodoInicio, grafo.getNombreNodo(nodoInicio), nodoTesoro);
}

void Juego::iniciarDijkstra() {
    if (nodoSeleccionado < 0) {
        printf("Seleccione un nodo primero\n");
        return;
    }

    int n = grafo.getNumNodos();
    for (int i = 0; i < n; i++) {
        visitadosAnim[i] = false;
    }

    costoTotal = explorador.dijkstra(nodoSeleccionado, nodoTesoro, padres, distanciasAnim);

    if (costoTotal < 0) {
        printf("No se encontro camino al tesoro\n");
        return;
    }

    rutaOptima.vaciar();
    explorador.reconstruirCamino(nodoTesoro, padres, rutaOptima);

    for (int i = 0; i < rutaOptima.longitud(); i++) {
        int nodo = rutaOptima.obtener(i);
        if (nodo >= 0 && nodo < n) {
            visitadosAnim[nodo] = true;
        }
    }

    estado = RUTA_OPTIMA;
    numVisitados = rutaOptima.longitud();
    std::strcpy(algoritmoUsado, "Dijkstra");

    printf("Dijkstra completo. Costo: %d, Ruta: %d nodos\n", costoTotal, rutaOptima.longitud());
}

void Juego::navegarPorPistas() {
    if (nodoSeleccionado < 0) {
        printf("Seleccione un nodo primero\n");
        return;
    }

    printf("Navegando por pistas desde '%s'...\n", grafo.getNombreNodo(nodoSeleccionado));
}

void Juego::pasoAnimacion() {
    if (estado == EXPLORANDO_BFS) {
        bool encontrado = explorador.bfsPaso(nodoInicio, nodoTesoro, padres,
                                              numVisitados, visitadosAnim,
                                              frenteActual, tamFrente, colaLocal,
                                              pasoActual);
        pasoActual++;

        if (encontrado) {
            rutaOptima.vaciar();
            explorador.reconstruirCamino(nodoTesoro, padres, rutaOptima);
            costoTotal = 0;
            for (int i = 0; i < rutaOptima.longitud() - 1; i++) {
                int a = rutaOptima.obtener(i);
                int b = rutaOptima.obtener(i + 1);
                Arista* arista = grafo.getAristas(a);
                while (arista != nullptr) {
                    if (arista->destino == b) {
                        costoTotal += arista->peso;
                        break;
                    }
                    arista = arista->siguiente;
                }
            }
            estado = COMPLETADO;
            printf("BFS: Tesoro encontrado! Costo: %d\n", costoTotal);
        }
    } else if (estado == EXPLORANDO_DFS) {
        bool encontrado = explorador.dfsPaso(nodoInicio, nodoTesoro, padres,
                                              numVisitados, visitadosAnim,
                                              topeLocal, pilaLocal, expandidos,
                                              pasoActual);
        pasoActual++;

        if (encontrado) {
            rutaOptima.vaciar();
            explorador.reconstruirCamino(nodoTesoro, padres, rutaOptima);
            costoTotal = 0;
            for (int i = 0; i < rutaOptima.longitud() - 1; i++) {
                int a = rutaOptima.obtener(i);
                int b = rutaOptima.obtener(i + 1);
                Arista* arista = grafo.getAristas(a);
                while (arista != nullptr) {
                    if (arista->destino == b) {
                        costoTotal += arista->peso;
                        break;
                    }
                    arista = arista->siguiente;
                }
            }
            estado = COMPLETADO;
            printf("DFS: Tesoro encontrado! Costo: %d\n", costoTotal);
        }
    }
}

void Juego::limpiar() {
    estado = INICIO;
    nodoSeleccionado = -1;
    nodoInicio = -1;
    costoTotal = 0;
    numVisitados = 0;
    pasoActual = 0;
    colaAnimacion.vaciar();
    pilaAnimacion.vaciar();
    rutaOptima.vaciar();

    int n = grafo.getNumNodos();
    for (int i = 0; i < n; i++) {
        visitadosAnim[i] = false;
    }
}

bool Juego::guardarResultado() {
    return ::guardarResultado("data/resultado.txt", rutaOptima, grafo, algoritmoUsado, costoTotal, numVisitados);
}

const char* Juego::getPistaNodo(int indice) const {
    if (indice < 0 || indice >= grafo.getNumNodos()) return nullptr;
    const char* nombre = grafo.getNombreNodo(indice);
    return pistas.obtener(nombre);
}

bool Juego::getEnRuta(int indice) const {
    return rutaOptima.contiene(indice);
}
