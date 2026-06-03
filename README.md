# El Tesoro del Pirata

Proyecto academico de Estructuras de Datos y Algoritmos en C++17 con representacion grafica en vivo usando raylib 5.x bajo MinGW-w64 (MSYS2). El programa explora una isla representada como **grafo**, corre **BFS/DFS/Dijkstra** paso a paso, e interpreta **pistas** mediante un arbol encadenado. Todas las estructuras de datos fueron implementadas manualmente sin STL.

---

## Guia de usuario

### Quick start
1. Abrir `tesoro.exe` (o compilar con `mingw32-make`)
2. Hacer clic en un nodo del mapa (ej: playa) — se marca con anillos blancos
3. Presionar **BFS** (o tecla **B**) para iniciar la busqueda desde ese nodo
4. La animacion arranca automaticamente en modo **AUTO**
5. Al llegar al tesoro, la ruta optima se pinta en **rojo** en el mapa
6. Presionar **LIMPIAR** (o **C**) para reiniciar y probar otro algoritmo

### Flujo completo recomendado
**Desde playa hasta el tesoro con BFS:**
1. Click en **playa** → se selecciona (anillos blancos)
2. Boton **BFS** → arranca la animacion, los nodos se pintan verde/amarillo/naranja
3. En la bitacora (panel derecho) se ve paso a paso: cola, procesando, visitados
4. Al final, la ruta aparece en rojo sobre el mapa
5. Presionar **GUARDAR** (o **S**) para exportar `resultado.txt`
6. Presionar **LIMPIAR** (o **C**) para resetear

### Explicacion de los modos
| Modo | Que hace | Como se activa |
|------|----------|----------------|
| **AUTO** | La animacion avanza sola cada ~500ms | Automatico al iniciar un algoritmo. Se muestra en verde |
| **MANUAL** | Hay que presionar **>>** (o **N**) para cada paso | Se activa al presionar **||** (o **P**). Se muestra en dorado |
| **PISTAS** | Navegacion guiada por pistas (16 pasos) | Boton **PISTAS** (o **F**) con un nodo seleccionado |

### Controles basicos
- **Click izquierdo** en nodo → seleccionar ubicacion
- **Click derecho + arrastrar** → mover el mapa (pan)
- **Rueda del mouse** → zoom en el mapa / scroll en el arbol de pistas
- **Botones** en la barra inferior o sus **teclas** asociadas

---

## Tecnologias usadas

| Tecnologia | Version | Por que se usa |
|------------|---------|----------------|
| **C++17** | ISO C++ 2017 | Requisito academico. Se usa `-std=c++17` por soporte de inicializadores, `auto`, `constexpr`, y `nullptr`. Todo el codigo es C++ puro sin STL — las estructuras (lista, cola, pila, diccionario, grafo, arbol) estan implementadas manualmente con arreglos dinamicos y punteros |
| **raylib** | 5.x | Libreria grafica minimalista y portable para ventanas, input, y renderizado 2D/3D. Se eligio sobre SDL o SFML por su API simple (todo en un header), sin necesidad de frameworks de UI ni sistemas de eventos complejos. Maneja `InitWindow`, `DrawCircle`, `DrawText`, `GetMousePosition`, input de teclado, y el game loop |
| **GLFW3** | 3.x | Dependencia de raylib para manejo de ventanas y contexto OpenGL en Windows. Se instala via MSYS2 junto con raylib |
| **OpenGL + GDI32 + WinMM** | — | Librerias del sistema para el pipeline grafico. OpenGL para renderizado acelerado, GDI32/WinMM para integracion con Windows |
| **MinGW-w64 (MSYS2)** | g++ 15+ | Entorno de compilacion en Windows. Provee GCC, GNU Make, y las herramientas POSIX minimas necesarias. Se usa `mingw32-make` en lugar de `make` por compatibilidad con Windows |
| **GNU Make** | 4.4+ | Automatizacion de compilacion via Makefile con reglas para `.cpp -> .o -> .exe`. Separacion entre `CXXFLAGS` (compilacion) y `LDFLAGS` (linkeo) |

---

## Estructura del proyecto

