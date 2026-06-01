# El Tesoro del Pirata

Proyecto académico de Estructuras de Datos y Algoritmos en C++17 con representacion grafica en vivo usando raylib 5.x bajo MinGW-w64 (MSYS2).

## Descripcion

Ayudar a un pirata a encontrar un tesoro escondido en una isla. El mapa es un **grafo** donde los nodos son ubicaciones y las aristas son caminos con costo. Cada ubicacion tiene **pistas** que guian al tesoro. El programa explora la isla usando **BFS/DFS**, calcula la ruta optima con **Dijkstra**, interpreta pistas mediante un **arbol de decision**, y visualiza todo graficamente en vivo con raylib.

Todas las estructuras de datos fueron implementadas **manualmente sin STL** como requisito academico.

---

## Estructura del proyecto

```
tesoropirata/
├── include/         7 headers   ─── structs, clases, prototipos
├── src/             8 .cpp      ─── implementaciones de estructuras y algoritmos
├── graphics/        9 archivos  ─── capa de renderizado con raylib
├── core/            4 archivos  ─── logica de orquestacion del juego
├── data/            3 archivos  ─── entrada (grafo, pistas) y salida (resultado)
├── main.cpp                     ─── entry point + game loop
├── Makefile                     ─── compilacion con g++
└── README.md                    ─── este archivo
```

---

## include/ — Headers de estructuras de datos

### lista.hpp — Lista doblemente enlazada

```cpp
struct NodoLista {
    int dato;                // Valor almacenado
    NodoLista* anterior;     // Puntero al nodo previo (nullptr si es cabeza)
    NodoLista* siguiente;    // Puntero al nodo siguiente (nullptr si es cola)
};

class Lista {
    // Atributos privados:
    //   NodoLista* cabeza   — primer nodo (nullptr si vacia)
    //   NodoLista* cola     — ultimo nodo (nullptr si vacia)
    //   int tam             — cantidad de nodos

    Lista();                         // Constructor: cabeza = cola = nullptr, tam = 0
    ~Lista();                        // Destructor: libera todos los nodos via vaciar()
    void insertar(int dato);         // Agrega al final, O(1) por puntero cola
    void eliminar(int dato);         // Elimina primera ocurrencia, O(n)
    bool contiene(int dato) const;   // Busqueda lineal, O(n)
    int obtener(int indice) const;   // Acceso por indice (0-based), O(n)
    int longitud() const;            // Retorna tam
    void vaciar();                   // Libera todos los nodos, O(n)
    NodoLista* getCabeza() const;    // Retorna cabeza (para recorridos externos)
    NodoLista* getCola() const;      // Retorna cola
};
```

**Uso en el proyecto:** Guarda la ruta optima al tesoro producida por BFS, DFS o Dijkstra. Tambien almacena caminos intermedios durante la reconstruccion desde el arreglo de padres.

---

### cola.hpp — Cola circular

```cpp
class Cola {
    // Atributos privados:
    //   int* datos        — arreglo dinamico con los elementos
    //   int frente        — indice del primer elemento
    //   int final         — indice del ultimo elemento
    //   int capacidad     — tamano maximo del arreglo
    //   int tam           — cantidad de elementos actuales

    Cola(int capacidad = 100);       // Asigna new int[capacidad]
    ~Cola();                         // delete[] datos
    void encolar(int valor);         // (final+1) % capacidad, incrementa tam
    int desencolar();                // Retorna datos[frente], (frente+1) % capacidad, decrementa tam
    int peek() const;                // Retorna datos[frente] sin remover
    bool estaVacia() const;          // tam == 0
    bool estaLlena() const;          // tam == capacidad
    int obtenerTam() const;          // Retorna tam
    int getFrente() const;           // Retorna el indice frente (para visualizacion)
    int getFinal() const;            // Retorna el indice final
    int getCapacidad() const;        // Retorna capacidad
    int getDatoEn(int indice) const; // Acceso relativo: datos[(frente + indice) % capacidad]
    void vaciar();                   // frente = 0, final = -1, tam = 0
};
```

**Uso en el proyecto:** Gestiona el orden de exploracion en BFS. Se expone al panel grafico para mostrar su contenido en vivo durante la animacion.

---

### pila.hpp — Pila dinamica

