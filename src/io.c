#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#ifdef _WIN32
#include <direct.h>
#endif
#include <string.h>
#include <time.h>
#include "io.h"

void leer_posicion_jugador(int *x, int *y) {
    printf("Ingrese la posición inicial del jugador (fila columna): ");
    if (scanf("%d %d", x, y) != 2) {
        *x = *y = -1;
    }
}

void mostrar_resultado_jugador(const Jugador* jugador) {
    if (!jugador) return;
    if (jugador->distanciaTotal == INF) {
        printf("Jugador %d no tiene ruta válida.\n", jugador->id);
        return;
    }
    printf("Jugador %d - peso total: %d\nRuta: ", jugador->id, jugador->distanciaTotal);
    for (int i = 0; i < jugador->longitudRuta; i++) {
        printf("(%d,%d)", jugador->ruta[i].x, jugador->ruta[i].y);
        if (i + 1 < jugador->longitudRuta) printf(" -> ");
    }
    printf("\n");
}

void mostrar_error(const char *mensaje) {
    fprintf(stderr, "Error: %s\n", mensaje);
}

void mostrarMapaIO(const Mapa* mapa) {
    // reutiliza la función de mapa
    if (mapa) {
        for (int i = 0; i < mapa->filas; i++) {
            for (int j = 0; j < mapa->columnas; j++) {
                printf("%2d ", mapa->datos[i][j]);
            }
            printf("\n");
        }
    }
}

static void ensure_output_dir(const char* dirpath) {
#ifdef _WIN32
    _mkdir(dirpath);
#else
    mkdir(dirpath, 0755);
#endif
}

void guardar_mejor_jugador(const Jugador* jugador, int ronda) {
    if (!jugador) return;

    // ruta: datos/output/mejores.txt
    const char* dir = "datos/output";
    const char* filepath = "datos/output/mejores.txt";

    ensure_output_dir(dir);

    FILE* f = fopen(filepath, "a");
    if (!f) {
        fprintf(stderr, "No se pudo abrir %s para escritura\n", filepath);
        return;
    }

    time_t t = time(NULL);
    char buf[64];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&t));

    fprintf(f, "Ronda %d | %s | Jugador %d | pos=(%d,%d) | distancia=%d\n",
            ronda, buf, jugador->id, jugador->pos.x, jugador->pos.y, jugador->distanciaTotal);

    fclose(f);
}

void mostrar_mejores_archivo(void) {
    const char* filepath = "datos/output/mejores.txt";
    FILE* f = fopen(filepath, "r");
    if (!f) {
        printf("No existe el archivo de mejores en '%s'\n", filepath);
        return;
    }

    printf("\nContenido de %s:\n\n", filepath);
    char linea[512];
    while (fgets(linea, sizeof(linea), f)) {
        fputs(linea, stdout);
    }
    fclose(f);
}