```
tesoropirata/
  include/         7 headers   --- definiciones de estructuras de datos
  src/             8 .cpp      --- implementaciones
  graphics/        9 archivos  --- renderizado con raylib
  core/            4 archivos  --- logica del juego
  data/            3 archivos  --- grafo, pistas, resultados
  main.cpp                   --- entry point + game loop
  Makefile                   --- compilacion con g++
  README.md                  --- este archivo
```

---

## include/ --- Headers de estructuras de datos

### lista.hpp --- Lista doblemente enlazada

Nodos con punteros a `anterior` y `siguiente`, cabeza y cola, y un contador `tam`. Permite insertar al final en O(1), acceso por indice en O(n), busqueda lineal, y liberacion total con `vaciar()`. Se usa para almacenar la ruta optima al tesoro producida por BFS, DFS o Dijkstra, y para caminos intermedios durante la reconstruccion desde el arreglo de padres.

### cola.hpp --- Cola circular

Arreglo dinamico con indices `frente` y `final` que permite encolar y desencolar en O(1) mediante aritmetica modular. Capacidad fija de 100. Se usa como frontera (frontier) del algoritmo BFS: los nodos por explorar se encolan y cada paso se desencola el frente. La clase se expone al panel grafico para mostrar su contenido en vivo como barras apiladas con nombres de nodos.

### pila.hpp --- Pila dinamica

Arreglo dinamico con indice `tope`. Operaciones push/pop en O(1). Capacidad fija de 100. Se usa como frontera del algoritmo DFS: los nodos se apilan y cada paso se desapila el tope. Se expone al panel grafico para visualizacion en vivo como barras apiladas.

### diccionario.hpp --- Tabla hash abierta

Arreglo de 101 buckets, cada uno una lista enlazada de `ParClaveValor` (clave texto + valor texto). La funcion hash suma los codigos ASCII de los caracteres de la clave modulo 101. Inserta al inicio del bucket. Se usa para almacenar la relacion `ubicacion -> pista` cargada desde `pistas.txt`. Consulta rapida O(1) promedio para mostrar la pista del nodo seleccionado.

### grafo.hpp --- Grafo ponderado no dirigido

Arreglo dinamico de `NodoGrafo`, cada uno con un nombre y una lista enlazada de `Arista` (destino, peso, siguiente). Permite agregar nodos, agregar aristas con peso, buscar por nombre, y consultar adyacencias. Se carga desde `grafo.txt` y representa el mapa de la isla de 28 nodos. Es la estructura central sobre la que operan BFS, DFS y Dijkstra.

### arbol.hpp --- Arbol de pistas n-ario

Cada `NodoArbol` tiene una pista (texto), un destino (nombre de ubicacion), un arreglo dinamico de hijos y un puntero al padre. Los metodos principales son `crearRaiz()`, `agregarHijo()` y `interpretar()` (busca una pista y retorna el destino asociado). Se construye como una cadena lineal de 16 pistas desde `playa` hasta `tesoro`. Alimenta la visualizacion grafica en el panel inferior derecho.

### archivos.hpp --- File I/O

Cuatro funciones libres: `cargarGrafo()` lee `data/grafo.txt` con formato `origen:destino(peso),...`; `cargarPistas()` lee `data/pistas.txt` al diccionario; `cargarArbolPistas()` construye el arbol como cadena anidada; `guardarResultado()` escribe `data/resultado.txt` con la ruta encontrada. Todas retornan `bool` para verificacion de errores.

### explorador.hpp --- BFS, DFS, Dijkstra paso a paso

Contiene `configurar()` para vincular referencias al grafo, cola y pila, y tres metodos de animacion paso a paso: `bfsPaso()`, `dfsPaso()` y `dijkstraPaso()`. Cada uno recibe el paso actual por parametro y avanza un unico paso del algoritmo (inicializa en paso 0, procesa un nodo en cada paso siguiente). Retornan `true` cuando encuentran el destino. `reconstruirCamino()` arma la ruta desde el arreglo de padres. No existen versiones batch completas — solo paso a paso para la animacion.

---

## core/ --- Logica del juego

### juego.hpp / juego.cpp

