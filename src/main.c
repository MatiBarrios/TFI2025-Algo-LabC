#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "tipos.h"
#include "mapa.h"
#include "rutas.h"
#include "jugadores.h"
#include "io.h"

int main() {

    int c;
    // Semilla global para mapas aleatorios
    srand((unsigned) time(NULL));

    printf("Menu inicial:\n");
    printf("1) Ver archivo con mejores posiciones\n");
    printf("2) Jugar (nuevas rondas)\n");
    printf("Seleccione una opcion (1-2): ");

    int opcion = 0;
    if (scanf("%d", &opcion) != 1) {
        mostrar_error("Entrada invalida");
        return 1;
    }

    if (opcion == 1) {
        mostrar_mejores_archivo();
        while ((c = getchar()) != '\n' && c != EOF) {}
        
        printf("Programa finalizado, presione Enter para continuar");
        getchar();
        return 0;
    } else if (opcion != 2) {
        mostrar_error("Opcion invalida");
        return 1;
    }

    int num_rondas = 1;
    printf("Ingrese cantidad de rondas a jugar: ");
    if (scanf("%d", &num_rondas) != 1 || num_rondas < 1) {
        mostrar_error("Cantidad de rondas invalida");
        return 1;
    }

    // limpiar buffer ESTO VA A SALIR BANDA DE VECES EN EL MAIN PERO ES PORQUE UN CANCER TIENE EL SCANF
    while ((c = getchar()) != '\n' && c != EOF) {}

    for (int ronda = 1; ronda <= num_rondas; ronda++) {
        printf("\n=== Ronda %d ===\n", ronda);

        // elegir mapa: aleatorio 10x10 o leer archivo
        printf("Usar mapa aleatorio 10x10? (1=si, 0=no): ");
        int usar_aleatorio = 0;
        if (scanf("%d", &usar_aleatorio) != 1) usar_aleatorio = 0;

        // limpiar buffer
        while ((c = getchar()) != '\n' && c != EOF) {}

        Mapa* mapa = NULL;
        if (usar_aleatorio) {
            mapa = generarMapaAleatorio(10, 10);
            if (!mapa) {
                mostrar_error("No se pudo generar mapa aleatorio");
                return 1;
            }
        } else {
            // preguntar si usar archivo por defecto o pedir ruta
            printf("Usar mapa por defecto '%s'? (1=si,0=no): ", "datos/ejemplos_entrada.txt");
            int usar_def = 1;
            if (scanf("%d", &usar_def) != 1) usar_def = 1;
            while ((c = getchar()) != '\n' && c != EOF) {}

            if (usar_def) {
                mapa = cargarMapa("datos/ejemplos_entrada.txt");
            } else {
                char ruta[256];
                printf("Ingrese ruta al archivo de mapa: ");
                if (fgets(ruta, sizeof(ruta), stdin)) {
                    // quitar newline
                    ruta[strcspn(ruta, "\r\n")] = '\0';
                    mapa = cargarMapa(ruta);
                } else {
                    mostrar_error("Entrada invalida de ruta");
                }
            }

            if (!mapa) {
                mostrar_error("No se pudo cargar el mapa solicitado");
                return 1;
            }
        }

        // mostrar mapa
        mostrarMapa(mapa);

        // calcular distancias
        int distancias[MAX_FILAS][MAX_COLUMNAS];
        calcularDistancias(mapa, distancias);

        // pedir jugadores
        int num_jugadores = 0;
        printf("\nIngrese la cantidad de jugadores (1 - %d): ", MAX_JUGADORES);
        if (scanf("%d", &num_jugadores) != 1 || num_jugadores < 1 || num_jugadores > MAX_JUGADORES) {
            mostrar_error("Cantidad de jugadores inválida");
            // limpiar y continuar a la siguiente ronda
            free(mapa);
            continue;
        }

        // limpiar buffer
        while ((c = getchar()) != '\n' && c != EOF) {}

        Jugador* jugadores = malloc(sizeof(Jugador) * num_jugadores);
        if (!jugadores) {
            mostrar_error("No se pudo reservar memoria para jugadores");
            free(mapa);
            return 1;
        }

        inicializarJugadores(jugadores, num_jugadores);

        // Leer posiciones
        for (int i = 0; i < num_jugadores; i++) {
            int x = -1, y = -1;
            printf("\nJugador %d:\n", i + 1);
            leer_posicion_jugador(&x, &y);

            if (!esValida(mapa, x, y)) {
                mostrar_error("Posición inválida; se pedira nuevamente");
                i--;
                continue;
            }

            asignarPosicionJugador(&jugadores[i], x, y);
        }

        // calcular rutas y encontrar ganador
        calcularRutasJugadores(jugadores, num_jugadores, mapa, distancias);
        Jugador* ganador = encontrarGanador(jugadores, num_jugadores);

        if (ganador && ganador->distanciaTotal != INF) {
            printf("\n¡El jugador %d es el ganador de la ronda %d!\n", ganador->id, ronda);
            printf("Distancia total: %d\n", ganador->distanciaTotal);
            printf("Ruta: ");
            for (int i = 0; i < ganador->longitudRuta; i++) {
                printf("(%d,%d)", ganador->ruta[i].x, ganador->ruta[i].y);
                if (i < ganador->longitudRuta - 1) printf(" -> ");
            }
            printf("\n");

            // guardar mejor jugador de la ronda
            guardar_mejor_jugador(ganador, ronda);
        } else {
            printf("\nNingún jugador encontró una ruta válida en esta ronda.\n");
        }

        // liberar memoria de jugadores y mapa
        for (int i = 0; i < num_jugadores; i++) {
            if (jugadores[i].ruta != NULL) free(jugadores[i].ruta);
        }
        free(jugadores);
        free(mapa);

        // opcional: pausar entre rondas
        printf("\nRonda %d finalizada.\n", ronda);
    } // fin rondas

    // esperar antes de salir
    printf("\nTodas las rondas finalizadas.\n");

    while ((c = getchar()) != '\n' && c != EOF) {}

    printf("Programa finalizado, presione Enter para continuar");
    getchar();
    
    return 0;
}
