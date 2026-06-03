# El Tesoro del Pirata — Plan de Implementación

Proyecto académico de Estructuras de Datos y Algoritmos en C++ con representación gráfica en vivo usando raylib.

---

## Tabla de Contenidos

1. [Descripción del Proyecto](#1-descripción-del-proyecto)
2. [Requisitos Funcionales](#2-requisitos-funcionales)
3. [Tech Stack](#3-tech-stack)
4. [Arquitectura del Proyecto](#4-arquitectura-del-proyecto)
5. [Diseño de Estructuras de Datos](#5-diseño-de-estructuras-de-datos)
6. [Capa Gráfica (raylib)](#6-capa-gráfica-raylib)
7. [Flujo del Programa](#7-flujo-del-programa)
8. [Formato de Archivos](#8-formato-de-archivos)
9. [Setup y Compilación](#9-setup-y-compilación)
10. [Orden de Implementación](#10-orden-de-implementación)
11. [Estructuras Visualizadas en Vivo](#11-estructuras-visualizadas-en-vivo)
12. [Interacción del Usuario](#12-interacción-del-usuario)
13. [Recomendaciones Finales](#13-recomendaciones-finales)

---

## 1. Descripción del Proyecto

Ayudar a un pirata a encontrar un tesoro escondido en una isla. El mapa es un **grafo** donde los nodos son ubicaciones (playa, montaña, cueva, etc.) y las aristas son caminos con costo. Cada ubicación tiene **pistas** (archivo de texto) que guían al tesoro.

### Requisitos originales

| # | Estructura | Propósito |
|---|-----------|-----------|
| 1 | **Grafo** | Representar el mapa y sus conexiones |
| 2 | **Árbol** | Interpretar las pistas (árbol de decisiones) |
| 3 | **Cola / Pila** | Gestionar el orden de exploración (BFS/DFS) |
| 4 | **Diccionario / Tabla Hash** | Almacenar información de ubicaciones |
| 5 | **Lista** | Guardar el camino óptimo al tesoro |
| 6 | **Archivos** | Leer pistas desde archivo y guardar el resultado |

### Tarea del programa

1. Cargar el grafo del mapa desde un archivo
2. Usar BFS/DFS para explorar la isla
3. Interpretar las pistas (ej: *"Busca donde el sol no llega"* → cueva)
4. Encontrar el camino más corto o la secuencia correcta de pistas
5. Guardar la ruta al tesoro en otro archivo
6. **Mostrar todo gráficamente en vivo** con raylib

---

## 2. Requisitos Funcionales

| ID | Requisito | Estructura DSA |
|----|-----------|----------------|
| FR1 | Cargar grafo del mapa desde archivo | Grafo (lista de adyacencia manual) |
| FR2 | Cada nodo = ubicación con nombre + pista | Diccionario/Tabla Hash (nombre → info) |
| FR3 | Aristas con peso (costo del camino) | Grafo ponderado |
| FR4 | Explorar la isla (BFS/DFS) | Cola (BFS) / Pila (DFS) |
| FR5 | Interpretar pistas secuencialmente | Árbol de decisión (cada pista → nodo hijo) |
| FR6 | Encontrar camino óptimo al tesoro | Lista (ruta final) + Dijkstra |
| FR7 | Leer pistas desde archivo | File I/O (ifstream) |
| FR8 | Guardar ruta del tesoro en archivo | File I/O (ofstream) |
| FR9 | Visualizar estructuras en tiempo real | raylib (gráficos 2D) |

### No funcionales

- **Rendimiento**: 20-100 nodos → O(V+E) suficiente
- **Corrección**: Prioridad #1 — funcionar según especificación académica
- **Mantenibilidad**: Código modular con archivos separados por estructura
- **Portabilidad**: C++ estándar, compilable con g++ (MinGW-w64)

### Constraints

| Aspecto | Valor |
|---------|-------|
| Lenguaje | **C++17** |
| STL | **No permitida** — implementación manual |
| Entorno | **Windows + MinGW-w64** (w64devkit) |
| Gráficos | **raylib** (sin dependencias externas) |
| Tamaño grafo | Mediano (20-100 nodos) |
| Tiempo | 1-2 semanas |
| Propósito | Tarea académica + proyecto personal |

---

## 3. Tech Stack

| Capa | Recomendación | Justificación |
|------|--------------|---------------|
| **Lenguaje** | C++17 | Balance características modernas / compatibilidad |
| **Compilador** | g++ (MinGW-w64 vía w64devkit) | Gratuito, funciona en Windows, recomendado por raylib |
| **Gráficos** | raylib 5.x | Sin dependencias, educativo, fácil de aprender |
| **GUI** | raygui (incluido en raylib) | UI inmediata para paneles de información |
| **Build** | Makefile + mingw32-make | Simple, sin IDE requerido |
| **Testing** | Manual con archivos .txt de prueba | Verificar cada estructura con datos concretos |
| **Control versiones** | Git + GitHub (privado) | Respaldo y entrega académica |

### ¿Por qué raylib y no SFML?

| Factor | raylib | SFML |
|--------|--------|------|
| Dependencias externas | **Cero** | Requiere CMake |
| Setup en MinGW | w64devkit + batch, listo en 5 min | Configuración más compleja |
| Curva de aprendizaje | **Muy baja** (~20 funciones) | Media |
| Enfoque | Educación, prototipado, tooling | Juegos, multimedia |
| GUI inmediata | **raygui incluido** | No tiene |

---

## 4. Arquitectura del Proyecto

```
el-tesoro-del-pirata/
│
├── include/                        # Headers (.hpp)
│   ├── grafo.hpp                   # Grafo (lista de adyacencia manual)
│   ├── arbol.hpp                   # Árbol de decisión para pistas
│   ├── cola.hpp                    # Cola circular (BFS)
│   ├── pila.hpp                    # Pila dinámica (DFS)
│   ├── diccionario.hpp             # Tabla hash abierta
│   ├── lista.hpp                   # Lista doblemente enlazada
│   └── archivos.hpp                # File I/O (lectura/escritura)
│
├── src/                            # Implementaciones (.cpp)
│   ├── grafo.cpp
│   ├── arbol.cpp
│   ├── cola.cpp
│   ├── pila.cpp
│   ├── diccionario.cpp
│   ├── lista.cpp
│   └── archivos.cpp
│
├── graphics/                       # Capa gráfica (raylib)
│   ├── renderizador.hpp/cpp        # Dibuja el grafo (nodos, aristas)
│   ├── panel_info.hpp/cpp          # Panel con info del nodo seleccionado
│   ├── animador.hpp/cpp            # Animación paso a paso (BFS/DFS)
│   ├── ui_arbol.hpp/cpp            # Visualiza el árbol de decisión
│   └── colores.hpp                 # Paleta de colores temática pirata
│
├── core/                           # Lógica de orquestación
│   ├── juego.hpp/cpp               # Estado del juego, loop principal
│   └── explorador.hpp/cpp          # BFS, DFS, Dijkstra
│
├── data/                           # Archivos de entrada/salida
│   ├── grafo.txt                   # Mapa de la isla
│   ├── pistas.txt                  # Pistas por ubicación
│   └── resultado.txt               # Output generado por el programa
│
├── tests/                          # Archivos de prueba
│   ├── grafo_prueba.txt
│   ├── pistas_prueba.txt
│   └── caso1_esperado.txt
│
├── main.cpp                        # Punto de entrada
├── Makefile                        # Compilación
└── README.md                       # Documentación
```

---

## 5. Diseño de Estructuras de Datos

### 5.1 Lista (doble enlace)

```cpp
struct NodoLista {
    int dato;
    NodoLista* anterior;
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
    void eliminar(int dato);
    bool contiene(int dato) const;
    int obtener(int indice) const;
    int longitud() const;
    void vaciar();
    NodoLista* getCabeza() const;
};
```

### 5.2 Cola circular

```cpp
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
};
```

### 5.3 Pila (arreglo dinámico)

```cpp
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
};
```

### 5.4 Diccionario / Tabla Hash

```cpp
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
```

### 5.5 Grafo (lista de adyacencia)

```cpp
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
public:
    Grafo(int capacidad = 100);
    ~Grafo();
    int agregarNodo(const char* nombre);
    void agregarArista(int origen, int destino, int peso);
    int buscarNodo(const char* nombre) const;
    const char* getNombreNodo(int indice) const;
    Arista* getAristas(int nodo) const;
    int getNumNodos() const;
};
```

### 5.6 Árbol de decisión (n-ario)

```cpp
struct NodoArbol {
    char pista[200];
    char destino[50];
    NodoArbol** hijos;
    int numHijos;
    NodoArbol* padre;
};

class Arbol {
private:
    NodoArbol* raiz;
public:
    Arbol();
    ~Arbol();
    NodoArbol* agregarHijo(NodoArbol* padre, const char* pista, const char* destino);
    NodoArbol* getRaiz() const;
    const char* interpretar(const char* pista) const;
};
```

---

## 6. Capa Gráfica (raylib)

### 6.1 renderizador.hpp

```cpp
#include "raylib.h"
#include "../include/grafo.hpp"

struct NodoVisual {
    Vector2 posicion;       // Centro del nodo en pantalla
    Color color;            // Estado del nodo (no visitado / visitado / en ruta)
    float radio;            // Radio del círculo
    bool seleccionado;
};

class Renderizador {
private:
    Grafo* grafo;
    NodoVisual* nodosVisuales;
    int numNodos;
    void calcularLayout();   // Disposición automática (circular o spring)
    void dibujarAristas();
    void dibujarNodos();
    void dibujarNombres();
public:
    Renderizador(Grafo* g);
    ~Renderizador();
    void dibujar();
    int nodoBajoMouse(Vector2 mousePos) const;  // Para clic
    void setColorNodo(int indice, Color color);
    void setSeleccionado(int indice, bool sel);
};
```

### 6.2 panel_info.hpp

```cpp
class PanelInfo {
private:
    // Área del panel (mitad derecha de la pantalla)
    Rectangle area;
public:
    PanelInfo(Rectangle area);
    void dibujar(const char* nodoNombre, const char* pista,
                 int costo, const char* estadoActual,
                 const Lista& ruta, const Cola& cola, const Pila& pila);
    void mostrarArbol(NodoArbol* raiz, NodoArbol* activo);
};
```

### 6.3 animador.hpp

```cpp
class Animador {
private:
    bool reproduciendo;
    int pasoActual;
    int velocidad;            // ms entre pasos
    double tiempoUltimoPaso;
public:
    Animador();
    void iniciar();
    void pausar();
    void siguientePaso();
    void anteriorPaso();
    void actualizar();        // Llama cada frame
    bool pasoCompletado() const;
    void setVelocidad(int ms);
};
```

### 6.4 Layout de pantalla

```
┌─────────────────────────────────────────────────┐
│  [Mapa del grafo]       │  [Panel de info]       │
│  (nodos cliqueables)    │  - Nombre del nodo      │
│  (BFS/DFS animado)      │  - Pista actual         │
│  (ruta resaltada)       │  - Costo acumulado      │
│                         │  - Estado exploración   │
├─────────────────────────┴─────────────────────────┤
│  [Cola/Pila en vivo]     [Árbol de decisión]      │
│  - Animación BFS/DFS    │  - Pistas jerárquicas   │
│  - Push/pop en tiempo   │  - Rama activa          │
│    real                 │  resaltada              │
└─────────────────────────────────────────────────┘
```

---

## 7. Flujo del Programa

```
main()
  │
  ├── Inicializar raylib (ventana 1280x720, "El Tesoro del Pirata")
  │
  ├── cargarGrafo("data/grafo.txt")       → Grafo (lista adyacencia)
  ├── cargarPistas("data/pistas.txt")     → Diccionario (nodo → pista)
  ├── cargarArbolPistas("data/pistas.txt") → Arbol (decisión)
  │
  ├── estado = SELECCIONAR_INICIO
  │
  ├── Loop principal (while !WindowShouldClose):
  │   │
  │   ├── Procesar input:
  │   │   ├── Clic en nodo → seleccionar / mostrar info
  │   │   ├── B → iniciar BFS animado
  │   │   ├── D → iniciar DFS animado
  │   │   ├── R → mostrar ruta óptima (Dijkstra)
  │   │   ├── ESPACIO → paso a paso en animación
  │   │   └── ESC → salir
  │   │
  │   ├── Actualizar animación
  │   │
  │   └── Dibujar frame:
  │       ├── Renderizador.dibujar()     → Grafo en pantalla
  │       ├── PanelInfo.dibujar()         → Info + cola/pila/árbol
  │       └── UI (raygui)                → Botones de control
  │
  ├── Limpiar recursos
  └── Cerrar ventana
```

### Algoritmo de búsqueda

```
FASE 1 — Navegación por pistas:
  nodoActual = nodoInicial (ej: "playa")
  mientras (nodoActual != tesoro):
    1. pista = diccionario.obtener(nodoActual)
    2. siguiente = arbol.interpretar(pista)  → nombre del próximo nodo
    3. ruta.agregar(nodoActual)
    4. # Opcional: BFS desde nodoActual hasta siguiente
    5. nodoActual = siguiente

FASE 2 — Camino más corto:
  camino = dijkstra(grafo, inicio, tesoro)

FASE 3 — Salida:
  guardarResultado("data/resultado.txt", camino)
```

---

## 8. Formato de Archivos

### grafo.txt

```
nodos: playa,montaña,cueva,bosque,laguna,templo,tesoro
playa:montaña(3),cueva(5)
montaña:cueva(2),bosque(4)
cueva:laguna(6),templo(1)
bosque:laguna(2)
laguna:templo(3),tesoro(4)
templo:tesoro(2)
```

**Formato:**
- Línea 1: `nodos: nombre1,nombre2,...`
- Línea 2+: `origen:destino1(peso1),destino2(peso2),...`

### pistas.txt

```
playa:Sigue el sonido del agua
montaña:Busca donde el sol no llega
cueva:El eco te guiará al este
bosque:Las flores señalan el camino
laguna:Reflejo de la luna te mostrará el camino
templo:Detrás del altar yace la respuesta
tesoro:¡Lo has encontrado!
```

**Formato:** `ubicacion:pista`

### resultado.txt (output)

```
RUTA AL TESORO
==============
Inicio: playa
Ruta: playa → montaña → cueva → templo → tesoro
Costo total: 8
Visitados: 7 nodos
Algoritmo usado: Dijkstra
```

---

## 9. Setup y Compilación

### 9.1 Instalar herramientas

1. **Descargar w64devkit** (MinGW-w64):
   ```
   https://github.com/skeeto/w64devkit/releases
   ```
   Extraer a `C:\w64devkit`

2. **Descargar raylib** (binarios para MinGW-w64):
   ```
   https://github.com/raysan5/raylib/releases
   ```
   Buscar `raylib-5.x_win64_mingw-w64.zip`
   Extraer a la carpeta del proyecto (tener `include/` y `lib/`)

### 9.2 Makefile

```makefile
CXX = g++
CXXFLAGS = -std=c++17 -Iinclude -Igraphics -Icore -Iraylib/include
LDFLAGS = -Lraylib/lib -lraylib -lgdi32 -lwinmm -static
SRC = main.cpp \
      src/grafo.cpp src/arbol.cpp src/cola.cpp \
      src/pila.cpp src/diccionario.cpp src/lista.cpp \
      src/archivos.cpp \
      graphics/renderizador.cpp graphics/panel_info.cpp \
      graphics/animador.cpp graphics/ui_arbol.cpp \
      core/juego.cpp core/explorador.cpp
OBJ = $(SRC:.cpp=.o)
OUT = tesoro.exe

all: $(OUT)

$(OUT): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	del /Q src\*.o graphics\*.o core\*.o *.o *.exe 2>nul || true

run: $(OUT)
	$(OUT)
```

### 9.3 Compilar y ejecutar

```bash
# Desde w64devkit.exe (o CMD con PATH a w64devkit)
mingw32-make
mingw32-make run

# O en un solo paso
mingw32-make && tesoro.exe
```

---

## 10. Orden de Implementación

### Semana 1 — Estructuras de datos + lógica (consola)

| Día | Tarea | Verificación |
|-----|-------|-------------|
| **1** | Implementar **Lista** doblemente enlazada (lista.hpp/cpp) | Tests: insertar, eliminar, recorrer |
| **2** | Implementar **Cola** circular + **Pila** dinámica (cola.hpp, pila.hpp) | Tests: encolar/desencolar, push/pop |
| **3** | Implementar **Diccionario** / Tabla Hash (diccionario.hpp/cpp) | Tests: insertar, obtener, contener |
| **4** | Implementar **Grafo** (lista de adyacencia) + **Árbol** n-ario | Tests: aristas, vecinos, recorrido árbol |
| **5** | Implementar **File I/O** (archivos.hpp/cpp) + BFS, DFS, **Dijkstra** | Tests: cargar grafo, ejecutar algoritmos, guardar |

### Semana 2 — Capa gráfica + pulido

| Día | Tarea | Verificación |
|-----|-------|-------------|
| **1** | Integrar raylib: Makefile, ventana, loop principal | Ventana 1280x720 en blanco |
| **2** | Renderizar grafo: layout circular, nodos, aristas, nombres | Grafo visible en pantalla |
| **3** | Panel de información: nodo seleccionado, pista, costo | Clic en nodo → muestra info |
| **4** | Animación BFS/DFS: cola/pila en vivo, paso a paso | B y D animan correctamente |
| **5** | Árbol de decisión visual + ruta óptima + archivo resultado.txt | Todo integrado y funcional |

---

## 11. Estructuras Visualizadas en Vivo

| Estructura | Representación gráfica |
|-----------|----------------------|
| **Grafo** | Nodos como círculos conectados por líneas. Peso visible en cada arista. Color según estado: gris (no visitado), amarillo (en cola/pila), verde (visitado), rojo (ruta final) |
| **Cola (BFS)** | Barras horizontales animadas — los elementos entran por la derecha, salen por la izquierda. Se actualiza en cada paso de BFS |
| **Pila (DFS)** | Barras verticales animadas — push apila arriba, pop desapila. Se actualiza en cada paso de DFS |
| **Árbol de decisión** | Árbol dibujado de arriba abajo. Cada nodo muestra la pista. La rama activa se resalta en dorado |
| **Diccionario** | Tabla con pares clave→valor visibles al seleccionar un nodo |
| **Lista (ruta)** | Secuencia de nodos conectados con flechas, mostrando el camino final al tesoro |

### Mapa de colores

| Estado | Color | Significado |
|--------|-------|-------------|
| No visitado | `GRAY` | Nodo aún no explorado |
| En cola/pila | `YELLOW` | Pendiente de visitar |
| Visitado | `GREEN` | Ya procesado |
| En ruta final | `RED` | Parte del camino al tesoro |
| Seleccionado | `BLUE` | Usuario hizo clic |

---

## 12. Interacción del Usuario

| Tecla / Input | Acción |
|---------------|--------|
| **Clic en nodo** | Seleccionar ubicación, mostrar pista e información |
| **B** | Iniciar BFS animado desde el nodo seleccionado |
| **D** | Iniciar DFS animado desde el nodo seleccionado |
| **R** | Mostrar ruta óptima (Dijkstra) desde inicio a tesoro |
| **Espacio** | Avanzar un paso en la animación actual |
| **+ / -** | Aumentar / disminuir velocidad de animación |
| **C** | Limpiar visualización (resetear colores) |
| **S** | Guardar resultado.txt |
| **ESC** | Salir del programa |

### Controles UI (raygui)

Botones en la parte inferior de la pantalla:

```
[ BFS ] [ DFS ] [ Ruta ] [ Paso ] [ + ] [ - ] [ Guardar ] [ Salir ]
```

---

## 13. Recomendaciones Finales

### Buenas prácticas

- **Commit temprano, commit seguido**: Usar Git desde el día 1, hacer commit por cada estructura implementada
- **Probar en consola primero**: Cada estructura debe funcionar perfectamente en tests de texto antes de agregar gráficos
- **Separar lógica de presentación**: Las estructuras en `include/` y `src/` NO deben depender de raylib
- **Mantener el Makefile actualizado**: Cada nuevo .cpp debe agregarse a SRC

### Posibles extensiones (si sobra tiempo)

| Extra | Descripción |
|-------|-------------|
| Más algoritmos | Agregar A*, Bellman-Ford, Floyd-Warshall |
| Mapa con imágenes | Fondo de isla, iconos en cada nodo |
| Música / efectos | raylib tiene módulo de audio incluido |
| Editor de mapa | Crear/editar nodos y aristas desde la UI |
| Carga de JSON | Ampliar archivos.cpp para leer JSON |

### Errores comunes a evitar

1. **No verificar punteros nulos** en listas, colas y pilas → causan crashes
2. **Olvidar liberar memoria** en destructores → memory leaks
3. **Mezclar lógica con gráficos** → código difícil de depurar
4. **No probar BFS/DFS en consola antes de animar** → bugs difíciles de ver en la UI

### Referencias

- [raylib documentation](https://www.raylib.com/cheatsheet/cheatsheet.html)
- [raylib wiki - Windows setup](https://github.com/raysan5/raylib/wiki/Working-on-Windows)
- [raylib quickstart](https://github.com/raylib-extras/raylib-quickstart)
- [w64devkit](https://github.com/skeeto/w64devkit)
- [raylib-cpp (wrapper opcional)](https://github.com/raysan5/raylib/wiki/Using-raylib-with-Cpp)

---

> **Resumen**: C++17 + estructuras manuales + raylib 5.x + Makefile + MinGW-w64.
> 1-2 semanas de implementación. 4 paneles gráficos en vivo. Todo listo para entrega académica.
