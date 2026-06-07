# El Tesoro del Pirata

Proyecto academico de Estructuras de Datos y Algoritmos en C++17 con representacion grafica en vivo usando raylib 5.x bajo MinGW-w64 (MSYS2). El programa ayuda a un pirata a encontrar un tesoro escondido en una isla: el mapa se representa como **grafo**, se ejecutan **BFS/DFS/Dijkstra** paso a paso con animacion, y se interpretan **pistas** mediante un arbol encadenado. Soporta multiples mapas cargados desde subdirectorios en `maps/`. El mapa se renderiza sobre una imagen de fondo con coordenadas virtuales normalizadas (0–1000), zoom/pan sincronizado, y edicion interactiva de posiciones de nodos. Todas las estructuras de datos fueron implementadas manualmente sin STL.

---

## Relacion con la consigna

El siguiente cuadro mapea cada requisito de la consigna a su implementacion concreta en el codigo:

### Requisitos de datos

| Requisito | Implementacion |
|-----------|----------------|
| **1. Grafo** — representar el mapa y sus conexiones | `include/grafo.hpp` + `src/grafo.cpp` — Arreglo dinamico de `NodoGrafo` con lista enlazada de `Arista` (destino, peso). Soporta nodos con nombre, coordenadas virtuales y aristas ponderadas. Se carga desde `grafo.txt` |
| **2. Arbol** — interpretar pistas mediante un arbol de decisiones | `include/arbol.hpp` + `src/arbol.cpp` — Arbol n-ario donde cada `NodoArbol` tiene una pista (texto) y un destino. Se construye como cadena lineal desde `playa` hasta `tesoro`. El metodo `interpretar()` busca una pista y retorna el destino asociado |
| **3. Cola/Pila** — gestionar el orden de exploracion (BFS/DFS) | `include/cola.hpp` (cola circular, capacidad 100) para BFS y `include/pila.hpp` (pila dinamica, capacidad 100) para DFS. Ambas se exponen al panel grafico para visualizacion en vivo como barras apiladas |
| **4. Diccionario/Tabla Hash** — almacenar informacion de ubicaciones (nombre, pistas) | `include/diccionario.hpp` + `src/diccionario.cpp` — Tabla hash abierta de 101 buckets con lista enlazada por bucket. Hash por suma ASCII modulo 101. Almacena la relacion `ubicacion -> pista_texto` |
| **5. Lista** — guardar el camino optimo al tesoro | `include/lista.hpp` + `src/lista.cpp` — Lista simplemente enlazada con cabeza, cola y contador `tam`. Insercion al final O(1). Almacena la ruta optima reconstruida desde el arreglo de padres |
| **6. Archivos** — leer pistas desde archivo y guardar el resultado | `include/archivos.hpp` + `src/archivos.cpp` — Seis funciones de I/O: `cargarGrafo()`, `cargarPistas()`, `cargarArbolPistas()`, `cargarCoordenadas()`, `guardarCoordenadas()`, `guardarResultado()`. Todas retornan `bool` para verificacion de errores |

### Tareas del programa

| Tarea | Implementacion |
|-------|----------------|
| **1. Cargar el grafo del mapa desde un archivo** | `Juego::cargarDatos(basePath)` en `core/juego.cpp` construye rutas como `maps/isla_tortuga/grafo.txt` y llama a `cargarGrafo()`. El manager `MapaManager` escanea el directorio `maps/` y permite seleccionar entre distintos mapas |
| **2. Usar BFS/DFS para explorar la isla** | `Explorador::bfsPaso()` y `Explorador::dfsPaso()` en `src/explorador.cpp`. Cada uno avanza un unico paso del algoritmo para permitir animacion en vivo. Se activan con los botones **BFS** (B) y **DFS** (D) |
| **3. Interpretar las pistas** | `NodoArbol::interpretar()` recibe un texto de pista y retorna el nombre de la ubicacion destino. Combinado con el `Diccionario` que mapea `ubicacion -> pista`, permite navegar la cadena completa desde `playa` hasta `tesoro` |
| **4. Encontrar el camino mas corto o la secuencia correcta de pistas** | Tres algoritmos disponibles: **BFS** (camino por niveles), **DFS** (camino en profundidad), **Dijkstra** (camino mas corto con busqueda lineal del minimo, O(V^2)). Ademas, el modo **PISTAS** (F) recorre la cadena de 16 pistas paso a paso |
| **5. Guardar la ruta al tesoro en otro archivo** | `guardarResultado()` escribe `maps/<mapa_actual>/resultado.txt` con la ruta optima, costo total, visitados y algoritmo usado. Se activa con el boton **GUARDAR** (S) |