```cpp
class Pila {
    // Atributos privados:
    //   int* datos        — arreglo dinamico
    //   int tope          — indice del ultimo elemento (-1 si vacia)
    //   int capacidad     — tamano maximo

    Pila(int capacidad = 100);       // Asigna new int[capacidad], tope = -1
    ~Pila();                         // delete[] datos
    void push(int valor);            // tope++, datos[tope] = valor
    int pop();                       // Retorna datos[tope], tope--
    int top() const;                 // Retorna datos[tope] sin remover
    bool estaVacia() const;          // tope == -1
    int obtenerTam() const;          // tope + 1
    void vaciar();                   // tope = -1
    int* getDatos() const;           // Retorna puntero al arreglo (para visualizacion)
};
```

**Uso en el proyecto:** Gestiona el orden de exploracion en DFS. Se expone al panel grafico para mostrar su contenido en vivo.

---

### diccionario.hpp — Tabla hash abierta

```cpp
struct ParClaveValor {
    char clave[50];              // Nombre de la ubicacion (ej: "playa")
    char valor[200];             // Pista asociada (ej: "Sigue el sonido del agua")
    ParClaveValor* siguiente;    // Para encadenamiento separado
};

class Diccionario {
    // Atributos privados:
    //   static const int TAM_TABLA = 101
    //   ParClaveValor* tabla[101]  — arreglo de listas enlazadas

    Diccionario();                           // Todas las entradas = nullptr
    ~Diccionario();                          // Libera todas las listas de todos los buckets

    // Privado:
    //   int hash(const char* clave) const   — suma ASCII de caracteres % TAM_TABLA

    void insertar(const char* clave, const char* valor);   // Inserta al inicio del bucket
    const char* obtener(const char* clave) const;           // Busca lineal en el bucket, retorna valor o nullptr
    bool contiene(const char* clave) const;                 // Busca existencia
    void eliminar(const char* clave);                       // Remueve nodo del bucket
};
```

**Uso en el proyecto:** Almacena la relacion ubicacion -> pista cargada desde `pistas.txt`. Consulta rapida O(1) promedio para mostrar la pista del nodo seleccionado.

---

### grafo.hpp — Grafo ponderado

```cpp
struct Arista {
    int destino;             // Indice del nodo destino en el arreglo nodos[]
    int peso;                // Costo de transitar por esta arista
    Arista* siguiente;       // Siguiente arista en la lista de adyacencia
};

struct NodoGrafo {
    char nombre[50];         // Nombre textual de la ubicacion
    Arista* aristas;         // Lista enlazada de aristas salientes
};

class Grafo {
    // Atributos privados:
    //   NodoGrafo* nodos     — arreglo dinamico de nodos
    //   int numNodos         — cantidad actual
    //   int capacidad        — tamano maximo del arreglo

    Grafo(int capacidad = 100);              // Asigna new NodoGrafo[capacidad]
    ~Grafo();                                // Libera aristas y arreglo de nodos

    int agregarNodo(const char* nombre);              // Copia nombre, retorna indice del nuevo nodo
    void agregarArista(int origen, int destino, int peso);  // Inserta al inicio de la lista del nodo origen
    int buscarNodo(const char* nombre) const;                // Busqueda lineal por nombre, retorna indice o -1
    const char* getNombreNodo(int indice) const;             // Retorna nombre en indice dado
    Arista* getAristas(int nodo) const;                      // Retorna cabeza de lista de aristas del nodo
    int getNumNodos() const;                    // Retorna numNodos
    void limpiar();                             // Libera todo y reinicia a 0 nodos
};
```

**Uso en el proyecto:** Representa el mapa de la isla. Se carga desde `grafo.txt`. Es la estructura central sobre la que operan BFS, DFS y Dijkstra.

---

### arbol.hpp — Arbol de decision n-ario

