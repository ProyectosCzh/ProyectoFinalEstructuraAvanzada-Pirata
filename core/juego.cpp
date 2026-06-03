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
    dijkTamCola = 0;
    dijkUActual = -1;
    dijkPasoActual = 0;
    nodoProcesandoIdx = -1;
    nodoProcesandoNombre[0] = '\0';
    pistaActiva[0] = '\0';
    nodoPistaActual = -1;
    nodoPistaSiguiente = -1;
    algoritmoUsado[0] = '\0';
    for (int i = 0; i < 100; i++) {
        visitadosAnim[i] = false;
        enColaAnim[i] = false;
    }
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

    if (!cargarCoordenadas("data/coords.txt", grafo)) {
        printf("Sin coordenadas externas, se usara layout rejilla\n");
    }

    explorador.configurar(&grafo, &colaAnimacion, &pilaAnimacion);

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

void Juego::iniciarAlgoritmo(EstadoJuego nuevoEstado, const char* algoNombre) {
    if (nodoSeleccionado < 0) return;

    estado = nuevoEstado;
    nodoInicio = nodoSeleccionado;
    pasoActual = 0;
    std::strcpy(algoritmoUsado, algoNombre);

    int n = grafo.getNumNodos();
    for (int i = 0; i < n; i++) {
        visitadosAnim[i] = false;
        enColaAnim[i] = false;
        padres[i] = -1;
        expandidos[i] = false;
    }

    colaAnimacion.vaciar();
    pilaAnimacion.vaciar();
    nodoProcesandoIdx = -1;
    nodoProcesandoNombre[0] = '\0';
    pistaActiva[0] = '\0';

    printf("Iniciando %s desde nodo %d ('%s') hacia tesoro (%d)\n",
           algoNombre, nodoInicio, grafo.getNombreNodo(nodoInicio), nodoTesoro);
}

void Juego::iniciarBFS() {
    if (nodoSeleccionado < 0) return;
    iniciarAlgoritmo(EXPLORANDO_BFS, "BFS");
    frenteActual = 0;
    tamFrente = 0;
}

void Juego::iniciarDFS() {
    if (nodoSeleccionado < 0) return;
    iniciarAlgoritmo(EXPLORANDO_DFS, "DFS");
    topeLocal = -1;
}

void Juego::iniciarDijkstra() {
    if (nodoSeleccionado < 0) return;
    iniciarAlgoritmo(EXPLORANDO_DIJKSTRA, "Dijkstra");
    dijkPasoActual = 0;
    dijkTamCola = 0;
    dijkUActual = -1;
    dijkVisitCount = 0;
    int n = grafo.getNumNodos();
    for (int i = 0; i < n; i++) distanciasAnim[i] = 999999;
}

void Juego::iniciarNavegacionPistas() {
    if (nodoSeleccionado < 0) {
        printf("Seleccione un nodo primero\n");
        return;
    }

    estado = NAVEGANDO_PISTAS;
    nodoPistaActual = nodoSeleccionado;
    nodoPistaSiguiente = -1;
    pasoActual = 0;
    std::strcpy(algoritmoUsado, "Pistas");

    int n = grafo.getNumNodos();
    for (int i = 0; i < n; i++) {
        visitadosAnim[i] = false;
    }
    visitadosAnim[nodoPistaActual] = true;

    const char* nombre = grafo.getNombreNodo(nodoPistaActual);
    const char* pista = nombre ? pistas.obtener(nombre) : nullptr;
    if (pista != nullptr) {
        std::strncpy(pistaActiva, pista, 199);
        pistaActiva[199] = '\0';
    } else {
        pistaActiva[0] = '\0';
    }

    const char* destino = (pista != nullptr) ? arbolPistas.interpretar(pista) : nullptr;
    if (destino != nullptr) {
        nodoPistaSiguiente = grafo.buscarNodo(destino);
    }

    nodoProcesandoIdx = nodoPistaActual;
    std::strncpy(nodoProcesandoNombre, nombre ? nombre : "?", 63);
    nodoProcesandoNombre[63] = '\0';

    printf("Navegando por pistas desde '%s'...\n", nombre ? nombre : "?");
}

