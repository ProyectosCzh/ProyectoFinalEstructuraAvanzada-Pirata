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
    const int ANCHO = 1600;
    const int ALTO  = 900;

    Juego juego;
    printf("Cargando datos...\n");
    if (!juego.cargarDatos()) {
        printf("Error al cargar datos. Saliendo.\n");
        return 1;
    }

    float anchoGrafo   = ANCHO * 0.60f;
    float anchoPanel   = ANCHO - anchoGrafo;
    float altoSuperior = ALTO * 0.50f;
    float altoInferior = ALTO - altoSuperior;

    Renderizador renderizador(&juego.getGrafo(), anchoGrafo, ALTO);
    PanelInfo    panelInfo({ anchoGrafo, 0, anchoPanel, altoSuperior });
    UIArbol      uiArbol(&juego.getArbolPistas(), { anchoGrafo, altoSuperior, anchoPanel, altoInferior });
    Animador     animador;

    InitWindow(ANCHO, ALTO, "El Tesoro del Pirata - El Mapa del Capitan");
    SetTargetFPS(60);

    Vector2 lastMouse = { 0, 0 };
    bool arrastrandoMapa = false;

    while (!WindowShouldClose()) {
        Vector2 mouse = GetMousePosition();

        float deltaWheel = GetMouseWheelMove();
        if (deltaWheel != 0) {
            if (CheckCollisionPointRec(mouse, { 0, 0, anchoGrafo, ALTO })) {
                renderizador.setZoom(renderizador.getZoom() + deltaWheel * 0.1f);
            } else if (CheckCollisionPointRec(mouse, { anchoGrafo, altoSuperior, anchoPanel, altoInferior })) {
                uiArbol.actualizarScroll({ 0, -deltaWheel * 30.0f });
            }
        }

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            int nodo = renderizador.nodoBajoMouse(mouse);
            if (nodo >= 0) {
                juego.seleccionarNodo(nodo);
                renderizador.resetearColores();
                renderizador.setSeleccionado(nodo, true);
                animador.pausar();
            }
        }

        if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON)) {
            if (CheckCollisionPointRec(mouse, { 0, 0, anchoGrafo, ALTO })) {
                if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
                    lastMouse = mouse;
                    arrastrandoMapa = true;
                }
                if (arrastrandoMapa) {
                    Vector2 delta = { mouse.x - lastMouse.x, mouse.y - lastMouse.y };
                    renderizador.pan(delta);
                    lastMouse = mouse;
                }
            }
        } else {
            arrastrandoMapa = false;
        }

        if (IsKeyPressed(KEY_B)) {
            juego.iniciarBFS();
            animador.iniciar(juego.getGrafo().getNumNodos() * 2);
        }

        if (IsKeyPressed(KEY_D)) {
            juego.iniciarDFS();
            animador.iniciar(juego.getGrafo().getNumNodos() * 2);
        }

        if (IsKeyPressed(KEY_R)) {
            juego.iniciarDijkstra();
            animador.iniciar(juego.getGrafo().getNumNodos());
        }

        if (IsKeyPressed(KEY_F)) {
            juego.iniciarNavegacionPistas();
            animador.iniciar(juego.getGrafo().getNumNodos());
        }

        if (IsKeyPressed(KEY_N)) {
            juego.pasoAnimacion();
        }

        if (IsKeyPressed(KEY_P)) {
            if (animador.estaReproduciendo()) animador.pausar();
            else animador.reanudar();
        }

        if (IsKeyPressed(KEY_C)) {
            juego.limpiar();
            renderizador.resetearColores();
            animador.pausar();
        }

        if (IsKeyPressed(KEY_S)) {
            juego.guardarResultado();
        }

        if (IsKeyPressed(KEY_V)) {
            renderizador.resetearVista();
            uiArbol.resetearScroll();
        }

        if (IsKeyPressed(KEY_EQUAL) || IsKeyPressed(KEY_KP_ADD)) {
            animador.setVelocidadRelativa(-100);
        }
        if (IsKeyPressed(KEY_MINUS) || IsKeyPressed(KEY_KP_SUBTRACT)) {
            animador.setVelocidadRelativa(+100);
        }

        if (animador.actualizar()) {
            juego.pasoAnimacion();
        }

        renderizador.actualizarInput(mouse);
        renderizador.sincronizarConJuego(juego);

        const char* pistaActiva = juego.getPistaActiva();
        if (pistaActiva != nullptr && pistaActiva[0] != '\0') {
            uiArbol.setNodoActivo(pistaActiva);
        } else if (juego.getNodoProcesando() != nullptr) {
            const char* nom = juego.getGrafo().getNombreNodo(juego.getNodoSeleccionado());
            if (nom != nullptr) {
                const char* p = juego.getPistaNodo(juego.getNodoSeleccionado());
                if (p != nullptr) uiArbol.setNodoActivo(p);
            }
        }

        BeginDrawing();
        ClearBackground(COLOR_FONDO);

        for (int y = 0; y < ALTO; y++) {
            float t = (float)y / (float)ALTO;
            Color c = {
                (unsigned char)(COLOR_FONDO_SUPERIOR.r * (1-t) + COLOR_FONDO_INFERIOR.r * t),
                (unsigned char)(COLOR_FONDO_SUPERIOR.g * (1-t) + COLOR_FONDO_INFERIOR.g * t),
                (unsigned char)(COLOR_FONDO_SUPERIOR.b * (1-t) + COLOR_FONDO_INFERIOR.b * t),
                255
            };
            DrawLine(0, y, ANCHO, y, c);
        }

        DrawText("EL TESORO DEL PIRATA", ANCHO/2 - MeasureText("EL TESORO DEL PIRATA", 28)/2, 8, 28, COLOR_DORADO);
        DrawText("EL TESORO DEL PIRATA", ANCHO/2 - MeasureText("EL TESORO DEL PIRATA", 28)/2 + 2, 10, 28, COLOR_TEXTO_SOMBRA);

        renderizador.dibujar();
        renderizador.dibujarTooltip(mouse, juego);

        const char* nombreNodo = (juego.getNodoSeleccionado() >= 0)
            ? juego.getGrafo().getNombreNodo(juego.getNodoSeleccionado())
            : nullptr;

        const char* pista = (juego.getNodoSeleccionado() >= 0)
            ? juego.getPistaNodo(juego.getNodoSeleccionado())
            : nullptr;

        const char* estadoStr = "INICIO";
        switch (juego.getEstado()) {
            case INICIO:               estadoStr = "INICIO"; break;
            case EXPLORANDO_BFS:       estadoStr = "EXPLORANDO (BFS)"; break;
            case EXPLORANDO_DFS:       estadoStr = "EXPLORANDO (DFS)"; break;
            case EXPLORANDO_DIJKSTRA:  estadoStr = "EXPLORANDO (DIJKSTRA)"; break;
            case NAVEGANDO_PISTAS:     estadoStr = "NAVEGANDO POR PISTAS"; break;
            case RUTA_OPTIMA:          estadoStr = "RUTA OPTIMA"; break;
            case COMPLETADO:           estadoStr = "COMPLETADO"; break;
            case PAUSADO:              estadoStr = "PAUSADO"; break;
        }

        panelInfo.dibujar("BITACORA DE EXPLORACION", pista, nombreNodo,
                          juego.getCostoTotal(), estadoStr,
                          juego.getNumVisitados(), juego.getPasoActual(),
                          juego.getRutaOptima(),
                          juego.getColaAnimacion(),
                          juego.getPilaAnimacion(),
                          &juego.getGrafo(),
                          juego.getNodoProcesando());

        uiArbol.dibujar();

        animador.dibujarControles({ 5, ALTO - 90, 280, 80 });

        int instY = ALTO - 90;
        DrawRectangle(290, instY, 360, 80, COLOR_TOOLTIP_FONDO);
        DrawRectangleLinesEx({ 290, (float)instY, 360, 80 }, 1, COLOR_MARCO);
        DrawText("CONTROLES", 300, instY + 4, 13, COLOR_DORADO);
        DrawText("B/D/R: BFS/DFS/Dijkstra   F: Pistas", 300, instY + 22, 12, COLOR_TEXTO);
        DrawText("N: siguiente paso   P: pausa/reanudar", 300, instY + 38, 12, COLOR_TEXTO);
        DrawText("+/-: velocidad   C: limpiar   S: guardar", 300, instY + 54, 12, COLOR_TEXTO);
        DrawText("V: reset vista   Click der: pan   ESC: salir", 300, instY + 70, 11, COLOR_TEXTO_OSCURO);

        float zoom = renderizador.getZoom();
        char zoomStr[32];
        std::sprintf(zoomStr, "Zoom: %.0f%%", zoom * 100.0f);
        DrawText(zoomStr, ANCHO - 130, 12, 14, COLOR_TEXTO);

        EndDrawing();
    }

    CloseWindow();
    printf("Programa finalizado.\n");
    return 0;
}