```cpp
struct NodoArbol {
    char pista[200];            // Texto de la pista (ej: "Busca donde el sol no llega")
    char destino[50];           // Nombre de la ubicacion a la que lleva (ej: "cueva")
    NodoArbol** hijos;          // Arreglo dinamico de punteros a hijos
    int numHijos;               // Cantidad de hijos
    NodoArbol* padre;           // Puntero al padre (nullptr si es raiz)
};

class Arbol {
    // Atributos privados:
    //   NodoArbol* raiz        — nodo raiz

    Arbol();                     // raiz = nullptr
    ~Arbol();                    // Libera recursivamente

    // Privados:
    //   void limpiarRecursivo(NodoArbol* nodo)     — libera hijos y el nodo
    //   NodoArbol* buscarNodoRecursivo(NodoArbol*, const char*) — busca por pista

    void crearRaiz(const char* pista, const char* destino);           // Crea nodo raiz
    NodoArbol* agregarHijo(NodoArbol* padre, const char* pista, const char* destino);  // Agrega hijo con redimension del arreglo
    NodoArbol* getRaiz() const;                                       // Retorna raiz
    const char* interpretar(const char* pista) const;                 // Busca pista y retorna destino asociado
    void limpiar();                                                   // Elimina todo el arbol
};
```

**Uso en el proyecto:** Modela las pistas como un arbol de decision. Dada una pista, `interpretar()` retorna el destino. Alimenta la visualizacion grafica del arbol en el panel inferior derecho.

---

### archivos.hpp — File I/O

```cpp
// Carga el grafo desde un archivo .txt
// Formato esperado:
//   Linea 1: "nodos: nombre1,nombre2,..."
//   Linea 2+: "origen:destino1(peso1),destino2(peso2),..."
// Retorna: true si exito
bool cargarGrafo(const char* ruta, Grafo& grafo);

// Carga las pistas desde un archivo .txt
// Formato: "ubicacion:pista"
// Retorna: true si exito
bool cargarPistas(const char* ruta, Diccionario& dicc);

// Carga las pistas construyendo el arbol de decision
// La primera pista se asigna como hijo de la raiz "inicio"
// Las siguientes se vinculan al nodo cuyo destino coincide
// Retorna: true si exito
bool cargarArbolPistas(const char* ruta, Arbol& arbol);

// Guarda la ruta al tesoro en un archivo .txt
// Formato: "RUTA AL TESORO\n... Ruta: nombre1 -> nombre2 ...\nCosto total: N\n..."
// Retorna: true si exito
bool guardarResultado(const char* ruta, const Lista& camino, const Grafo& grafo,
                      const char* algoritmo, int costoTotal, int visitados);
```

**Uso en el proyecto:** Punto de entrada de datos (lectura) y salida de resultados (escritura). Las funciones retornan `bool` para verificacion de errores.

---

### explorador.hpp — BFS, DFS, Dijkstra

```cpp
class Explorador {
    // Atributos privados:
    //   Grafo* grafo              — grafo a explorar
    //   Diccionario* pistas       — pistas de cada ubicacion
    //   Arbol* arbolPistas        — arbol de decision de pistas
    //   Cola* colaRef             — referencia a cola para animacion en vivo
    //   Pila* pilaRef             — referencia a pila para animacion en vivo
    //   Lista* rutaRef            — referencia a lista para almacenar ruta

    Explorador();                              // Inicializa todo a nullptr
    void configurar(Grafo*, Diccionario*, Arbol*, Cola*, Pila*, Lista*);  // Vincula referencias

    // BFS completo: recorre desde inicio hasta destino usando Cola
    // Llena arreglo padres con el nodo previo de cada nodo visitado
    // Retorna true si encuentra destino
    bool bfs(int nodoInicio, int nodoDestino, int* padres, int& numVisitados, bool* visitados);

    // BFS paso a paso: avanza un paso de BFS segun el contador 'paso'
    // En paso=0 inicializa las estructuras
    // En pasos siguientes procesa un nodo del frente
    // colaLocal, frenteActual, tamFrente mantienen estado entre llamadas
    // Retorna true cuando encuentra el destino
    bool bfsPaso(int nodoInicio, int nodoDestino, int* padres,
                 int& numVisitados, bool* visitados,
                 int& frenteActual, int& tamFrente, int* colaLocal, int paso);

    // DFS completo: recorre desde inicio hasta destino usando Pila
    // Usa arreglo expandidos para saber que nodos ya tienen todos sus vecinos explorados
    bool dfs(int nodoInicio, int nodoDestino, int* padres, int& numVisitados, bool* visitados);

    // DFS paso a paso: similar a bfsPaso pero con Pila
    bool dfsPaso(int nodoInicio, int nodoDestino, int* padres,
                 int& numVisitados, bool* visitados,
                 int& topeLocal, int* pilaLocal, bool* expandidos, int paso);

    // Dijkstra: camino mas corto desde inicio hasta destino
    // Usa arreglo de distancias con busqueda lineal del minimo O(V^2)
    // Retorna costo total o -1 si no hay camino
    int dijkstra(int nodoInicio, int nodoDestino, int* padres, int* distancias);

    // Navega secuencialmente por pistas:
    //   nodoActual -> obtenerPista -> arbol.interpretar(pista) -> siguienteNodo
    // Llena caminoPistas con los indices visitados
    // Retorna el ultimo nodo alcanzado (idealmente el tesoro)
    int navegarPorPistas(int nodoInicio, int* caminoPistas, int& numPasos);

    // Reconstruye el camino desde el destino hasta el inicio usando el arreglo padres
    // Invierte el orden para que quede inicio -> ... -> destino
    void reconstruirCamino(int destino, int* padres, Lista& camino);
};
```