---

## Guia de usuario

### Quick start
1. Abrir `tesoro.exe` (o compilar con `mingw32-make`)
2. Seleccionar un mapa del combo box ubicado sobre la barra de botones (`Mapa: isla_tortuga >`)
3. Hacer clic en un nodo del mapa (ej: playa) — se marca con anillos blancos
4. Presionar **BFS** (o tecla **B**) para iniciar la busqueda desde ese nodo
5. La animacion arranca automaticamente en modo **AUTO**
6. Al llegar al tesoro, la ruta optima se pinta en **rojo** en el mapa
7. Presionar **LIMPIAR** (o **C**) para reiniciar y probar otro algoritmo
8. Presionar **EDITAR** (o **E**) para arrastrar nodos y reposicionarlos
9. Presionar **K** o boton **G.CORDS** para guardar las coordenadas ajustadas

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
| **MANUAL** | Hay que presionar **>>** (o **N**) para cada paso | Se activa al presionar **\|\|** (o **P**). Se muestra en dorado |
| **PISTAS** | Navegacion guiada por pistas (16 o 6 pasos segun el mapa) | Boton **PISTAS** (o **F**) con un nodo seleccionado |
| **EDITAR** | Arrastrar nodos con el mouse para reposicionarlos | Boton **EDITAR** (o **E**). Guardar con **G.CORDS** o **K** |

### Seleccion de mapa
Sobre la barra de botones hay un combo box que muestra el mapa actual. Al hacer clic se despliega la lista de mapas disponibles (escaneados del directorio `maps/`). Cada mapa es una isla distinta con su propio grafo, pistas, coordenadas e imagen de fondo. Al cambiar de mapa, todos los estados se reinician y el grafo se recarga completamente.

### Controles basicos
- **Click izquierdo** en nodo → seleccionar ubicacion (o arrastrar en modo edicion)
- **Click derecho + arrastrar** → mover el mapa (pan), el fondo se mueve con los nodos
- **Rueda del mouse** → zoom sincronizado (mapa + nodos) en el grafo / scroll en arbol de pistas
- **Botones** en la barra inferior o sus **teclas** asociadas

---

## Tecnologias usadas

| Tecnologia | Version | Por que se usa |
|------------|---------|----------------|
| **C++17** | ISO C++ 2017 | Requisito academico. Se usa `-std=c++17` por soporte de inicializadores, `auto`, `constexpr`, y `nullptr`. Todo el codigo es C++ puro sin STL — las estructuras (lista, cola, pila, diccionario, grafo, arbol) estan implementadas manualmente con arreglos dinamicos y punteros |
| **raylib** | 5.x | Libreria grafica minimalista y portable para ventanas, input, y renderizado 2D/3D. Maneja `InitWindow`, `DrawCircle`, `DrawText`, `GetMousePosition`, input de teclado, texturas, y el game loop |
| **GLFW3** | 3.x | Dependencia de raylib para manejo de ventanas y contexto OpenGL en Windows. Se instala via MSYS2 junto con raylib |
| **OpenGL + GDI32 + WinMM** | — | Librerias del sistema para el pipeline grafico. OpenGL para renderizado acelerado, GDI32/WinMM para integracion con Windows |
| **MinGW-w64 (MSYS2)** | g++ 15+ | Entorno de compilacion en Windows. Provee GCC, GNU Make, y las herramientas POSIX minimas necesarias. Se usa `mingw32-make` en lugar de `make` por compatibilidad con Windows |
| **GNU Make** | 4.4+ | Automatizacion de compilacion via Makefile con reglas para `.cpp -> .o -> .exe`. Separacion entre `CXXFLAGS` (compilacion) y `LDFLAGS` (linkeo) |

---

## Estructura del proyecto

