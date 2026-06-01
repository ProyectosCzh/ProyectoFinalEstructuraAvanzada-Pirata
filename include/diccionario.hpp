#ifndef DICCIONARIO_HPP
#define DICCIONARIO_HPP

struct ParClaveValor {
    char clave[50];
    char valor[200];
    ParClaveValor* siguiente;
};

class Diccionario {
private:
    static const int TAM_TABLA = 101;
    ParClaveValor* tabla[TAM_TABLA];

    int hash(const char* clave) const;

public:
    Diccionario();
    ~Diccionario();
    void insertar(const char* clave, const char* valor);
    const char* obtener(const char* clave) const;
    bool contiene(const char* clave) const;
    void eliminar(const char* clave);
};

#endif
