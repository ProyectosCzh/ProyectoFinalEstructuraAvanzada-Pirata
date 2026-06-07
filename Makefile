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
