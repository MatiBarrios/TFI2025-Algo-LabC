#include <stdio.h>
#include <stdlib.h>
#include "mapa.h"
#include "rutas.h"
#include "jugadores.h"
#include "io.h"
#include "tipos.h"

int main() {
    printf("|-------------------------------------|\n");
    printf("|    SISTEMA DE NAVEGACION ESTELAR    |\n");
    printf("|-------------------------------------|\n");

    int opcionMenu;
    do {
        printf("\n----------------- MENU -----------------\n");
        printf("1. Ver resultados de las mejores jugadas\n");
        printf("2. Comenzar partida\n");
        printf("0. Salir\n"); //se puede salir con ctrl+C, pero dio error en la prueba anterior
        printf("Seleccione una opcion: ");
        
        if (scanf("%d", &opcionMenu) != 1) {
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            printf("X Opcion invalida\n");
            continue;
        }

        if (opcionMenu == 1) {
            mostrar_mejores_archivo();
        } 
        else if (opcionMenu == 2) {
            // Selección de mapa
            printf("\n----- SELECCIONAR MAPA -----\n");
            printf("1. Cargar mapa desde archivo\n");
            printf("2. Generar mapa aleatorio\n");
            printf("Seleccione una opcion: ");
            
            int opcionMapa;
            if (scanf("%d", &opcionMapa) != 1) {
                int c;
                while ((c = getchar()) != '\n' && c != EOF);
                printf("X Opcion invalida\n");
                continue;
            }

            Mapa mapa;
            Mapa* pMapa = NULL;
            CodigoError errorCarga = OK;

            if (opcionMapa == 1) {
                //COLOCAR AQUI EL MAPA POR DEFECTO
                // Sugerir por defecto: data/ejemplos_entrada.txt
                char ruta[256];
                io_leer_ruta_archivo(ruta, sizeof(ruta), "datos/ejemplos_entrada.txt");

                errorCarga = cargarMapa(ruta, &mapa);
                if (errorCarga == OK) {
                    pMapa = &mapa;
                } else {
                    mostrar_error(errorCarga);
                    continue;
                }
            } 
            else if (opcionMapa == 2) {
                int f, c;
                printf("Ingrese la cantidad de filas y columnas que desea (ej: 10 10): ");
                if (scanf("%d %d", &f, &c) != 2 || f <= 0 || c <= 0 || 
                    f > MAX_FILAS || c > MAX_COLUMNAS) {
                    printf("X Dimensiones invalidas\n");
                    continue;
                }
                
                pMapa = generarMapaAleatorio(f, c);
                if (!pMapa) {
                    mostrar_error(ERROR_MEMORIA);
                    continue;
                }

                // Guardar (sobrescribir/crear) el último mapa aleatorio
                CodigoError ge = guardar_mapa_txt(pMapa, "datos/ultimo_random.txt");
                if (ge == OK) {
                    printf("Mapa aleatorio guardado en: datos/ultimo_random.txt\n");
                } else {
                    printf("No se pudo guardar el mapa aleatorio (datos/ultimo_random.txt)\n");
                }
            } 
            else {
                printf("X Opcion invalida\n");
                continue;
            }

            // Mostrar mapa
            mostrarMapa(pMapa);

            // Preguntar si se desea elegir una salida destino concreta
            int distancias[MAX_FILAS][MAX_COLUMNAS];
            int elegirSalida = 0;
            printf("Desea elegir una salida en especifico? (1=si, 0=no): ");
            if (scanf("%d", &elegirSalida) != 1) elegirSalida = 0;
            int ch; while ((ch = getchar()) != '\n' && ch != EOF) {}

            // Si elige una salida específica, listarlas y pedir selección
            if (elegirSalida) {
                int ex[MAX_FILAS * MAX_COLUMNAS];
                int ey[MAX_FILAS * MAX_COLUMNAS];
                int nEx = 0;
                for (int i = 0; i < pMapa->filas; i++) {
                    for (int j = 0; j < pMapa->columnas; j++) {
                        if (pMapa->datos[i][j] == SALIDA) {
                            ex[nEx] = i; ey[nEx] = j;
                            printf("%d) Salida en (%d,%d)\n", nEx + 1, i, j);
                            nEx++;
                        }
                    }
                }
                if (nEx == 0) {
                    printf("No hay salidas en el mapa.\n");
                    calcularDistancias(pMapa, distancias); // fallback
                } else {
                    int idx = 0;
                    printf("Seleccione numero de salida (1-%d): ", nEx);
                    if (scanf("%d", &idx) != 1 || idx < 1 || idx > nEx) {
                        printf("Seleccion invalida, usando todas las salidas.\n");
                        calcularDistancias(pMapa, distancias);
                    } else {
                        int sx = ex[idx - 1], sy = ey[idx - 1];
                        calcularDistanciasDesde(pMapa, sx, sy, distancias);
                        printf("Calculo de distancias hecho desde la salida (%d,%d)\n", sx, sy);
                    }
                }
            } else {
                calcularDistancias(pMapa, distancias);
            }
            // Solicitar cantidad de jugadores
            int numJugadores;
            printf("\nIngrese la cantidad de jugadores (1-%d): ", MAX_JUGADORES);
            if (scanf("%d", &numJugadores) != 1 || numJugadores < 1 || numJugadores > MAX_JUGADORES) {
                printf("X Cantidad de jugadores invalida\n");
                if (opcionMapa == 2) liberarMapa(pMapa);
                continue;
            }

            // Inicializar jugadores
            Jugador jugadores[MAX_JUGADORES];
            inicializarJugadores(jugadores, numJugadores);

            // Leer posiciones de cada jugador
            int todosValidos = 1;
            for (int i = 0; i < numJugadores; i++) {
                int x, y;
                CodigoError error = leer_posicion_jugador(pMapa, &x, &y, i + 1);
                
                if (error != OK) {
                    mostrar_error(error);
                    todosValidos = 0;
                    break;
                }
                
                error = asignarPosicionJugador(&jugadores[i], x, y, pMapa);
                if (error != OK) {
                    mostrar_error(error);
                    todosValidos = 0;
                    break;
                }
            }

            if (!todosValidos) {
                liberarJugadores(jugadores, numJugadores);
                if (opcionMapa == 2) liberarMapa(pMapa);
                continue;
            }

            // Calcular rutas
            calcularRutasJugadores(jugadores, numJugadores, pMapa, distancias);

            // Mostrar resultados
            printf("\n|--------------------------------------|\n");
            printf("|              RESULTADOS              |\n");
            printf("|--------------------------------------|\n");

            for (int i = 0; i < numJugadores; i++) {
                mostrar_resultado_jugador(&jugadores[i], pMapa);
            }

            // Determinar ganador(es)
            int winners[MAX_JUGADORES];
            int nw = determinar_ganadores(jugadores, numJugadores, winners, MAX_JUGADORES);

            if (nw == 0) {
                printf("\nX  Nadie gano\n");
            } else if (nw == 1) {
                Jugador* ganador = &jugadores[winners[0]];
                printf("\n:D GANADOR: Jugador %d con distancia %d y pasos %d\n",
                    ganador->id, ganador->distanciaTotal, ganador->longitudRuta);
                guardar_mejor_jugador(ganador);
            } else {
                // empate múltiple: todos los de 'winners' son ganadores
                int idx0 = winners[0];
                printf("\nEMPATE entre %d jugadores! distancia=%d pasos=%d\n",
                    nw, jugadores[idx0].distanciaTotal, jugadores[idx0].longitudRuta);


                printf("Ganadores: \n");
                for (int k = 0; k < nw; k++) {
                    int idx = winners[k];
                    printf("%d%s", jugadores[idx].id, (k + 1 < nw) ? ", " : "\n");
                    guardar_mejor_jugador(&jugadores[idx]);
                }
            }

            // Liberar memoria
            liberarJugadores(jugadores, numJugadores);
            if (opcionMapa == 2) liberarMapa(pMapa);
        }

    } while (opcionMenu != 0);

    printf("\nENIE\n");
    return 0;
}