**Uso en el proyecto:** Contiene todos los algoritmos de exploracion. `bfsPaso` / `dfsPaso` permiten la animacion paso a paso desde el game loop. `dijkstra` se ejecuta completo en un solo frame.

---

## core/ — Logica del juego

### juego.hpp / juego.cpp

```cpp
enum EstadoJuego {
    INICIO,              // Pantalla inicial, esperando seleccion de nodo
    EXPLORANDO_BFS,      // BFS en ejecucion (animacion paso a paso)
    EXPLORANDO_DFS,      // DFS en ejecucion (animacion paso a paso)
    RUTA_OPTIMA,         // Dijkstra completado, ruta visible
    COMPLETADO,          // Tesoro encontrado por BFS o DFS
    PAUSADO              // (reservado para futuras expansiones)
};

class Juego {
    // Atributos privados principales:
    //   Grafo grafo               — mapa de la isla
    //   Diccionario pistas        — tabla hash ubicacion -> pista
    //   Arbol arbolPistas         — arbol de decision de pistas
    //   Cola colaAnimacion        — cola expuesta al panel grafico
    //   Pila pilaAnimacion        — pila expuesta al panel grafico
    //   Lista rutaOptima          — ruta final hallada
    //   Explorador explorador     — instancia de algoritmos
    //
    //   EstadoJuego estado        — estado actual
    //   int nodoSeleccionado      — indice del nodo clickeado
    //   int nodoInicio            — desde donde se inicia la busqueda
    //   int nodoTesoro            — indice del nodo "tesoro"
    //   int costoTotal            — costo acumulado de la ruta
    //   int numVisitados          — contador de visitados
    //   char algoritmoUsado[50]   — nombre del algoritmo ejecutado
    //
    //   int padres[100]           — arreglo de padres para reconstruccion
    //   bool visitadosAnim[100]   — estado de visita para colorear nodos
    //   int distanciasAnim[100]   — distancias calculadas por Dijkstra
    //   int colaLocal[100]        — frente de BFS para paso a paso
    //   int pilaLocal[100]        — pila de DFS para paso a paso
    //   bool expandidos[100]      — control de expansion en DFS
    //   int pasoActual            — contador de pasos de animacion

    Juego();
    ~Juego();

    bool cargarDatos();              // Llama a cargarGrafo, cargarPistas, cargarArbolPistas
    void seleccionarNodo(int indice);        // Fija nodoSeleccionado, muestra pista por consola
    void iniciarBFS();                       // Prepara estado para BFS paso a paso
    void iniciarDFS();                       // Prepara estado para DFS paso a paso
    void iniciarDijkstra();                  // Ejecuta Dijkstra completo, reconstruye ruta
    void navegarPorPistas();                // (reservado)
    void pasoAnimacion();                    // Avanza un paso de BFS o DFS segun estado
    void limpiar();                          // Reinicia todos los estados y estructuras
    bool guardarResultado();                // Llama a ::guardarResultado de archivos.hpp

    // Getters para el panel grafico:
    EstadoJuego getEstado() const;
    int getNodoSeleccionado() const;
    int getNodoInicio() const;
    int getCostoTotal() const;
    int getNumVisitados() const;
    int getPasoActual() const;
    const char* getAlgoritmoUsado() const;
    const char* getPistaNodo(int indice) const;     // Busca en diccionario por nombre del nodo
    bool getVisitado(int indice) const;             // Estado de visitadosAnim
    bool getEnRuta(int indice) const;               // Si el indice esta en rutaOptima
    Grafo& getGrafo();
    Arbol& getArbolPistas();
    Cola& getColaAnimacion();
    Pila& getPilaAnimacion();
    Lista& getRutaOptima();
};
```