Orquesta toda la logica del programa. Define `EstadoJuego` con 5 estados: `INICIO`, `EXPLORANDO_BFS`, `EXPLORANDO_DFS`, `EXPLORANDO_DIJKSTRA`, `NAVEGANDO_PISTAS` y `COMPLETADO`. La clase `Juego` mantiene el grafo, diccionario, arbol, cola/pila para animacion, ruta optima, y el explorador. Provee metodos publicos: `cargarDatos()` para lectura de archivos, `seleccionarNodo()` para fijar el nodo clickeado, `iniciarBFS/DFS/Dijkstra()` para preparar estado, `iniciarNavegacionPistas()` para el recorrido por pistas, `pasoAnimacion()` que avanza un paso segun el estado actual, y `limpiar()` para reiniciar todo. Incluye getters para que la capa grafica consulte visitados, cola, pila, ruta, pista activa, nodo procesando, etc. Los helpers privados `iniciarAlgoritmo()` y `finalizarExploracion()` eliminan la duplicacion de codigo entre los tres algoritmos.

---

## graphics/ --- Capa grafica (raylib)

### colores.hpp --- Paleta de colores

Todos son tipo `Color` de raylib (RGBA 0-255):

| Constante             | RGB           | Uso                            |
|-----------------------|---------------|--------------------------------|
| COLOR_FONDO           | 24, 35, 60    | Fondo degradado superior       |
| COLOR_NODO_NO_VIS     | 130, 130, 150 | Nodo no visitado               |
| COLOR_NODO_EN_COLA    | 255, 220, 60  | Nodo en cola/pila (frontier)   |
| COLOR_NODO_VISITADO   | 80, 200, 120  | Nodo ya visitado               |
| COLOR_NODO_PROCESANDO | 255, 140, 0   | Nodo siendo procesado ahora    |
| COLOR_NODO_RUTA       | 220, 60, 60   | Ruta final                     |
| COLOR_NODO_SELECCION  | 80, 150, 255  | Color del anillo de seleccion  |
| COLOR_TEXTO           | 240, 240, 250 | Texto principal                |
| COLOR_TEXTO_OSCURO    | 180, 190, 210 | Texto secundario               |
| COLOR_ARISTA          | 90, 110, 140  | Aristas del grafo              |
| COLOR_ARISTA_RUTA     | 255, 100, 100 | Aristas en ruta final          |
| COLOR_TESORO          | 255, 215, 0   | Nodo tesoro (dorado)           |
| COLOR_PANEL_FONDO     | 20, 28, 50    | Fondo de paneles               |
| COLOR_BOTON           | 60, 60, 90    | Boton inactivo                 |
| COLOR_ARBOL_LINEA     | 150, 150, 180 | Lineas del arbol               |
| COLOR_ARBOL_ACTIVO    | 255, 200, 60  | Nodo activo del arbol          |
| COLOR_COLA_BARRA      | 255, 200, 0   | Barras de cola (BFS)           |
| COLOR_PILA_BARRA      | 200, 100, 0   | Barras de pila (DFS)           |

**Prioridad de colores en pantalla:**
1. Rojo (en ruta final)
2. Naranja (procesando ahora)
3. Verde (visitado)
4. Amarillo (en cola/pila)
5. Dorado (tesoro)
6. Gris (no visitado)

---

### renderizador.hpp / renderizador.cpp --- Renderizado del grafo con pan/zoom

Mantiene un arreglo `NodoVisual` paralelo a los nodos del grafo, cada uno con posicion (Vector2), color y radio. Calcula el layout inicial como una rejilla (el grafo no tiene coordenadas inherentes). Provee transformacion de coordenadas mundo ↔ pantalla mediante `aplicarTransform()` (zoom + offset) y `deshacerTransform()` para el hit test. Dibuja aristas con `DrawLineEx` y etiquetas de peso, nodos como circulos con bordes y anillos de seleccion, nombres con `DrawText`, y una leyenda semitransparente. `sincronizarConJuego()` consulta el estado de cada nodo (visitado, en ruta, en cola, procesando) y asigna colores con prioridad: rojo ruta > naranja procesando > verde visitado > amarillo frontier > dorado tesoro. `nodoBajoMouse()` invierte la transformacion antes de medir distancia, eliminando el efecto del zoom. Soporta pan con click derecho + arrastrar y zoom con la rueda del mouse.

