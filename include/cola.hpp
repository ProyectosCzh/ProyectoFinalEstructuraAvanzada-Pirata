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
    bool estaLlena() const;
    int obtenerTam() const;
    int getDatoEn(int indice) const;
    void vaciar();
};

#endif
