# El Tesoro del Pirata — Guía de Requisitos del Proyecto

## Descripción

Debemos ayudar a un pirata a encontrar un tesoro escondido en una isla. El mapa está representado como un **grafo** donde los nodos son ubicaciones (playa, montaña, cueva, etc.) y las aristas son caminos entre ellas con etiquetas que indican el costo. Cada ubicación tiene **pistas** (almacenadas en un archivo de texto) que guían al tesoro.

---

## Requisitos de Estructuras de Datos

### 1. Grafo — Representar el mapa y sus conexiones

El mapa de la isla se modela como un **grafo no dirigido o dirigido** donde:
- Cada **nodo** representa una ubicación (playa, montaña, cueva, etc.) con un nombre único.
- Cada **arista** conecta dos ubicaciones y tiene un **peso** (costo del camino).
- Se debe poder agregar nodos y aristas, buscar un nodo por nombre, y obtener los vecinos de un nodo.
- Forma de implementación: **lista de adyacencia** (cada nodo tiene una lista enlazada de aristas que conectan a otros nodos).
- Carga desde archivo: leer la lista de nodos y sus conexiones. Formato típico: `origen:destino1(peso),destino2(peso)`.

### 2. Árbol — Interpretar las pistas

Se usa un **árbol de decisiones** para modelar la secuencia de pistas:
- Cada nodo del árbol contiene una **pista** (texto) y el **nombre del destino** al que lleva.
- La **raíz** representa la primera pista. Cada **hijo** es la siguiente pista según la decisión tomada.
- Dada una pista (ej: "Busca donde el sol no llega"), el árbol debe devolver el nombre de la siguiente ubicación.
- Forma de implementación: **árbol n-ario** (un nodo puede tener múltiples hijos). Cada nodo tiene un arreglo dinámico de punteros a hijos y una referencia al padre.
- Recorrido típico: desde la raíz, comparar la pista actual con los hijos hasta encontrar la coincidencia, luego avanzar a ese hijo.

### 3. Cola / Pila — Gestionar el orden de exploración

Para explorar la isla se usan dos estrategias clásicas:
- **BFS (Breadth-First Search)**: usa una **cola** para explorar nodos por niveles. La cola debe ser **circular** (evita desperdicio de espacio) o dinámica. Opera en FIFO: los nodos se encolan cuando se descubren y se desencolan cuando se procesan.
- **DFS (Depth-First Search)**: usa una **pila** para explorar nodos en profundidad. Se implementa con un **arreglo dinámico** con tope. Opera en LIFO: se apila el nodo actual y se desapila al retroceder.
- Ambas deben proveer: `encolar(int)/push(int)`, `desencolar()/pop()`, `estaVacia()`, `obtenerTam()`.

### 4. Diccionario / Tabla Hash — Almacenar información de ubicaciones

Cada ubicación tiene asociada una **pista** y un **estado de exploración**. Se necesita una estructura clave→valor eficiente:
- **Clave**: nombre del nodo (cadena).
- **Valor**: pista asociada u otra información (costo acumulado, visitado, etc.).
- Forma de implementación: **tabla hash abierta** con un arreglo de listas enlazadas (encadenamiento separado). Función hash simple sobre el nombre.
- Operaciones: `insertar(clave, valor)`, `obtener(clave)`, `contiene(clave)`.

### 5. Lista — Guardar el camino óptimo al tesoro

Una vez encontrado el tesoro (por pistas o por camino más corto), se debe almacenar la **ruta final** como secuencia ordenada de nodos:
- Forma de implementación: **lista doblemente enlazada** con punteros a anterior y siguiente.
- Cada nodo contiene un entero (índice del nodo en el grafo) y punteros al anterior/siguiente.
- Operaciones: `insertar(dato)`, `eliminar(dato)`, `contiene(dato)`, `obtener(indice)`, `longitud()`, `vaciar()`.

### 6. Archivos — Leer pistas y guardar el resultado

El programa debe leer datos desde archivos de texto y escribir el resultado:
- **Lectura**: cargar el grafo desde `grafo.txt` y las pistas desde `pistas.txt` (o un solo archivo con formato `ubicacion:pista`).
- **Escritura**: guardar la ruta al tesoro en un archivo de salida (ej: `resultado.txt`) con formato legible que incluya: inicio, ruta completa, costo total, nodos visitados, algoritmo usado.
- Manejo básico de flujos: `ifstream` para lectura, `ofstream` para escritura. Control de errores (archivo no encontrado, formato incorrecto).

---

## Tareas del Programa

### 1. Cargar el grafo del mapa desde un archivo
Leer un archivo de texto que defina los nodos y sus conexiones. Construir internamente la estructura de grafo con lista de adyacencia.

### 2. Usar BFS / DFS para explorar la isla
Implementar ambos algoritmos de recorrido sobre el grafo. BFS descubre nodos por niveles desde un inicio. DFS explora rama por rama hasta el final. Ambos deben marcar el estado de cada nodo (no visitado, visitado, en ruta).

### 3. Interpretar las pistas
Cada nodo contiene una pista. Usando el árbol de decisiones, dada la pista del nodo actual, determinar cuál es el siguiente nodo al que dirigirse. Ejemplo: "Busca donde el sol no llega" → ir a "cueva".

### 4. Encontrar el camino más corto o la secuencia correcta de pistas
Se pueden combinar dos enfoques:
- **Por pistas**: seguir la secuencia del árbol de decisiones hasta llegar a "tesoro".
- **Por camino más corto**: una vez conocido el destino, usar **Dijkstra** (sobre el grafo ponderado) para encontrar la ruta de menor costo total desde el inicio.
- Puede integrarse: navegar por pistas hasta identificar el tesoro, luego calcular la ruta óptima desde el inicio hasta allí.

### 5. Guardar la ruta al tesoro en otro archivo
Escribir el resultado en un archivo de salida con formato claro: punto de inicio, recorrido paso a paso, costo acumulado, nodos totales visitados, y qué algoritmo se utilizó.