void Juego::pasoAnimacion() {
    if (estado == EXPLORANDO_BFS) {
        bool encontrado = explorador.bfsPaso(nodoInicio, nodoTesoro, padres,
                                              numVisitados, visitadosAnim,
                                              frenteActual, tamFrente, colaLocal,
                                              pasoActual);
        pasoActual++;

        int n = grafo.getNumNodos();
        for (int i = 0; i < n; i++) enColaAnim[i] = false;
        for (int i = frenteActual; i < tamFrente; i++) {
            int v = colaLocal[i];
            if (v >= 0 && v < n) enColaAnim[v] = true;
        }
        for (int i = 0; i < colaAnimacion.obtenerTam(); i++) {
            int v = colaAnimacion.getDatoEn(i);
            if (v >= 0 && v < n) enColaAnim[v] = true;
        }

        if (tamFrente > 0 && frenteActual > 0) {
            int proc = colaLocal[frenteActual - 1];
            if (proc >= 0 && proc < grafo.getNumNodos()) {
                nodoProcesandoIdx = proc;
                const char* nom = grafo.getNombreNodo(proc);
                std::strncpy(nodoProcesandoNombre, nom ? nom : "?", 63);
                nodoProcesandoNombre[63] = '\0';
                const char* p = nom ? pistas.obtener(nom) : nullptr;
                if (p != nullptr) {
                    std::strncpy(pistaActiva, p, 199);
                    pistaActiva[199] = '\0';
                }
            }
        } else if (tamFrente == 1 && frenteActual == 0) {
            int proc = colaLocal[0];
            if (proc >= 0 && proc < grafo.getNumNodos()) {
                nodoProcesandoIdx = proc;
                const char* nom = grafo.getNombreNodo(proc);
                std::strncpy(nodoProcesandoNombre, nom ? nom : "?", 63);
                nodoProcesandoNombre[63] = '\0';
            }
        }

        if (encontrado) {
            finalizarExploracion("BFS");
        }
    } else if (estado == EXPLORANDO_DFS) {
        bool encontrado = explorador.dfsPaso(nodoInicio, nodoTesoro, padres,
                                              numVisitados, visitadosAnim,
                                              topeLocal, pilaLocal, expandidos,
                                              pasoActual);
        pasoActual++;

        int n = grafo.getNumNodos();
        for (int i = 0; i < n; i++) enColaAnim[i] = false;
        for (int i = 0; i <= topeLocal; i++) {
            int v = pilaLocal[i];
            if (v >= 0 && v < n) enColaAnim[v] = true;
        }
        for (int i = 0; i < pilaAnimacion.obtenerTam(); i++) {
            int* datos = pilaAnimacion.getDatos();
            if (datos != nullptr) {
                int v = datos[i];
                if (v >= 0 && v < n) enColaAnim[v] = true;
            }
        }

        if (topeLocal >= 0) {
            int proc = pilaLocal[topeLocal];
            if (proc >= 0 && proc < grafo.getNumNodos()) {
                nodoProcesandoIdx = proc;
                const char* nom = grafo.getNombreNodo(proc);
                std::strncpy(nodoProcesandoNombre, nom ? nom : "?", 63);
                nodoProcesandoNombre[63] = '\0';
                const char* p = nom ? pistas.obtener(nom) : nullptr;
                if (p != nullptr) {
                    std::strncpy(pistaActiva, p, 199);
                    pistaActiva[199] = '\0';
                }
            }
        } else {
            nodoProcesandoIdx = -1;
        }

        if (encontrado) {
            finalizarExploracion("DFS");
        }
    } else if (estado == EXPLORANDO_DIJKSTRA) {
        bool encontrado = explorador.dijkstraPaso(nodoInicio, nodoTesoro,
                                                   padres, distanciasAnim,
                                                   visitadosAnim, dijkPasoActual,
                                                   dijkUActual, dijkColaLocal, dijkTamCola);
        pasoActual++;
        if (dijkUActual >= 0) dijkVisitCount++;

        int n = grafo.getNumNodos();
        for (int i = 0; i < n; i++) enColaAnim[i] = false;
        for (int i = 0; i < dijkTamCola; i++) {
            int v = dijkColaLocal[i];
            if (v >= 0 && v < n) enColaAnim[v] = true;
        }
        if (dijkUActual >= 0 && dijkUActual < n) enColaAnim[dijkUActual] = false;

        numVisitados = dijkVisitCount;

        if (dijkUActual >= 0 && dijkUActual < grafo.getNumNodos()) {
            nodoProcesandoIdx = dijkUActual;
            const char* nom = grafo.getNombreNodo(dijkUActual);
            std::strncpy(nodoProcesandoNombre, nom ? nom : "?", 63);
            nodoProcesandoNombre[63] = '\0';
        }

        if (encontrado) {
            costoTotal = distanciasAnim[nodoTesoro];
            rutaOptima.vaciar();
            explorador.reconstruirCamino(nodoTesoro, padres, rutaOptima);
            for (int i = 0; i < rutaOptima.longitud(); i++) {
                int nodo = rutaOptima.obtener(i);
                if (nodo >= 0 && nodo < grafo.getNumNodos()) {
                    visitadosAnim[nodo] = true;
                }
            }
            estado = COMPLETADO;
            nodoProcesandoIdx = -1;
            for (int i = 0; i < n; i++) enColaAnim[i] = false;
            printf("Dijkstra: Tesoro encontrado! Costo: %d\n", costoTotal);
        }
    } else if (estado == NAVEGANDO_PISTAS) {
        if (nodoPistaSiguiente < 0) {
            estado = COMPLETADO;
            return;
        }
        nodoPistaActual = nodoPistaSiguiente;
        nodoPistaSiguiente = -1;
        visitadosAnim[nodoPistaActual] = true;

        const char* nombre = grafo.getNombreNodo(nodoPistaActual);
        const char* pista = nombre ? pistas.obtener(nombre) : nullptr;
        if (pista != nullptr) {
            std::strncpy(pistaActiva, pista, 199);
            pistaActiva[199] = '\0';
        } else {
            pistaActiva[0] = '\0';
        }

        if (pista != nullptr && std::strcmp(pista, "!Lo has encontrado!") != 0) {
            const char* destino = arbolPistas.interpretar(pista);
            if (destino != nullptr) {
                nodoPistaSiguiente = grafo.buscarNodo(destino);
            }
        } else {
            nodoPistaSiguiente = -1;
        }

        nodoProcesandoIdx = nodoPistaActual;
        std::strncpy(nodoProcesandoNombre, nombre ? nombre : "?", 63);
        nodoProcesandoNombre[63] = '\0';
        pasoActual++;

        if (nodoPistaSiguiente < 0) {
            estado = COMPLETADO;
        }
    }
}

