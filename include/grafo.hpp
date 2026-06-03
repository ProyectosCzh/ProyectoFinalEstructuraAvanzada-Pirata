#ifndef GRAFO_HPP
#define GRAFO_HPP

struct Arista {
    int destino;
    int peso;
    Arista* siguiente;
};

struct NodoGrafo {
    char nombre[50];
    Arista* aristas;
};

class Grafo {
private:
    NodoGrafo* nodos;
    int numNodos;
    int capacidad;
    bool dirigido;

public:
    Grafo(int capacidad = 100);
    ~Grafo();
    int agregarNodo(const char* nombre);
    void agregarArista(int origen, int destino, int peso);
    int buscarNodo(const char* nombre) const;
    const char* getNombreNodo(int indice) const;
    Arista* getAristas(int nodo) const;
    int getNumNodos() const;
    void setDirigido(bool d) { dirigido = d; }
    bool esDirigido() const { return dirigido; }
};

#endif