```
tesoropirata/
  include/         8 headers   --- definiciones de estructuras de datos
  src/             8 .cpp      --- implementaciones
  graphics/        9 archivos  --- renderizado con raylib
  core/            4 archivos  --- logica del juego (juego + mapa_manager)
  maps/            N subdirs   --- cada isla con sus datos (grafo, pistas, coords, fondo, resultado)
  main.cpp                   --- entry point + game loop
  Makefile                   --- compilacion con g++
  README.md                  --- este archivo
```

---

## include/ --- Headers de estructuras de datos (8 headers)

### lista.hpp --- Lista simplemente enlazada

Nodos con puntero a `siguiente`, cabeza y cola, y un contador `tam`. Permite insertar al final en O(1), acceso por indice en O(n), busqueda lineal, y liberacion total con `vaciar()`. Se usa para almacenar la ruta optima al tesoro producida por BFS, DFS o Dijkstra, y para caminos intermedios durante la reconstruccion desde el arreglo de padres.

### cola.hpp --- Cola circular

Arreglo dinamico con indices `frente` y `final` que permite encolar y desencolar en O(1) mediante aritmetica modular. Capacidad fija de 100. Se usa como frontera (frontier) del algoritmo BFS: los nodos por explorar se encolan y cada paso se desencola el frente. La clase se expone al panel grafico para mostrar su contenido en vivo como barras apiladas con nombres de nodos.

### pila.hpp --- Pila dinamica

Arreglo dinamico con indice `tope`. Operaciones push/pop en O(1). Capacidad fija de 100. Se usa como frontera del algoritmo DFS: los nodos se apilan y cada paso se desapila el tope. Se expone al panel grafico para visualizacion en vivo como barras apiladas.

### diccionario.hpp --- Tabla hash abierta

Arreglo de 101 buckets, cada uno una lista enlazada de `ParClaveValor` (clave texto + valor texto). La funcion hash suma los codigos ASCII de los caracteres de la clave modulo 101. Inserta al inicio del bucket. Se usa para almacenar la relacion `ubicacion -> pista` cargada desde `pistas.txt`. Consulta rapida O(1) promedio para mostrar la pista del nodo seleccionado.

### grafo.hpp --- Grafo ponderado no dirigido

Arreglo dinamico de `NodoGrafo`, cada uno con un nombre, coordenadas virtuales (x, y en rango 0-1000) para posicionamiento sobre el fondo, y una lista enlazada de `Arista` (destino, peso, siguiente). Permite agregar nodos, agregar aristas con peso, buscar por nombre, y consultar adyacencias. Se carga desde `grafo.txt` y opcionalmente `coords.txt`. Representa el mapa de la isla. Es la estructura central sobre la que operan BFS, DFS y Dijkstra.

### arbol.hpp --- Arbol de pistas n-ario

Cada `NodoArbol` tiene una pista (texto), un destino (nombre de ubicacion), un arreglo dinamico de hijos y un puntero al padre. Los metodos principales son `crearRaiz()`, `agregarHijo()` y `interpretar()` (busca una pista y retorna el destino asociado). Se construye como una cadena lineal desde `playa` hasta `tesoro` (16 o 6 pistas segun el mapa). Alimenta la visualizacion grafica en el panel inferior derecho.

### archivos.hpp --- File I/O

Seis funciones que reciben la ruta del archivo como parametro:
- `cargarGrafo()` — lee `grafo.txt` con formato `origen:destino(peso),...`
- `cargarPistas()` — lee `pistas.txt` al diccionario (ubicacion -> pista)
- `cargarArbolPistas()` — construye el arbol como cadena anidada desde el mismo `pistas.txt`
- `cargarCoordenadas()` — lee `coords.txt` con formato `nombre=x y`
- `guardarCoordenadas()` — escribe de vuelta las coordenadas modificadas durante la edicion interactiva
- `guardarResultado()` — escribe `resultado.txt` con la ruta encontrada, costo, visitados y algoritmo

Todas retornan `bool` para verificacion de errores.

### explorador.hpp --- BFS, DFS, Dijkstra paso a paso