**Uso en el proyecto:** Orquesta toda la logica. El `main.cpp` llama a sus metodos en cada iteracion del game loop. Mantiene el estado compartido entre los algoritmos y la capa grafica.

---

## graphics/ — Capa grafica (raylib)

### colores.hpp — Paleta de colores

Todos son tipo `Color` de raylib (RGBA 0-255):

| Constante        | RGB           | Uso                          |
|------------------|---------------|------------------------------|
| COLOR_FONDO      | 44, 44, 68    | Fondo de la ventana          |
| COLOR_NODO_NO_VIS| 128, 128, 128 | Nodo no visitado             |
| COLOR_NODO_EN_COLA| 255, 255, 0  | Nodo en cola/pila            |
| COLOR_NODO_VISITADO| 0, 200, 0   | Nodo ya visitado             |
| COLOR_NODO_RUTA  | 200, 0, 0     | Ruta final                   |
| COLOR_NODO_SELECCION| 0, 100, 255 | Nodo seleccionado por clic   |
| COLOR_TEXTO      | 255, 255, 255 | Texto principal              |
| COLOR_TEXTO_OSCURO| 200, 200, 200| Texto secundario             |
| COLOR_ARISTA     | 100, 100, 120 | Aristas del grafo            |
| COLOR_TESORO     | 255, 215, 0   | Nodo tesoro (dorado)         |
| COLOR_PANEL_FONDO| 30, 30, 50    | Fondo de paneles             |
| COLOR_BOTON      | 60, 60, 90    | Boton inactivo               |
| COLOR_ARBOL_LINEA| 150, 150, 180 | Lineas del arbol             |
| COLOR_COLA_BARRA | 255, 200, 0   | Barras de cola (BFS)         |
| COLOR_PILA_BARRA | 200, 100, 0   | Barras de pila (DFS)         |

---

### renderizador.hpp / renderizador.cpp — Renderizado del grafo

```cpp
struct NodoVisual {
    Vector2 posicion;        // Coordenadas (x, y) en pantalla (tipo raylib)
    Color color;             // Color segun estado del nodo
    float radio;             // Radio del circulo en pixeles (22.0f por defecto)
    bool seleccionado;       // True si el usuario hizo clic
};

class Renderizador {
    // Atributos privados:
    //   Grafo* grafo
    //   NodoVisual* nodosVisuales   — arreglo paralelo a nodos del grafo
    //   int numNodos                — cantidad de nodos
    //   float anchoPanel            — ancho disponible para el grafo
    //   float altoPanel             — alto disponible

    Renderizador(Grafo* g, float ancho, float alto);
    ~Renderizador();

    // Privados:
    //   void calcularLayout()        — Distribucion circular: angulo = 2pi*i/N, radio = min(w,h)*0.32
    //   void dibujarAristas()        — DrawLineEx entre nodos conectados, etiqueta peso en el medio
    //   void dibujarNodos()          — DrawCircle con color del estado, borde, aro si seleccionado
    //   void dibujarNombres()        — DrawText con nombre debajo de cada nodo
    //   void dibujarLeyenda()        — Rectangulo semitransparente con significado de colores

    void dibujar();                              // Dibuja aristas -> nodos -> nombres -> leyenda
    void dibujarConEstado(Juego& juego);         // Sincroniza + dibuja
    int nodoBajoMouse(Vector2 mousePos) const;   // Hit test: retorna indice bajo el cursor o -1
    void setColorNodo(int indice, Color color);   // Cambia color de un nodo especifico
    void setSeleccionado(int indice, bool sel);   // Marca/desmarca seleccion visual
    void resetearColores();                       // Gris para todos (dorado para tesoro)
    void sincronizarConJuego(Juego& juego);       // Lee estado del juego y asigna colores
};
```

**Uso en el proyecto:** Se instancia en `main.cpp` con el grafo. Su metodo `dibujar()` se llama cada frame. `sincronizarConJuego()` consulta `getVisitado()`, `getEnRuta()` y `getNodoSeleccionado()` para asignar colores. `nodoBajoMouse()` permite la interaccion por clic.

---

### panel_info.hpp / panel_info.cpp — Panel de informacion

