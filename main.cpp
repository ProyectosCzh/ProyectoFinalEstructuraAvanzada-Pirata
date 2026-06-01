#include "raylib.h"
#include "core/juego.hpp"
#include "graphics/renderizador.hpp"
#include "graphics/panel_info.hpp"
#include "graphics/animador.hpp"
#include "graphics/ui_arbol.hpp"
#include "graphics/colores.hpp"
#include <cstdio>
#include <cstring>

int main() {
    const int ANCHO = 1280;
    const int ALTO = 720;

    Juego juego;
    printf("Cargando datos...\n");
    if (!juego.cargarDatos()) {
        printf("Error al cargar datos. Saliendo.\n");
        return 1;
    }

    float anchoGrafo = ANCHO * 0.55f;
    float anchoPanel = ANCHO - anchoGrafo;
    float altoSuperior = ALTO * 0.55f;
    float altoInferior = ALTO - altoSuperior;

    Renderizador renderizador(&juego.getGrafo(), anchoGrafo, ALTO);
    PanelInfo panelInfo({ anchoGrafo, 0, anchoPanel, altoSuperior });
    UIArbol uiArbol(&juego.getArbolPistas(), { anchoGrafo, altoSuperior, anchoPanel, altoInferior });

    bool bfsIniciado = false;
    bool dfsIniciado = false;

    InitWindow(ANCHO, ALTO, "El Tesoro del Pirata");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        Vector2 mouse = GetMousePosition();

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            int nodo = renderizador.nodoBajoMouse(mouse);
            if (nodo >= 0) {
                juego.seleccionarNodo(nodo);
                renderizador.resetearColores();
                renderizador.setSeleccionado(nodo, true);
                bfsIniciado = false;
                dfsIniciado = false;
            }
        }

        if (IsKeyPressed(KEY_B)) {
            juego.iniciarBFS();
            bfsIniciado = true;
            dfsIniciado = false;
            renderizador.sincronizarConJuego(juego);
        }

        if (IsKeyPressed(KEY_D)) {
            juego.iniciarDFS();
            dfsIniciado = true;
            bfsIniciado = false;
            renderizador.sincronizarConJuego(juego);
        }

        if (IsKeyPressed(KEY_R)) {
            juego.iniciarDijkstra();
            renderizador.sincronizarConJuego(juego);
        }

        if (IsKeyPressed(KEY_SPACE)) {
            juego.pasoAnimacion();
            if (juego.getEstado() == COMPLETADO && bfsIniciado) {
                printf("BFS completo! Ruta encontrada.\n");
            }
            if (juego.getEstado() == COMPLETADO && dfsIniciado) {
                printf("DFS completo! Ruta encontrada.\n");
            }
            renderizador.sincronizarConJuego(juego);
        }

        if (IsKeyPressed(KEY_C)) {
            juego.limpiar();
            renderizador.resetearColores();
            bfsIniciado = false;
            dfsIniciado = false;
        }

        if (IsKeyPressed(KEY_S)) {
            juego.guardarResultado();
        }

        BeginDrawing();
        ClearBackground(COLOR_FONDO);

        renderizador.sincronizarConJuego(juego);
        renderizador.dibujar();

        const char* nombreNodo = (juego.getNodoSeleccionado() >= 0)
            ? juego.getGrafo().getNombreNodo(juego.getNodoSeleccionado())
            : nullptr;

        const char* pista = (juego.getNodoSeleccionado() >= 0)
            ? juego.getPistaNodo(juego.getNodoSeleccionado())
            : nullptr;

        const char* estadoStr = "INICIO";
        switch (juego.getEstado()) {
            case INICIO:         estadoStr = "INICIO"; break;
            case EXPLORANDO_BFS: estadoStr = "EXPLORANDO (BFS)"; break;
            case EXPLORANDO_DFS: estadoStr = "EXPLORANDO (DFS)"; break;
            case RUTA_OPTIMA:    estadoStr = "RUTA OPTIMA"; break;
            case COMPLETADO:     estadoStr = "COMPLETADO"; break;
            case PAUSADO:        estadoStr = "PAUSADO"; break;
        }

        panelInfo.dibujar("INFORMACION", pista, nombreNodo,
                          juego.getCostoTotal(), estadoStr,
                          juego.getNumVisitados(), juego.getPasoActual(),
                          juego.getRutaOptima(),
                          juego.getColaAnimacion(),
                          juego.getPilaAnimacion());

        uiArbol.dibujar();

        // Instrucciones en pantalla
        int instY = ALTO - 75;
        DrawRectangle(5, instY, 350, 70, { 0, 0, 0, 180 });
        DrawText("Click: seleccionar nodo", 10, instY + 2, 12, COLOR_TEXTO);
        DrawText("B: BFS | D: DFS | R: Dijkstra", 10, instY + 18, 12, COLOR_TEXTO);
        DrawText("ESPACIO: paso a paso | C: limpiar", 10, instY + 34, 12, COLOR_TEXTO);
        DrawText("S: guardar resultado | ESC: salir", 10, instY + 50, 12, COLOR_TEXTO);

        EndDrawing();
    }

    CloseWindow();
    printf("Programa finalizado.\n");
    return 0;
}
