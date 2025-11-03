#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#ifdef _WIN32
#include <direct.h>
#define MKDIR(p) _mkdir(p)
#else
#define MKDIR(p) mkdir(p, 0777)
#endif
#include "io.h"
#include "tipos.h"

static void io_trim_nl(char* s) {
    if (!s) return;
    size_t l = strlen(s);
    if (l && (s[l - 1] == '\n' || s[l - 1] == '\r')) s[l - 1] = '\0';
}

void io_leer_ruta_archivo(char* ruta, size_t n, const char* sugerida) {
    // limpiar salto pendiente de scanf
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {}

    printf("Ingrese ruta del archivo (ej: %s)\n", sugerida);
    printf("[ENTER para usar por defecto: %s]: ", sugerida);

    if (!fgets(ruta, (int)n, stdin)) {
        // fallback
        strncpy(ruta, sugerida, n);
        ruta[n - 1] = '\0';
        return;
    }
    io_trim_nl(ruta);
    if (ruta[0] == '\0') {
        strncpy(ruta, sugerida, n);
        ruta[n - 1] = '\0';
    }
}

CodigoError guardar_mapa_txt(const Mapa* mapa, const char* ruta) {
    if (!mapa || !ruta) return ERROR_FORMATO_INVALIDO;

    // Crear carpeta "datos" si corresponde
    
    MKDIR("datos");

    FILE* f = fopen(ruta, "w");
    if (!f) return ERROR_ARCHIVO_NO_ENCONTRADO;

    for (int i = 0; i < mapa->filas; i++) {
        for (int j = 0; j < mapa->columnas; j++) {
            if (j) fputc(' ', f);
            fprintf(f, "%d", mapa->datos[i][j]);
        }
        fputc('\n', f);
    }
    fclose(f);
    return OK;
}

CodigoError leer_posicion_jugador(const Mapa* mapa, int* x, int* y, int jugadorId) {
    printf("\n--- Jugador %d ---\n", jugadorId);
    printf("Ingrese coordenadas de inicio (fila columna): ");
    if (scanf("%d %d", x, y) != 2) {
        int c; while ((c = getchar()) != '\n' && c != EOF);
        return ERROR_FORMATO_INVALIDO;
    }
    if (*x < 0 || *x >= mapa->filas || *y < 0 || *y >= mapa->columnas) {
        return ERROR_COORDENADAS_FUERA_RANGO;
    }
    if (mapa->datos[*x][*y] == OBSTACULO) {
        return ERROR_POSICION_OBSTACULO;
    }
    if (mapa->datos[*x][*y] == SALIDA) {
        return ERROR_POSICION_EN_SALIDA;
    }
    return OK;
}

void mostrar_resultado_jugador(const Jugador* jugador, const Mapa* mapa) {
    printf("\n========== JUGADOR %d ==========\n", jugador->id);
    printf("Posicion inicial: (%d, %d)\n", jugador->pos.x, jugador->pos.y);

    if (!jugador->ruta || jugador->distanciaTotal == INF) {
        printf("NO HAY RUTA POSIBLE\n");
        return;
    }

    printf("Distancia total: %d\n", jugador->distanciaTotal);
    printf("Pasos en la ruta: %d\n", jugador->longitudRuta);

    printf("\nRuta:\n");
    for (int i = 0; i < jugador->longitudRuta; i++) {
        int px = jugador->ruta[i].x, py = jugador->ruta[i].y;
        if (mapa->datos[px][py] == SALIDA) {
            printf("  %2d. (%d,%d) -> SALIDA\n", i + 1, px, py);
        } else {
            printf("  %2d. (%d,%d) peso=%d\n", i + 1, px, py, mapa->datos[px][py]);
        }
    }
}

void mostrar_error(CodigoError codigo) {
    printf("\nERROR: ");
    switch (codigo) {
        case OK: return;
        case ERROR_ARCHIVO_NO_ENCONTRADO: printf("No se pudo abrir el archivo\n"); break;
        case ERROR_FORMATO_INVALIDO: printf("Formato inválido (archivo o entrada)\n"); break;
        case ERROR_DIMENSIONES_INVALIDAS: printf("Dimensiones del mapa inválidas o mapa vacío\n"); break;
        case ERROR_VALOR_CELDA_INVALIDO: printf("Valor de celda invalido (-1, 0 o 1, -10)\n"); break;
        case ERROR_SIN_SALIDA: printf("El mapa no contiene ninguna salida (0)\n"); break;
        case ERROR_COORDENADAS_FUERA_RANGO: printf("Coordenadas fuera del rango del mapa\n"); break;
        case ERROR_POSICION_OBSTACULO: printf("No se puede iniciar en un obstaculo (#)\n"); break;
        case ERROR_POSICION_EN_SALIDA: printf("No se puede iniciar en una salida (S)\n"); break;
        case ERROR_RUTA_IMPOSIBLE: printf("No existe ruta hacia una salida\n"); break;
        case ERROR_MEMORIA: printf("Fallo al asignar memoria\n"); break;
        default: printf("Desconocido\n"); break;
    }
}

void guardar_mejor_jugador(const Jugador* jugador) {
    MKDIR("datos");
    MKDIR("datos/output");
    FILE* f = fopen("datos/output/mejores.txt", "a");
    if (!f) {
        printf("No se pudo guardar el resultado\n");
        return;
    }
    fprintf(f, "Jugador %d | Inicio: (%d,%d) | Distancia: %d | Pasos: %d\n",
            jugador->id, jugador->pos.x, jugador->pos.y,
            jugador->distanciaTotal, jugador->longitudRuta);
    fclose(f);
    printf("Guardado en datos/output/mejores.txt\n");
}

void mostrar_mejores_archivo() {
    FILE* f = fopen("datos/output/mejores.txt", "r");
    if (!f) {
        printf("\nNo hay registros previos.\n");
        return;
    }
    printf("\n================= MEJORES RESULTADOS =================\n");
    char linea[256];
    while (fgets(linea, sizeof(linea), f)) {
        printf("%s", linea);
    }
    printf("=======================================================\n");
    fclose(f);
}