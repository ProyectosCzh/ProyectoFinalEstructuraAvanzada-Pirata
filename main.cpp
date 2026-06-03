#include "raylib.h"
#include "core/juego.hpp"
#include "graphics/renderizador.hpp"
#include "graphics/panel_info.hpp"
#include "graphics/animador.hpp"
#include "graphics/ui_arbol.hpp"
#include "graphics/colores.hpp"
#include <cstdio>
#include <cstring>
#include <cmath>

struct Boton {
    Rectangle area;
    const char* etiqueta;
    Color colorBase;
    bool habilitado;
    bool hover;
    bool presionado;
};

static void inicializarBoton(Boton& b, float x, float y, float w, float h, const char* txt) {
    b.area = { x, y, w, h };
    b.etiqueta = txt;
    b.colorBase = COLOR_BOTON;
    b.habilitado = true;
    b.hover = false;
    b.presionado = false;
}

static void dibujarBoton(Boton& b, Vector2 mouse, bool mouseDown) {
    if (!b.habilitado) {
        DrawRectangleRec(b.area, { 40, 40, 50, 255 });
        DrawRectangleLinesEx(b.area, 1, COLOR_MARCO);
        int tw = MeasureText(b.etiqueta, 14);
        DrawText(b.etiqueta, (int)(b.area.x + b.area.width/2 - tw/2),
                 (int)(b.area.y + b.area.height/2 - 7), 14, COLOR_TEXTO_OSCURO);
        return;
    }
    b.hover = CheckCollisionPointRec(mouse, b.area);
    Color fondo = b.hover ? COLOR_BOTON_HOVER : b.colorBase;
    if (b.presionado) fondo = COLOR_DORADO;
    DrawRectangleRec(b.area, fondo);
    DrawRectangleLinesEx(b.area, 2, b.hover ? COLOR_DORADO : COLOR_MARCO);
    int tw = MeasureText(b.etiqueta, 14);
    Color txtCol = b.presionado ? BLACK : COLOR_TEXTO;
    DrawText(b.etiqueta, (int)(b.area.x + b.area.width/2 - tw/2),
             (int)(b.area.y + b.area.height/2 - 7), 14, txtCol);
}

static bool clickEnBoton(Boton& b, Vector2 mouse) {
    return b.habilitado && CheckCollisionPointRec(mouse, b.area);
}

