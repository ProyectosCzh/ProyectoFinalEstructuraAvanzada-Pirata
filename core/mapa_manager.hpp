#ifndef MAPA_MANAGER_HPP
#define MAPA_MANAGER_HPP

struct InfoMapa {
    char dirName[30];
    char nombre[60];
};

class MapaManager {
private:
    InfoMapa mapas[20];
    int totalMapas;
    int actual;

    void derivarNombre(const char* dirName, char* out);

public:
    MapaManager();
    bool escanearMapas();
    int getTotal() const { return totalMapas; }
    int getActual() const { return actual; }
    bool setActual(int idx);
    const InfoMapa& getMapa(int idx) const;
    const InfoMapa& getActualMapa() const;
    void getRutaBase(char* out) const;
    void getRutaGrafo(char* out) const;
    void getRutaPistas(char* out) const;
    void getRutaCoords(char* out) const;
    void getRutaFondo(char* out) const;
    void getRutaResultado(char* out) const;
};

#endif
