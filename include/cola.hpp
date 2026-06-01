#ifndef COLA_HPP
#define COLA_HPP

class Cola {
private:
    int* datos;
    int frente;
    int final;
    int capacidad;
    int tam;

public:
    Cola(int capacidad = 100);
    ~Cola();
    void encolar(int valor);
    int desencolar();
    int peek() const;
    bool estaVacia() const;
    bool estaLlena() const;
    int obtenerTam() const;
    int getFrente() const { return frente; }
    int getFinal() const { return final; }
    int getCapacidad() const { return capacidad; }
    int getDatoEn(int indice) const;
    void vaciar();
};

#endif
