#ifndef ARBOL_HPP
#define ARBOL_HPP

struct NodoArbol {
    char pista[200];
    char destino[50];
    NodoArbol** hijos;
    int numHijos;
};

class Arbol {
private:
    NodoArbol* raiz;

    void limpiarRecursivo(NodoArbol* nodo);
    NodoArbol* buscarNodoRecursivo(NodoArbol* nodo, const char* pista) const;

public:
    Arbol();
    ~Arbol();
    void crearRaiz(const char* pista, const char* destino);
    NodoArbol* agregarHijo(NodoArbol* padre, const char* pista, const char* destino);
    NodoArbol* getRaiz() const;
    const char* interpretar(const char* pista) const;
    void limpiar();
};

#endif