int main() {
    const int ANCHO = 1600;
    const int ALTO  = 900;

    Juego juego;
    printf("Cargando datos...\n");
    if (!juego.cargarDatos()) {
        printf("Error al cargar datos. Saliendo.\n");
        return 1;
    }

    float altoBarraBotones  = 60.0f;
    float altoBarraAnimador = 40.0f;
    float yBarraBotones  = ALTO - altoBarraBotones - altoBarraAnimador - 6;
    float yBarraAnimador = ALTO - altoBarraAnimador;

    float anchoGrafo   = ANCHO * 0.60f;
    float anchoPanel   = ANCHO - anchoGrafo;
    float altoPanelSup = yBarraBotones * 0.50f;
    float altoPanelInf = yBarraBotones - altoPanelSup;

    Renderizador renderizador(&juego.getGrafo(), anchoGrafo, ALTO);
    PanelInfo    panelInfo({ anchoGrafo, 0, anchoPanel, altoPanelSup });
    UIArbol      uiArbol(&juego.getArbolPistas(), { anchoGrafo, altoPanelSup, anchoPanel, altoPanelInf });
    Animador     animador;

    const int NUM_BOTONES = 13;
    Boton botones[NUM_BOTONES];
    float bw = 105.0f, bh = altoBarraBotones - 14;
    float by = yBarraBotones + 7;
    float bx = 14;
    float sep = 8;

    inicializarBoton(botones[0],  bx, by, bw, bh, "BFS");          bx += bw + sep;
    inicializarBoton(botones[1],  bx, by, bw, bh, "DFS");          bx += bw + sep;
    inicializarBoton(botones[2],  bx, by, bw, bh, "DIJKSTRA");     bx += bw + sep;
    inicializarBoton(botones[3],  bx, by, bw, bh, "PISTAS");       bx += bw + sep;
    inicializarBoton(botones[4],  bx, by, bw*0.6f, bh, "||");       bx += bw*0.6f + sep;
    inicializarBoton(botones[5],  bx, by, bw*0.6f, bh, ">>");       bx += bw*0.6f + sep;
    inicializarBoton(botones[6],  bx, by, bw*0.6f, bh, "AUTO");     bx += bw*0.6f + sep;
    inicializarBoton(botones[7],  bx, by, bw*0.5f, bh, "-");        bx += bw*0.5f + sep;
    inicializarBoton(botones[8],  bx, by, bw*0.5f, bh, "+");        bx += bw*0.5f + sep;
    inicializarBoton(botones[9],  bx, by, bw, bh, "LIMPIAR");      bx += bw + sep;
    inicializarBoton(botones[10], bx, by, bw, bh, "GUARDAR");      bx += bw + sep;
    inicializarBoton(botones[11], bx, by, bw*0.7f, bh, "RESET");    bx += bw*0.7f + sep;
    inicializarBoton(botones[12], bx, by, bw, bh, "SALIR");

    InitWindow(ANCHO, ALTO, "El Tesoro del Pirata - El Mapa del Capitan");
    SetTargetFPS(60);

    Vector2 lastMouse = { 0, 0 };
    bool arrastrandoMapa = false;
    bool necesitaNodoSeleccion = true;

    while (!WindowShouldClose()) {
        Vector2 mouse = GetMousePosition();

        bool sobreGrafo = CheckCollisionPointRec(mouse, { 0, 0, anchoGrafo, yBarraBotones });
        bool sobreArbol = CheckCollisionPointRec(mouse, { anchoGrafo, altoPanelSup, anchoPanel, altoPanelInf });
        bool sobreBotones = CheckCollisionPointRec(mouse, { 0, yBarraBotones, ANCHO, altoBarraBotones + altoBarraAnimador });

        float deltaWheel = GetMouseWheelMove();
        if (deltaWheel != 0) {
            if (sobreGrafo) {
                renderizador.setZoom(renderizador.getZoom() + deltaWheel * 0.1f);
            } else if (sobreArbol) {
                uiArbol.actualizarScroll({ 0, -deltaWheel * 30.0f });
            }
        }

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (sobreGrafo) {
                int nodo = renderizador.nodoBajoMouse(mouse);
                if (nodo >= 0) {
                    juego.seleccionarNodo(nodo);
                    renderizador.resetearColores();
                    renderizador.setSeleccionado(nodo, true);
                    necesitaNodoSeleccion = false;
                }
            } else if (sobreArbol) {
                for (int i = 0; i < NUM_BOTONES; i++) {
                    if (clickEnBoton(botones[i], mouse)) {
                        botones[i].presionado = true;
                        break;
                    }
                }
            } else {
                for (int i = 0; i < NUM_BOTONES; i++) {
                    if (clickEnBoton(botones[i], mouse)) {
                        botones[i].presionado = true;
                        break;
                    }
                }
            }
        }

        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
            for (int i = 0; i < NUM_BOTONES; i++) {
                if (botones[i].presionado) {
                    botones[i].presionado = false;
                    if (clickEnBoton(botones[i], mouse)) {
                        switch (i) {
                            case 0: {
                                if (juego.getNodoSeleccionado() < 0) break;
                                juego.iniciarBFS();
                                animador.iniciar(juego.getGrafo().getNumNodos() * 3);
                            } break;
                            case 1: {
                                if (juego.getNodoSeleccionado() < 0) break;
                                juego.iniciarDFS();
                                animador.iniciar(juego.getGrafo().getNumNodos() * 3);
                            } break;
                            case 2: {
                                if (juego.getNodoSeleccionado() < 0) break;
                                juego.iniciarDijkstra();
                                animador.iniciar(juego.getGrafo().getNumNodos() * 2);
                            } break;
                            case 3: {
                                if (juego.getNodoSeleccionado() < 0) break;
                                juego.iniciarNavegacionPistas();
                                animador.iniciar(juego.getGrafo().getNumNodos());
                            } break;
                            case 4:
                                if (animador.estaReproduciendo()) animador.pausar();
                                else animador.reanudar();
                                break;
                            case 5:
                                if (!animador.estaReproduciendo()) juego.pasoAnimacion();
                                break;
                            case 6:
                                animador.iniciar(juego.getGrafo().getNumNodos() * 3);
                                break;
                            case 7:
                                animador.setVelocidadRelativa(+100);
                                break;
                            case 8:
                                animador.setVelocidadRelativa(-100);
                                break;
                            case 9:
                                juego.limpiar();
                                renderizador.resetearColores();
                                uiArbol.limpiarNodoActivo();
                                uiArbol.resetearScroll();
                                animador.pausar();
                                necesitaNodoSeleccion = true;
                                break;
                            case 10:
                                juego.guardarResultado();
                                break;
                            case 11:
                                renderizador.resetearVista();
                                uiArbol.resetearScroll();
                                break;
                            case 12:
                                CloseWindow();
                                return 0;
                        }
                    }
                }
            }
        }

        if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON)) {
            if (sobreGrafo) {
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
            if (juego.getNodoSeleccionado() >= 0) {
                juego.iniciarBFS();
                animador.iniciar(juego.getGrafo().getNumNodos() * 3);
            }
        }
        if (IsKeyPressed(KEY_D)) {
            if (juego.getNodoSeleccionado() >= 0) {
                juego.iniciarDFS();
                animador.iniciar(juego.getGrafo().getNumNodos() * 3);
            }
        }
        if (IsKeyPressed(KEY_R)) {
            if (juego.getNodoSeleccionado() >= 0) {
                juego.iniciarDijkstra();
                animador.iniciar(juego.getGrafo().getNumNodos() * 2);
            }
        }
        if (IsKeyPressed(KEY_F)) {
            if (juego.getNodoSeleccionado() >= 0) {
                juego.iniciarNavegacionPistas();
                animador.iniciar(juego.getGrafo().getNumNodos());
            }
        }
        if (IsKeyPressed(KEY_N) && !animador.estaReproduciendo()) juego.pasoAnimacion();
        if (IsKeyPressed(KEY_P)) {
            if (animador.estaReproduciendo()) animador.pausar();
            else animador.reanudar();
        }
        if (IsKeyPressed(KEY_C)) {
            juego.limpiar();
            renderizador.resetearColores();
            uiArbol.limpiarNodoActivo();
            uiArbol.resetearScroll();
            animador.pausar();
            necesitaNodoSeleccion = true;
        }
        if (IsKeyPressed(KEY_S)) juego.guardarResultado();
        if (IsKeyPressed(KEY_V)) {
            renderizador.resetearVista();
            uiArbol.resetearScroll();
        }
        if (IsKeyPressed(KEY_EQUAL) || IsKeyPressed(KEY_KP_ADD)) animador.setVelocidadRelativa(-100);
        if (IsKeyPressed(KEY_MINUS) || IsKeyPressed(KEY_KP_SUBTRACT)) animador.setVelocidadRelativa(+100);

        if (animador.actualizar()) {
            juego.pasoAnimacion();
        }

        bool hayAlgoritmoCorriendo =
            (juego.getEstado() == EXPLORANDO_BFS ||
             juego.getEstado() == EXPLORANDO_DFS ||
             juego.getEstado() == EXPLORANDO_DIJKSTRA ||
             juego.getEstado() == NAVEGANDO_PISTAS);

        for (int i = 0; i < NUM_BOTONES; i++) {
            botones[i].habilitado = true;
        }
        if (necesitaNodoSeleccion) {
            for (int i = 0; i <= 3; i++) botones[i].habilitado = false;
        }
        if (!hayAlgoritmoCorriendo) {
            botones[4].habilitado = false;
            botones[5].habilitado = false;
            botones[6].habilitado = false;
        }

        renderizador.actualizarInput(mouse);
        renderizador.sincronizarConJuego(juego);

        const char* pistaActiva = juego.getPistaActiva();
        if (pistaActiva != nullptr && pistaActiva[0] != '\0') {
            uiArbol.setNodoActivo(pistaActiva);
        } else if (juego.getNodoSeleccionado() >= 0) {
            const char* p = juego.getPistaNodo(juego.getNodoSeleccionado());
            if (p != nullptr) uiArbol.setNodoActivo(p);
        }

        BeginDrawing();
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

        const char* titulo = "EL TESORO DEL PIRATA";
        int tw = MeasureText(titulo, 28);
        DrawText(titulo, ANCHO/2 - tw/2 + 2, 10, 28, COLOR_TEXTO_SOMBRA);
        DrawText(titulo, ANCHO/2 - tw/2, 8, 28, COLOR_DORADO);

        renderizador.dibujarConEstado(juego);
        renderizador.dibujarTooltip(mouse, juego);

        const char* nombreNodo = (juego.getNodoSeleccionado() >= 0)
            ? juego.getGrafo().getNombreNodo(juego.getNodoSeleccionado())
            : nullptr;
        const char* pista = (juego.getNodoSeleccionado() >= 0)
            ? juego.getPistaNodo(juego.getNodoSeleccionado())
            : nullptr;

        const char* estadoStr = "INICIO";
        switch (juego.getEstado()) {
            case INICIO:               estadoStr = "INICIO - Selecciona un nodo"; break;
            case EXPLORANDO_BFS:       estadoStr = "EXPLORANDO (BFS)"; break;
            case EXPLORANDO_DFS:       estadoStr = "EXPLORANDO (DFS)"; break;
            case EXPLORANDO_DIJKSTRA:  estadoStr = "EXPLORANDO (DIJKSTRA)"; break;
            case NAVEGANDO_PISTAS:     estadoStr = "NAVEGANDO POR PISTAS"; break;
            case COMPLETADO:           estadoStr = "COMPLETADO (Ruta optima encontrada)"; break;
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

        DrawRectangle(0, (int)yBarraBotones, ANCHO, (int)altoBarraBotones, COLOR_PANEL_FONDO);
        DrawLine(0, (int)yBarraBotones, ANCHO, (int)yBarraBotones, COLOR_MARCO);
        for (int i = 0; i < NUM_BOTONES; i++) {
            dibujarBoton(botones[i], mouse, IsMouseButtonDown(MOUSE_LEFT_BUTTON));
        }

        animador.dibujarControles({ 0, yBarraAnimador, (float)ANCHO, altoBarraAnimador });

        float zoom = renderizador.getZoom();
        char zoomStr[32];
        std::sprintf(zoomStr, "Zoom: %.0f%%", zoom * 100.0f);
        DrawText(zoomStr, ANCHO - 130, 12, 14, COLOR_TEXTO);

        if (necesitaNodoSeleccion) {
            const char* msj = ">> Haz CLICK en un nodo del mapa para empezar <<";
            int mw = MeasureText(msj, 18);
            DrawText(msj, ANCHO/2 - mw/2, 46, 18, COLOR_DORADO);
        }

        EndDrawing();
    }

    CloseWindow();
    printf("Programa finalizado.\n");
    return 0;
}