Contiene `configurar()` para vincular referencias al grafo, cola y pila, y tres metodos de animacion paso a paso:
- `bfsPaso(nodoInicio, nodoDestino, padres, visitados, frente, tamFrente, colaLocal, paso)` — Recorrido por niveles. En paso 0 inicializa (encola el nodo inicio), en cada paso siguiente desencola el frente, marca como visitado y encola los vecinos no visitados. Retorna `true` al encontrar el destino.
- `dfsPaso(nodoInicio, nodoDestino, padres, visitados, tope, pilaLocal, paso)` — Recorrido en profundidad. En paso 0 apila el inicio, en cada paso desapila el tope y apila vecinos no visitados ni expandidos. Retorna `true` al encontrar el destino.
- `dijkstraPaso(nodoInicio, nodoDestino, padres, distancias, visitados, paso, uActual, cola, tam)` — Camino mas corto. Busqueda lineal del minimo no visitado, relaja sus aristas. Retorna `true` al encontrar el destino.

`reconstruirCamino(destino, padres, camino)` arma la ruta desde el arreglo de padres. No existen versiones batch completas — solo paso a paso para la animacion.

---

## core/ --- Logica del juego

### juego.hpp / juego.cpp

Orquesta toda la logica del programa. Define `EstadoJuego` con 6 estados:

| Estado | Significado |
|--------|------------|
| `INICIO` | Esperando seleccion de nodo |
| `EXPLORANDO_BFS` | Ejecutando BFS paso a paso |
| `EXPLORANDO_DFS` | Ejecutando DFS paso a paso |
| `EXPLORANDO_DIJKSTRA` | Ejecutando Dijkstra paso a paso |
| `NAVEGANDO_PISTAS` | Recorriendo la cadena de pistas |
| `COMPLETADO` | Algoritmo finalizado, ruta encontrada |

La clase `Juego` mantiene el grafo, diccionario, arbol, cola/pila para animacion, ruta optima, y el explorador. Metodos publicos principales:

- `cargarDatos(const char* basePath)` — lee todos los archivos de datos desde la ruta base del mapa activo (construye paths como `maps/isla_tortuga/grafo.txt`)
- `limpiarDatos()` — libera el grafo, diccionario, arbol, cola, pila y ruta, luego llama a `limpiar()`. Se usa al cambiar de mapa
- `seleccionarNodo(int indice)` — fija el nodo clickeado y busca su pista en el diccionario
- `iniciarBFS()`, `iniciarDFS()`, `iniciarDijkstra()` — preparan el estado y el explorador para el algoritmo correspondiente
- `iniciarNavegacionPistas()` — inicia el recorrido por pistas desde el nodo seleccionado
- `pasoAnimacion()` — avanza un paso segun el estado actual (BFS, DFS, Dijkstra o Pistas)
- `guardarResultado(const char* ruta)` — persiste la ruta encontrada
- `guardarCoordenadas(const char* ruta)` — persiste las coordenadas editadas
- `limpiar()` — reinicia todos los estados, colores y seleccion

Incluye getters para que la capa grafica consulte visitados, cola, pila, ruta, pista activa, nodo procesando, etc. Los helpers privados `iniciarAlgoritmo()` y `finalizarExploracion()` eliminan la duplicacion de codigo entre los tres algoritmos.

### mapa_manager.hpp / mapa_manager.cpp

Gestiona la seleccion de mapas. Escanea el directorio `maps/` en busca de subdirectorios que contengan un archivo `grafo.txt` (valida que sean mapas validos). Expone:

- `escanearMapas()` — recorre `maps/` y registra cada subdirectorio con mapa valido. Deriva un nombre legible (ej: `isla_tortuga` → `Isla Tortuga`)
- `setActual(int idx)` — cambia el mapa activo por indice
- `getRutaBase()`, `getRutaCoords()`, `getRutaFondo()`, `getRutaResultado()` — construyen rutas como `maps/isla_tortuga/coords.txt`

Se integra con `main.cpp` mediante un combo box que permite al usuario cambiar de mapa en tiempo de ejecucion. Al cambiar, se reinicia el juego completo, se recarga el grafo, la imagen de fondo y el arbol de pistas.

---

## graphics/ --- Capa grafica (raylib)

### colores.hpp --- Paleta de colores

Todos son tipo `Color` de raylib (RGBA 0-255):

