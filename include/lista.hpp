#ifndef LISTA_HPP
#define LISTA_HPP

struct NodoLista {
    int dato;
    NodoLista* siguiente;
};

class Lista {
private:
    NodoLista* cabeza;
    NodoLista* cola;
    int tam;

public:
    Lista();
    ~Lista();
    void insertar(int dato);
    bool contiene(int dato) const;
    int obtener(int indice) const;
    int longitud() const;
    void vaciar();
};

#endif