void Juego::finalizarExploracion(const char* algoNombre) {
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
    numVisitados = rutaOptima.longitud();
    nodoProcesandoIdx = -1;
    int n = grafo.getNumNodos();
    for (int i = 0; i < n; i++) enColaAnim[i] = false;
    printf("%s: Tesoro encontrado! Costo: %d\n", algoNombre, costoTotal);
}

void Juego::limpiar() {
    estado = INICIO;
    nodoSeleccionado = -1;
    nodoInicio = -1;
    costoTotal = 0;
    numVisitados = 0;
    pasoActual = 0;
    frenteActual = 0;
    tamFrente = 0;
    topeLocal = -1;
    colaAnimacion.vaciar();
    pilaAnimacion.vaciar();
    rutaOptima.vaciar();
    nodoProcesandoIdx = -1;
    nodoProcesandoNombre[0] = '\0';
    pistaActiva[0] = '\0';
    nodoPistaActual = -1;
    nodoPistaSiguiente = -1;
    dijkPasoActual = 0;
    dijkTamCola = 0;
    dijkUActual = -1;
    dijkVisitCount = 0;

    int n = grafo.getNumNodos();
    for (int i = 0; i < n; i++) {
        visitadosAnim[i] = false;
        enColaAnim[i] = false;
        expandidos[i] = false;
    }
}

bool Juego::guardarResultado() {
    return ::guardarResultado("data/resultado.txt", rutaOptima, grafo, algoritmoUsado, costoTotal, numVisitados);
}

bool Juego::guardarCoordenadas(const char* ruta) {
    return ::guardarCoordenadas(ruta, grafo);
}

const char* Juego::getPistaNodo(int indice) const {
    if (indice < 0 || indice >= grafo.getNumNodos()) return nullptr;
    const char* nombre = grafo.getNombreNodo(indice);
    return pistas.obtener(nombre);
}

bool Juego::getEnRuta(int indice) const {
    return rutaOptima.contiene(indice);
}