| Constante | RGB | Uso |
|-----------|-----|-----|
| COLOR_FONDO_SUPERIOR | 35, 50, 85 | Color superior del fondo degradado |
| COLOR_FONDO_INFERIOR | 10, 15, 30 | Color inferior del fondo degradado |
| COLOR_NODO_NO_VIS | 70, 80, 110 | Nodo no visitado |
| COLOR_NODO_EN_COLA | 255, 220, 60 | Nodo en cola/pila (frontier) |
| COLOR_NODO_VISITADO | 80, 200, 120 | Nodo ya visitado |
| COLOR_NODO_PROCESANDO | 255, 155, 40 | Nodo siendo procesado ahora |
| COLOR_NODO_RUTA | 220, 60, 60 | Ruta final |
| COLOR_NODO_SELECCION | 80, 150, 255 | Anillo de seleccion |
| COLOR_NODO_HOVER | 180, 220, 255 | Resaltado al pasar el mouse |
| COLOR_NODO_INICIO | 255, 255, 255 | Nodo de inicio |
| COLOR_TEXTO | 240, 240, 250 | Texto principal |
| COLOR_TEXTO_NODO | 255, 255, 255 | Texto sobre nodos |
| COLOR_TEXTO_OSCURO | 180, 190, 210 | Texto secundario |
| COLOR_TEXTO_SOMBRA | 10, 10, 20, 200 | Sombra de texto |
| COLOR_ARISTA | 90, 110, 140 | Aristas del grafo |
| COLOR_ARISTA_RUTA | 255, 100, 100 | Aristas en ruta final |
| COLOR_TESORO | 255, 215, 0 | Nodo tesoro (dorado) |
| COLOR_DORADO | 255, 200, 60 | Dorado generico (titulos, bordes activos) |
| COLOR_MARCO | 120, 90, 40 | Bordes de paneles y botones |
| COLOR_MARCO_BRILLO | 200, 150, 60 | Bordes brillantes |
| COLOR_PANEL_FONDO | 20, 28, 50 | Fondo de paneles |
| COLOR_PANEL_INTERNO | 30, 40, 70 | Fondo interno de paneles |
| COLOR_BOTON | 60, 60, 90 | Boton inactivo |
| COLOR_BOTON_HOVER | 80, 80, 120 | Boton con hover |
| COLOR_ARBOL_LINEA | 150, 150, 180 | Lineas del arbol |
| COLOR_ARBOL_ACTIVO | 255, 200, 60 | Nodo activo del arbol |
| COLOR_ARBOL_TEXTO_PISTA | 210, 215, 235 | Texto de pista en arbol |
| COLOR_COLA_BARRA | 255, 200, 0 | Barras de cola (BFS) |
| COLOR_PILA_BARRA | 200, 100, 0 | Barras de pila (DFS) |
| COLOR_TOOLTIP_FONDO | 10, 10, 20, 230 | Fondo de tooltip |

**Prioridad de colores en pantalla:**
1. Rojo (en ruta final)
2. Naranja (procesando ahora)
3. Verde (visitado)
4. Amarillo (en cola/pila)
5. Dorado (tesoro)
6. Azul claro (hover)
7. Blanco (nodo inicio)
8. Gris oscuro (no visitado)

---

### renderizador.hpp / renderizador.cpp --- Renderizado del grafo con fondo, zoom/pan y modo edicion

Mantiene un arreglo `NodoVisual` paralelo a los nodos del grafo, cada uno con posicion (Vector2), color y radio. Soporta dos modos de layout: coordenadas cargadas desde `coords.txt` (virtuales 0-1000 mapeadas al panel) o rejilla automatica como fallback. Carga la imagen de fondo (`fondo.jpg`) y la dibuja aplicando la misma transformacion (zoom + pan) que los nodos, manteniendo la alineacion perfecta en todo momento.

Provee transformacion de coordenadas mundo ↔ pantalla mediante `aplicarTransform()` (zoom + offset) y `deshacerTransform()` para el hit test. Dibuja aristas con `DrawLineEx` y etiquetas de peso, nodos como circulos con bordes y anillos de seleccion, nombres con `DrawText`, tooltips informativos, y una leyenda semitransparente.

`sincronizarConJuego()` consulta el estado de cada nodo (visitado, en ruta, en cola, procesando, hover) y asigna colores con prioridad. `nodoBajoMouse()` invierte la transformacion antes de medir distancia. `dibujarTooltip()` muestra informacion del nodo bajo el mouse.

Incluye modo de edicion: al activarlo, los nodos pueden arrastrarse con el mouse (`iniciarArrastreNodo()`, `actualizarArrastreNodo()`, `finalizarArrastreNodo()`) y sus coordenadas virtuales se actualizan en tiempo real en el grafo. Soporta pan con click derecho + arrastrar y zoom con la rueda del mouse entre 40% y 250%. `recargarGrafo()` permite cambiar el grafo subyacente al seleccionar otro mapa.