```cpp
class PanelInfo {
    // Atributos privados:
    //   Rectangle area         — area del panel en pantalla
    //   int fuenteTam          — tamano de fuente (14)

    PanelInfo(Rectangle area);      // Recibe coordenadas y dimensiones

    // Dibuja el panel completo con:
    //   - Titulo "INFORMACION" en amarillo
    //   - Nombre del nodo seleccionado
    //   - Pista del nodo seleccionado
    //   - Costo acumulado
    //   - Estado actual (INICIO, EXPLORANDO_BFS, etc.)
    //   - Cantidad de nodos visitados
    //   - Numero de paso actual
    //   - Ruta encontrada (como texto "0 -> 3 -> 5")
    //   - Cola visual (BFS) y pila visual (DFS) en la mitad inferior
    void dibujar(const char* titulo, const char* pistaActual,
                 const char* nodoNombre, int costo,
                 const char* estadoStr, int visitados,
                 int pasoActual, const Lista& ruta,
                 const Cola& cola, const Pila& pila);

    // Dibuja la cola como barras horizontales apiladas
    // Cada barra = elemento, color decreciente en intensidad
    // La barra superior es el frente (primer elemento en salir)
    void dibujarColaVisual(const Cola& cola, Rectangle areaCola);

    // Dibuja la pila como barras horizontales apiladas
    // Cada barra = elemento, color decreciente en intensidad
    // La barra superior es el tope (ultimo elemento agregado)
    void dibujarPilaVisual(const Pila& pila, Rectangle areaPila);
};
```

**Uso en el proyecto:** Panel superior derecho. Recibe datos del `Juego` y los muestra. Las funciones `dibujarColaVisual` y `dibujarPilaVisual` son el corazon de la visualizacion "en vivo" de estructuras.

---

### animador.hpp / animador.cpp — Control de animacion

```cpp
class Animador {
    // Atributos privados:
    //   bool reproduciendo       — true si la animacion automatica esta activa
    //   bool pasoAPaso           — true si esta en modo manual (Espacio)
    //   int velocidad            — milisegundos entre pasos (50-3000, default 500)
    //   double tiempoUltimoPaso  — timestamp del ultimo avance (GetTime())
    //   int pasoActual           — paso actual de la animacion
    //   int pasosTotales         — total de pasos para la animacion actual

    Animador();                     // reproduciendo=false, pasoAPaso=false, velocidad=500

    void iniciar(int totalPasos);   // Activa reproduccion automatica, reset contadores
    void pausar();                  // Pausa (mantiene paso actual)
    void reanudar();                // Reanuda desde donde se pauso
    void togglePasoAPaso();         // Cambia entre modo automatico y manual
    bool siguientePaso();           // Avanza un paso en modo manual, retorna false si termino
    void setVelocidad(int ms);      // Cambia intervalo entre pasos
    void setVelocidadRelativa(int delta);  // +/-50 (sujeto a limites 50-3000)
    bool actualizar();              // Llama cada frame; si paso el tiempo, avanza y retorna true
    bool estaReproduciendo() const;
    int getPasoActual() const;
    void dibujarControles(Rectangle area);  // Botones Play/Pause, Step, +, -
};
```

**Uso en el proyecto:** Controla la temporizacion de la animacion. Su metodo `actualizar()` se llama en cada frame del game loop.

---

### ui_arbol.hpp / ui_arbol.cpp — Visualizacion del arbol de pistas

```cpp
class UIArbol {
    // Atributos privados:
    //   Arbol* arbol            — arbol a visualizar
    //   const char* pistaActiva — pista actual (rama a resaltar en amarillo)
    //   Rectangle area           — area de dibujo en pantalla

    UIArbol(Arbol* a, Rectangle area);

    void dibujar();                          // Dibuja el panel y el arbol
    void setNodoActivo(const char* pista);    // Marca que pista esta activa (resalta en amarillo)

    // Privados:
    //   void dibujarNodo(NodoArbol* nodo, float x, float y, float dx, float dy, int nivel, int maxNivel)
    //     — Dibuja un nodo como circulo con su destino, luego sus hijos recursivamente
    //     — Si es el nodo activo -> circulo amarillo con texto negro
    //   int contarNiveles(NodoArbol* nodo) — profundidad maxima desde este nodo
};
```

**Uso en el proyecto:** Panel inferior derecho. Muestra el arbol de decision cargado desde `pistas.txt`. Cada nodo es un circulo con el nombre de la ubicacion destino. La rama activa se resalta.