---

### panel_info.hpp / panel_info.cpp --- Panel de informacion

Panel superior derecho con titulo "BITACORA DE EXPLORACION" en dorado. Recibe los datos del juego y los muestra en formato de texto: nombre del nodo seleccionado, pista del nodo, costo acumulado, estado actual, cantidad de visitados, paso actual, nodo siendo procesado, y la ruta encontrada como texto (`playa -> bosque -> ...`). En la mitad inferior dibuja la cola (BFS) o la pila (DFS) como barras horizontales apiladas donde cada barra es un elemento con el nombre del nodo, y el color decrece en intensidad de arriba a abajo. La barra superior de la cola es el frente (proximo a salir); la de la pila es el tope (ultimo en entrar).

---

### animador.hpp / animador.cpp --- Control de animacion

Gestiona el temporizador de la animacion. Su metodo `actualizar()` se llama en cada frame y retorna `true` cuando ha pasado el intervalo configurado (default 500ms), momento en el que `main.cpp` llama a `juego.pasoAnimacion()`. `iniciar()` activa la reproduccion automatica y resetea contadores, `pausar()`/`reanudar()` permiten el modo manual, y `setVelocidadRelativa()` ajusta ±100ms entre 50 y 3000. En la barra inferior dibuja "AUTO REPRODUCIENDO" en verde o "MANUAL PAUSADO" en dorado, junto con el paso actual sobre el total y la velocidad en milisegundos.

---

### ui_arbol.hpp / ui_arbol.cpp --- Visualizacion del arbol de pistas

Panel inferior derecho con titulo "ARBOL DE PISTAS". Recibe el arbol de pistas (cadena de 16 nodos) y lo dibuja como una columna vertical con lineas de conexion entre niveles. Cada nodo se representa como un circulo con el nombre de la ubicacion destino adentro y el texto de la pista debajo. Si el nodo tiene un solo hijo (como en la cadena lineal), se dibuja centrado verticalmente. `setNodoActivo()` resalta la pista actual en amarillo con texto negro; `limpiarNodoActivo()` quita el resaltado. Soporta scroll con la rueda del mouse. La leyenda inferior dice "Cada pista lleva a la siguiente ubicacion".

---

## main.cpp --- Entry point y game loop

Punto de entrada del programa. En la fase de **inicializacion** crea las instancias de `Juego` (que a su vez carga grafo, pistas y arbol), `Renderizador` (calcula layout de nodos), `PanelInfo` (bitacora superior derecha), `UIArbol` (arbol inferior derecho), `Animador` (control de reproduccion), y los 13 botones de la barra inferior. Abre la ventana a 1600x900 con `InitWindow` y fija 60 FPS.

El **game loop** se repite hasta que el usuario cierra la ventana. Cada iteracion:
1. **Input**: procesa click izquierdo (selecciona nodo via hit test), click derecho (arrastra mapa), rueda (zoom en grafo, scroll en arbol), botones y teclas (BFS/DFS/Dijkstra/Pistas, play/pause, paso manual, velocidad, limpiar, guardar, reset vista, salir)
2. **Sincronizacion**: `animador.actualizar()` decide si avanza un paso de animacion, `renderizador.sincronizarConJuego()` actualiza colores de nodos, `uiArbol.setNodoActivo()` resalta la pista actual
3. **Render**: dibuja fondo degradado, titulo dorado, mapa (grafo con colores), bitacora (panel derecho superior), arbol de pistas (panel derecho inferior), barra de botones, controles de animacion (AUTO/MANUAL, paso, velocidad), zoom porcentual, y mensaje de ayuda si no hay nodo seleccionado

---

## data/ --- Archivos de datos

### grafo.txt --- Mapa de la isla (28 nodos, 4 regiones)