---

### panel_info.hpp / panel_info.cpp --- Panel de informacion

Panel superior derecho con titulo "BITACORA DE EXPLORACION" en dorado. Recibe los datos del juego y los muestra en formato de texto: nombre del nodo seleccionado, pista del nodo, costo acumulado, estado actual, cantidad de visitados, paso actual, nodo siendo procesado, y la ruta encontrada como texto (`playa -> bosque -> ...`).

En la mitad inferior dibuja la cola (BFS) o la pila (DFS) como barras horizontales apiladas donde cada barra es un elemento con el nombre del nodo, y el color decrece en intensidad de arriba a abajo. La barra superior de la cola es el frente (proximo a salir); la de la pila es el tope (ultimo en entrar). Los metodos `dibujarColaVisual()` y `dibujarPilaVisual()` manejan el renderizado por separado.

---

### animador.hpp / animador.cpp --- Control de animacion

Gestiona el temporizador de la animacion. Su metodo `actualizar()` se llama en cada frame y retorna `true` cuando ha pasado el intervalo configurado (default 500ms), momento en el que `main.cpp` llama a `juego.pasoAnimacion()`. `iniciar(totalPasos)` activa la reproduccion automatica y resetea contadores. `pausar()`/`reanudar()` permiten el modo manual. `setVelocidadRelativa(delta)` ajusta ±100ms con limite entre 50 y 3000ms.

En la barra inferior dibuja "AUTO REPRODUCIENDO" en verde o "MANUAL PAUSADO" en dorado, junto con el paso actual sobre el total de pasos y la velocidad en milisegundos.

---

### ui_arbol.hpp / ui_arbol.cpp --- Visualizacion del arbol de pistas

Panel inferior derecho con titulo "ARBOL DE PISTAS". Recibe el arbol de pistas y lo dibuja como una columna vertical con lineas de conexion entre niveles. Cada nodo se representa como un circulo con el nombre de la ubicacion destino adentro y el texto de la pista debajo. Si el nodo tiene un solo hijo (como en la cadena lineal), se dibuja centrado verticalmente.

`setNodoActivo(const char* pista)` resalta la pista actual en amarillo con texto negro; `limpiarNodoActivo()` quita el resaltado. Soporta scroll con la rueda del mouse mediante `actualizarScroll()`. La leyenda inferior dice "Cada pista lleva a la siguiente ubicacion".

---

## main.cpp --- Entry point y game loop

Punto de entrada del programa.

**Inicializacion:**
1. Crea `Juego` (gestiona logica, estado y datos)
2. Crea `MapaManager` que escanea `maps/` en busca de subdirectorios con mapas validos
3. Toma la ruta base del primer mapa y llama a `juego.cargarDatos(basePath)`
4. Crea `Renderizador` (asociado al grafo), `PanelInfo` (bitacora superior derecha), `UIArbol` (arbol inferior derecho), `Animador` (control de reproduccion), y los 15 botones de la barra inferior
5. Carga la imagen de fondo del mapa activo: `renderizador.cargarFondo("maps/isla_tortuga/fondo.jpg")`
6. Abre la ventana a 1600x900 con `InitWindow` y fija 60 FPS
7. Inicializa el combo box de seleccion de mapa sobre la barra de botones

**Game loop** (se repite hasta cerrar la ventana):

1. **Input**: procesa click izquierdo (selecciona nodo via hit test, arrastra en modo edicion, o interactua con el combo box de mapas y sus items del dropdown), click derecho (arrastra mapa con fondo sincronizado), rueda (zoom en grafo segun posicion del mouse, scroll en arbol si el mouse esta sobre el panel del arbol), botones y teclas (BFS/DFS/Dijkstra/Pistas, play/pause, paso manual, velocidad, limpiar, guardar, editar, guardar coordenadas, reset vista, salir)

2. **Cambio de mapa**: al seleccionar un mapa distinto en el dropdown, se llama a `juego.limpiarDatos()`, `juego.cargarDatos(nuevaRuta)`, `renderizador.recargarGrafo()`, `renderizador.cargarFondo()`, se reinicia la animacion y el modo edicion

