#ifndef PILA_HPP
#define PILA_HPP

class Pila {
private:
    int* datos;
    int tope;
    int capacidad;

public:
    Pila(int capacidad = 100);
    ~Pila();
    void push(int valor);
    int pop();
    int top() const;
    bool estaVacia() const;
    int obtenerTam() const;
    void vaciar();
    int* getDatos() const { return datos; }
};

#endif