---

## main.cpp — Entry point

```
main()
  │
  ├── 1. Inicializar
  │   ├── Crear Juego
  │   ├── juego.cargarDatos()      -> carga grafo, pistas y arbol de pistas
  │   ├── Crear Renderizador        -> layout circular de nodos
  │   ├── Crear PanelInfo           -> area superior derecha
  │   ├── Crear UIArbol             -> area inferior derecha
  │   ├── InitWindow(1280, 720)
  │   └── SetTargetFPS(60)
  │
  ├── 2. Game Loop (while !WindowShouldClose)
  │   │
  │   ├── 2.1 Input Processing
  │   │   ├── MOUSE_LEFT_BUTTON  -> renderizador.nodoBajoMouse() -> juego.seleccionarNodo()
  │   │   ├── KEY_B              -> juego.iniciarBFS()
  │   │   ├── KEY_D              -> juego.iniciarDFS()
  │   │   ├── KEY_R              -> juego.iniciarDijkstra()
  │   │   ├── KEY_SPACE          -> juego.pasoAnimacion()
  │   │   ├── KEY_C              -> juego.limpiar() + renderizador.resetearColores()
  │   │   └── KEY_S              -> juego.guardarResultado()
  │   │
  │   ├── 2.2 Sincronizacion
  │   │   └── renderizador.sincronizarConJuego(juego)
  │   │
  │   ├── 2.3 Render (BeginDrawing / EndDrawing)
  │   │   ├── ClearBackground(COLOR_FONDO)
  │   │   ├── renderizador.dibujar()
  │   │   ├── panelInfo.dibujar(..., juego.getPistaNodo(...), ...)
  │   │   ├── uiArbol.dibujar()
  │   │   └── Instrucciones en pantalla
  │   │
  │   └── (fin loop)
  │
  ├── 3. Finalizar
  │   ├── CloseWindow()
  │   └── return 0
```

---

## data/ — Archivos de datos

### grafo.txt — Mapa de la isla

```
nodos: playa,montana,cueva,bosque,laguna,templo,tesoro
playa:montana(3),cueva(5)
montana:cueva(2),bosque(4)
cueva:laguna(6),templo(1)
bosque:laguna(2)
laguna:templo(3),tesoro(4)
templo:tesoro(2)
```

**Formato:**
- Linea 1: `nodos:` seguido de nombres separados por comas. Cada nombre es un nodo del grafo.
- Lineas 2+: `origen:destino1(peso1),destino2(peso2),...`. Cada par es una arista dirigida con costo.
- El mapa representa una isla con 7 ubicaciones. El "tesoro" es el nodo destino.

### pistas.txt — Pistas por ubicacion

```
playa:Sigue el sonido del agua
montana:Busca donde el sol no llega
cueva:El eco te guiara al este
bosque:Las flores senalan el camino
laguna:Reflejo de la luna te mostrara el camino
templo:Detras del altar yace la respuesta
tesoro:!Lo has encontrado!
```

**Formato:** `ubicacion:pista` (una por linea). Cada ubicacion tiene una pista textual. El arbol de decision se construye relacionando estas pistas con los nombres de las ubicaciones.

### resultado.txt — Output generado

```
RUTA AL TESORO
==============
Inicio: playa
Ruta: playa -> montana -> cueva -> templo -> tesoro
Costo total: 8
Visitados: 6 nodos
Algoritmo usado: Dijkstra
```

**Generado por:** `guardarResultado()` al presionar **S** en el juego. Muestra la ruta encontrada con nombres de nodos en vez de indices numericos.

---

## Makefile — Compilacion

```makefile
CXX = g++
CXXFLAGS = -std=c++17 -Iinclude -Igraphics -Icore
LDFLAGS = -lraylib -lglfw3 -lopengl32 -lgdi32 -lwinmm

SRC = main.cpp \
      src/lista.cpp src/cola.cpp src/pila.cpp \
      src/diccionario.cpp src/grafo.cpp src/arbol.cpp \
      src/archivos.cpp src/explorador.cpp \
      core/juego.cpp \
      graphics/renderizador.cpp graphics/panel_info.cpp \
      graphics/animador.cpp graphics/ui_arbol.cpp

OBJ = $(SRC:.cpp=.o)
OUT = tesoro.exe

all: $(OUT)
$(OUT): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@
clean:
	rm -f $(OBJ) $(OUT)
run: $(OUT)
	./$(OUT)
```