3. **Sincronizacion**: `animador.actualizar()` decide si avanza un paso de animacion, `renderizador.sincronizarConJuego()` actualiza colores de nodos, `uiArbol.setNodoActivo()` resalta la pista actual

4. **Renderizado** (en orden):
   - Fondo degradado de pantalla completa
   - Titulo "EL TESORO DEL PIRATA" en dorado con sombra
   - Mapa del grafo con colores sobre el fondo (renderizador)
   - Tooltip del nodo bajo el mouse
   - Bitacora de exploracion (panel derecho superior)
   - Arbol de pistas (panel derecho inferior)
   - Combo box de seleccion de mapa
   - Barra de botones (15 botones con separadores visuales)
   - Dropdown de mapas (si esta abierto)
   - Controles de animacion (AUTO/MANUAL, paso, velocidad)
   - Zoom porcentual
   - Mensajes de ayuda ("Haz CLICK en un nodo...", "MODO EDICION...")

---

## maps/ --- Archivos de datos

El programa soporta multiples mapas almacenados como subdirectorios dentro de `maps/`. Cada subdirectorio representa una isla distinta y contiene sus propios archivos de datos.

### Estructura de un mapa

```
maps/isla_prueba/       --- Mapa de prueba con 19 nodos y cadena de 16 pistas
maps/isla_tortuga/      --- Mapa de la isla tortuga con 13 nodos y cadena de 6 pistas
```

Cada subdirectorio de mapa contiene:

| Archivo | Descripcion |
|---------|-------------|
| `grafo.txt` | Definicion del grafo (nodos, aristas, pesos) |
| `coords.txt` | Coordenadas virtuales de cada nodo (0-1000) |
| `pistas.txt` | Pistas que guian desde playa hasta tesoro |
| `fondo.jpg` | Imagen de fondo del mapa |
| `resultado.txt` | Output generado al presionar GUARDAR |

### grafo.txt --- Definicion del grafo

Formato:
- `dirigido: true|false` --- modo del grafo (siempre `false` en los mapas actuales)
- `nodos: nombre1,nombre2,...` --- lista de nodos
- `origen:destino1(peso1),destino2(peso2),...` --- aristas con peso
- Lineas con `#` son comentarios ignorados

Ejemplo (`maps/isla_tortuga/grafo.txt`):
```
dirigido: false
nodos: playa,pueblo,caleta,valle,laguna,tesoro,dunas,acantilado,mirador,puerto,manglar,ruinas,mina
```
Cada algoritmo encuentra una ruta distinta al tesoro en este mapa:
- BFS: `playa → dunas → acantilado → tesoro` (3 saltos, peso 45)
- DFS: `playa → pueblo → caleta → valle → laguna → tesoro` (5 saltos, peso 25)
- Dijkstra: `playa → puerto → manglar → ruinas → mina → tesoro` (5 saltos, peso 14)

### coords.txt --- Coordenadas virtuales de nodos (0-1000)

```
# Coordenadas virtuales 0-1000
# Formato: nombre=x y
playa=850 750
pueblo_pirata=780 680
...
```

Coordenadas normalizadas en rango 0-1000, independientes de la resolucion de ventana. El renderizador las mapea a pixeles del panel y las transforma con zoom/pan. Modificables en tiempo real via modo edicion y persistibles con `guardarCoordenadas()`.

### pistas.txt --- Pistas por ubicacion (formato 3 campos)

```
# RUTA DE PISTAS: playa -> pueblo -> caleta -> valle -> laguna -> tesoro
playa:El loro en la palmera repite "pueblo" sin cesar:pueblo
pueblo:La anciana del mercado seniala el camino a la caleta:caleta
...
```

**Formato:** `ubicacion:pista_texto:destino_siguiente`
- El `Diccionario` mapea `ubicacion -> pista_texto` (primeros 2 campos)
- El `Arbol` mapea `pista_texto -> destino_siguiente` (ultimos 2 campos), formando una cadena anidada que guia desde `playa` hasta `tesoro`

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
      core/juego.cpp core/mapa_manager.cpp \
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