```
dirigido: false
nodos: playa,pueblo_pirata,bahia,acantilado,r_caleta,r_costa,p_bosque,montana,desfiladero,cima_arbol,cueva,caverna,bosque,l_esmeralda,rio,cascada,valle,pantano,ruinas,laguna,templo,mirador,atalaya,fortin,mercado,ermita,gruta_coral,tesoro

# REGION 1 --- COSTA
playa:         pueblo_pirata(2), bahia(4), acantilado(6)
pueblo_pirata: bahia(3), r_caleta(5)
bahia:         acantilado(4), r_caleta(3), gruta_coral(7)
acantilado:    r_costa(5), desfiladero(9)
r_caleta:      p_bosque(3), r_costa(4)
r_costa:       p_bosque(2), bosque(6)

# REGION 2 --- MONTANAS
p_bosque:      montana(4), bosque(3)
montana:       desfiladero(3), cueva(2)
desfiladero:   cima_arbol(5), valle(7)
cima_arbol:    mirador(4), cascada(8)
cueva:         bosque(4), caverna(3)
caverna:       laguna(5), pantano(7)

# REGION 3 --- BOSQUE Y RIO
bosque:        rio(3), l_esmeralda(2)
rio:           cascada(4), valle(5), pantano(6)
cascada:       valle(3), laguna(8)
valle:         ruinas(4), laguna(5)
l_esmeralda:   pantano(4), laguna(6)
pantano:       ruinas(3), mirador(6)

# REGION 4 --- INTERIOR
ruinas:        laguna(3), templo(4)
laguna:        templo(2), mirador(5)
templo:        atalaya(3), fortin(5)
mirador:       atalaya(2), ermita(6)
atalaya:       fortin(3), mercado(5)
fortin:        mercado(2), ermita(4)
mercado:       ermita(3), gruta_coral(8)
ermita:        tesoro(2)
gruta_coral:   tesoro(5)
```

**Formato:**
- `dirigido: true|false` --- modo del grafo
- `nodos: nombre1,nombre2,...` --- lista de nodos
- `origen:destino1(peso1),destino2(peso2),...` --- aristas con peso
- Lineas con `#` son comentarios ignorados

### pistas.txt --- Pistas por ubicacion (formato 3 campos)

```
# CADENA: playa -> pueblo_pirata -> r_caleta -> ... -> tesoro
playa:Los pescadores del pueblo saben algo:pueblo_pirata
pueblo_pirata:El humo en la caleta escondida senala el camino:r_caleta
r_caleta:Las huellas en el barro llevan al bosque espeso:p_bosque
p_bosque:Los arboles mas viejos guardan el secreto:bosque
bosque:Sigue el murmullo del agua corriente:rio
rio:Remonta la corriente hasta que el agua caiga del cielo:cascada
cascada:El velo de agua oculta un pasaje:valle
valle:Entre las colinas verdes yacen piedras antiguas:ruinas
ruinas:El reflejo en el ojo de la estatua apunta a la laguna:laguna
laguna:El templo emerge entre la niebla matutina:templo
templo:Desde la torre mas alta se divisa una luz:atalaya
atalaya:La fortaleza junto al acantilado resguarda un secreto:fortin
fortin:El cofre del capitan esta vacio, la llave esta en el mercado:mercado
mercado:El ermitano del cerro conoce el ultimo paso:ermita
ermita:El tesoro brilla bajo la luz del sol poniente:tesoro
tesoro:!Lo has encontrado!:tesoro
```

**Formato:** `ubicacion:pista_texto:destino_siguiente` (16 entradas en cadena).
- El `Diccionario` mapea `ubicacion -> pista_texto` (primeros 2 campos).
- El `Arbol` mapea `pista_texto -> destino_siguiente` (ultimos 2 campos), formando una cadena anidada que guia desde `playa` hasta `tesoro`.

### resultado.txt --- Output generado

```
RUTA AL TESORO
==============

Inicio: playa
Ruta: playa -> bosque -> rio -> cascada -> valle -> ruinas -> laguna -> templo -> atalaya -> fortin -> mercado -> ermita -> tesoro
Costo total: 40
Visitados: 12 nodos
Algoritmo usado: BFS
```

**Generado por:** `guardarResultado()` al presionar **S** o boton GUARDAR en el juego. Muestra la ruta encontrada con nombres de nodos, costo total, visitados y algoritmo usado.