**Uso:** `make` compila, `make clean` limpia objetos, `make run` compila y ejecuta.

---

## Compilacion y ejecucion

```bash
# Desde MSYS2 MinGW64 en la carpeta tesoropirata
mingw32-make        # Compila todos los .cpp y genera tesoro.exe
./tesoro.exe        # Ejecuta el programa

# Compilar y ejecutar en un paso
mingw32-make run

# Limpiar archivos objeto
mingw32-make clean
```

---

## Controles del programa

| Entrada       | Accion                  | Descripcion tecnica                              |
|---------------|-------------------------|--------------------------------------------------|
| Click en nodo | seleccionarNodo()       | Detecta colision circulo-punto con distancia euclidiana |
| **B**         | iniciarBFS()            | Inicializa arreglos, setea estado EXPLORANDO_BFS |
| **D**         | iniciarDFS()            | Inicializa arreglos, setea estado EXPLORANDO_DFS |
| **R**         | iniciarDijkstra()       | Ejecuta Dijkstra completo O(V^2), reconstruye ruta |
| **Espacio**   | pasoAnimacion()         | bfsPaso() o dfsPaso() segun estado actual        |
| **C**         | limpiar() + resetearColores() | Vacua cola, pila, ruta; resetea colores      |
| **S**         | guardarResultado()      | Escribe resultado.txt con nombres de nodos        |
| **ESC**       | WindowShouldClose()     | Cierra ventana y termina el programa              |

---

## Algoritmos implementados

| Algoritmo | Estructura usada | Complejidad | Descripcion |
|-----------|-----------------|-------------|-------------|
| **BFS**   | Cola circular   | O(V + E)    | Recorrido por niveles. Usa `bfsPaso()` para animacion paso a paso |
| **DFS**   | Pila dinamica   | O(V + E)    | Recorrido en profundidad. Usa `dfsPaso()` para animacion paso a paso, con arreglo `expandidos` para control de retroceso |
| **Dijkstra** | Arreglo (busqueda lineal del minimo) | O(V^2) | Camino mas corto desde nodo inicio a tesoro. Adecuado para 20-100 nodos |

---

## Pantalla

```
+-----------------------------------+------------------------------+
|                                   |  INFORMACION                 |
|   Mapa del grafo                 |  - Nodo seleccionado         |
|   (nodos cliqueables)            |  - Pista actual              |
|   (colores por estado)           |  - Costo acumulado           |
|                                   |  - Estado de exploracion     |
|   Colores:                       |                              |
|   Gris  = no visitado            |  [COLA (BFS)] [PILA (DFS)]  |
|   Verde = visitado               |  barras visibles en vivo     |
|   Rojo  = ruta final             +------------------------------+
|   Azul  = seleccionado           |  ARBOL DE PISTAS             |
|   Oro   = tesoro                 |  (rama activa resaltada)     |
+-----------------------------------+------------------------------+
```

---

## Diagrama de dependencias entre archivos

```
main.cpp
  |-- core/juego.hpp
  |     |-- include/grafo.hpp
  |     |-- include/diccionario.hpp
  |     |-- include/arbol.hpp
  |     |-- include/cola.hpp
  |     |-- include/pila.hpp
  |     |-- include/lista.hpp
  |     |-- include/archivos.hpp
  |     |-- include/explorador.hpp
  |           |-- include/grafo.hpp
  |
  |-- graphics/renderizador.hpp
  |     |-- include/grafo.hpp
  |     |-- include/lista.hpp
  |     |-- core/juego.hpp
  |
  |-- graphics/panel_info.hpp
  |     |-- include/lista.hpp
  |     |-- include/cola.hpp
  |     |-- include/pila.hpp
  |
  |-- graphics/ui_arbol.hpp
  |     |-- include/arbol.hpp
  |
  |-- graphics/colores.hpp
```

---

## Requisitos del sistema

- **SO**: Windows 10/11
- **Compilador**: g++ 15+ (MinGW-w64 desde MSYS2)
- **Librerias**: raylib 5.x, glfw3 (instalar via `pacman`)
  ```
  pacman -S mingw-w64-x86_64-raylib mingw-w64-x86_64-glfw
  ```
- **Build**: GNU Make 4.4+