| Entrada | Accion | Descripcion tecnica |
|---------|--------|---------------------|
| Click nodo | `seleccionarNodo()` | Hit test con distancia euclidiana en coordenadas mundo |
| Click der + arrastrar | `pan()` | Arrastra el mapa (offset en Renderizador), fondo sincronizado |
| Rueda mouse | Zoom / Scroll | Zoom sincronizado en grafo y fondo, scroll en arbol de pistas |
| Click en combo box | Abre dropdown de mapas | Muestra lista de mapas disponibles; al seleccionar uno se recarga todo |

### Botones en pantalla

| Boton | Funcion | Tecla |
|-------|---------|-------|
| BFS | Inicia BFS desde nodo seleccionado | B |
| DFS | Inicia DFS desde nodo seleccionado | D |
| DIJKSTRA | Inicia Dijkstra desde nodo seleccionado | R |
| PISTAS | Inicia navegacion por pistas | F |
| \|\| | Pausa/Reanuda (toggle AUTO/MANUAL) | P |
| >> | Paso manual (solo en MANUAL) | N |
| AUTO | Reinicia reproduccion automatica | — |
| - | Mas lento (+100ms) | - (tecla) |
| + | Mas rapido (-100ms) | + (tecla) |
| LIMPIAR | Limpia todo (estados, colores, arbol, seleccion) | C |
| GUARDAR | Guarda ruta en resultado.txt | S |
| EDITAR | Toggle modo edicion de nodos | E |
| G.CORDS | Guarda coordenadas actuales en coords.txt | K |
| RESET | Reset zoom/pan/scroll | V |
| SALIR | Cierra programa | Esc |

---

## Algoritmos implementados

| Algoritmo | Estructura usada | Complejidad | Descripcion |
|-----------|-----------------|-------------|-------------|
| **BFS** | Cola circular | O(V + E) | Recorrido por niveles. Usa `bfsPaso()` para animacion paso a paso. Encuentra la ruta con menor cantidad de aristas |
| **DFS** | Pila dinamica | O(V + E) | Recorrido en profundidad. Usa `dfsPaso()` para animacion paso a paso, con arreglo de expandidos para control de retroceso |
| **Dijkstra** | Arreglo (busqueda lineal del minimo) | O(V^2) | Camino mas corto por peso acumulado. Version paso a paso con `dijkstraPaso()`. En cada paso selecciona el nodo no visitado con menor distancia y relaja sus aristas |
| **Pistas** | Arbol n-ario + Diccionario | O(N) | Navegacion guiada por pistas. Cada paso: leer pista del nodo actual en el diccionario, interpretar en el arbol, saltar al siguiente nodo. Cantidad de pasos variable segun el mapa |

---

## Pantalla

```
+------------------------------------------+-----------------------------+
|                                          |  BITACORA DE EXPLORACION   |
|   Fondo de mapa                          |  - Nodo seleccionado       |
|   Mapa del grafo (N nodos)              |  - Pista actual            |
|   Pan + Zoom sincronizado               |  - Costo acumulado         |
|   Click selecciona / Editar arrastra     |  - Estado de exploracion   |
|                                          |  - Procesando: [nombre]    |
|   Colores nodo:                         |  - Ruta encontrada         |
|   Gris    = no visitado                 |                             |
|   Naranja = procesando ahora            |  [COLA (BFS)]  [PILA (DFS)]|
|   Verde   = visitado                    |  barras visibles en vivo   |
|   Rojo    = ruta final                  +-----------------------------+
|   Amarillo = en cola/pila (frontier)    |  ARBOL DE PISTAS           |
|   Dorado  = tesoro                      |  (pista activa resaltada)  |
|   Anillos blancos = seleccionado        |  "Cada pista lleva a la    |
|                                          |   siguiente ubicacion"     |
|   Aristas rojas = ruta final            |                             |
+------------------------------------------+-----------------------------+
| Mapa: isla_tortuga  >                                                       |
| [BFS][DFS][DIJKSTRA][PISTAS] [||][>>][AUTO] [-][+] [LIMPIAR][GUARDAR][EDITAR][G.CORDS][RESET][SALIR] |
| AUTO REPRODUCIENDO / MANUAL PAUSADO  -  Paso N/N  -  Vel NNNms              |
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
  |     |-- include/explorador.hpp
  |           |-- include/grafo.hpp
  |           |-- include/cola.hpp
  |           |-- include/pila.hpp
  |           |-- include/lista.hpp
  |
  |-- core/mapa_manager.hpp
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
  |     |-- include/grafo.hpp
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