---

## Makefile --- Compilacion

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

| Entrada        | Accion                          | Descripcion tecnica                                          |
|----------------|---------------------------------|--------------------------------------------------------------|
| Click nodo     | seleccionarNodo()               | Hit test con distancia euclidiana en coordenadas mundo       |
| Click der + arrastrar | pan()                   | Arrastra el mapa (offset en Renderizador)                    |
| Rueda mouse    | Zoom / Scroll                   | Zoom en grafo, scroll en arbol de pistas                     |

### Botones en pantalla

| Boton     | Funcion                     | Tecla |
|-----------|-----------------------------|-------|
| BFS       | Inicia BFS desde nodo seleccionado | B |
| DFS       | Inicia DFS desde nodo seleccionado | D |
| DIJKSTRA  | Inicia Dijkstra desde nodo seleccionado | R |
| PISTAS    | Inicia navegacion por pistas | F |
| \|\|      | Pausa/Reanuda (toggle AUTO/MANUAL) | P |
| >>        | Paso manual (solo en MANUAL) | N |
| AUTO      | Reinicia reproduccion automatica | - |
| -         | Mas lento (+100ms)          | - (tecla) |
| +         | Mas rapido (-100ms)          | + (tecla) |
| LIMPIAR   | Limpia todo (estados, colores, arbol, seleccion) | C |
| GUARDAR   | Guarda ruta en resultado.txt | S |
| RESET     | Reset zoom/pan/scroll        | V |
| SALIR     | Cierra programa              | Esc |

---

## Algoritmos implementados

| Algoritmo | Estructura usada | Complejidad | Descripcion |
|-----------|-----------------|-------------|-------------|
| **BFS**   | Cola circular   | O(V + E)    | Recorrido por niveles. Usa `bfsPaso()` para animacion paso a paso |
| **DFS**   | Pila dinamica   | O(V + E)    | Recorrido en profundidad. Usa `dfsPaso()` para animacion paso a paso, con arreglo `expandidos` para control de retroceso |
| **Dijkstra** | Arreglo (busqueda lineal del minimo) | O(V^2) | Camino mas corto desde nodo inicio a tesoro. Version paso a paso con `dijkstraPaso()` |
| **Pistas** | Arbol n-ario + Diccionario | O(N) | Navegacion guiada por pistas. Cada paso: leer pista del nodo actual -> interpretar en arbol -> saltar al siguiente nodo. 16 pasos desde playa hasta tesoro |

---

## Pantalla

```
+------------------------------------------+-----------------------------+
|                                          |  BITACORA DE EXPLORACION   |
|   Mapa del grafo (28 nodos)             |  - Nodo seleccionado       |
|   Pan + Zoom + click seleccion          |  - Pista actual            |
|                                          |  - Costo acumulado         |
|   Colores nodo:                         |  - Estado de exploracion   |
|   Gris  = no visitado                   |  - Procesando: [nombre]    |
|   Naranja = procesando ahora            |  - Ruta encontrada         |
|   Verde  = visitado                     |                             |
|   Rojo   = ruta final                   |  [COLA (BFS)]  [PILA (DFS)]|
|   Amarillo = en cola/pila (frontier)    |  barras visibles en vivo   |
|   Dorado = tesoro                       +-----------------------------+
|   Anillos blancos = seleccionado        |  ARBOL DE PISTAS           |
|                                          |  (pista activa resaltada)  |
|   Aristas rojas = ruta final            |  "Cada pista lleva a la    |
|                                          |   siguiente ubicacion"     |
+------------------------------------------+-----------------------------+
| [BFS][DFS][DIJKSTRA][PISTAS] [||][>>][AUTO] [-][+] [LIMPIAR][GUARDAR][RESET][SALIR] |
| AUTO REPRODUCIENDO / MANUAL PAUSADO  -  Paso N/N  -  Vel NNNms                    |
+------------------------------------------------------------------------------+
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
  |           |-- include/cola.hpp
  |           |-- include/pila.hpp
  |           |-- include/lista.hpp
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
  |-- graphics/animador.hpp
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
- **Build**: GNU Make 4.4+ (via `mingw32-make`)
