#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "mapa.h"

CodigoError cargarMapa(const char* nombreArchivo, Mapa* mapa) {
    FILE* archivo = fopen(nombreArchivo, "r");
    if (!archivo) {
        return ERROR_ARCHIVO_NO_ENCONTRADO;
    }

    mapa->filas = 0;
    mapa->columnas = 0;
    int tieneSalida = 0;

    char linea[1024];
    int filaActual = 0;
    int columnasEsperadas = -1;

    while (fgets(linea, sizeof(linea), archivo) && filaActual < MAX_FILAS) {
        // Ignorar líneas vacías/blancas
        int esVacia = 1;
        for (char* p = linea; *p; p++) {
            if (*p != ' ' && *p != '\t' && *p != '\n' && *p != '\r') {
                esVacia = 0;
                break;
            }
        }
        if (esVacia) continue;

        char* token = strtok(linea, " \t\n\r");
        int columnaActual = 0;

        while (token && columnaActual < MAX_COLUMNAS) {
            char* endptr;
            long valor = strtol(token, &endptr, 10);
            if (*endptr != '\0') {
                fclose(archivo);
                return ERROR_FORMATO_INVALIDO;
            }
            // Solo -1 (obstáculo), 0 (salida), o 1..10 (peso)
            if (!(valor == OBSTACULO || valor == SALIDA ||
                  (valor >= PESO_MINIMO && valor <= PESO_MAXIMO))) {
                fclose(archivo);
                return ERROR_VALOR_CELDA_INVALIDO;
            }

            mapa->datos[filaActual][columnaActual] = (int)valor;
            if (valor == SALIDA) tieneSalida = 1;

            columnaActual++;
            token = strtok(NULL, " \t\n\r");
        }

        if (columnaActual == 0) continue;

        if (columnasEsperadas == -1) {
            columnasEsperadas = columnaActual;
        } else if (columnaActual != columnasEsperadas) {
            fclose(archivo);
            return ERROR_FORMATO_INVALIDO;
        }

        filaActual++;
    }

    fclose(archivo);

    if (filaActual == 0 || columnasEsperadas == 0) {
        return ERROR_DIMENSIONES_INVALIDAS;
    }
    if (!tieneSalida) {
        return ERROR_SIN_SALIDA;
    }

    mapa->filas = filaActual;
    mapa->columnas = columnasEsperadas;
    return OK;
}

Mapa* generarMapaAleatorio(int filas, int columnas) {
    if (filas <= 0 || columnas <= 0 || filas > MAX_FILAS || columnas > MAX_COLUMNAS) {
        return NULL;
    }

    Mapa* mapa = (Mapa*)malloc(sizeof(Mapa));
    if (!mapa) return NULL;

    mapa->filas = filas;
    mapa->columnas = columnas;

    srand((unsigned)time(NULL));

    // Pesos y obstáculos
    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < columnas; j++) {
            int r = rand() % 100;
            if (r < 15) {
                mapa->datos[i][j] = OBSTACULO; // 15% obstáculos
            } else {
                mapa->datos[i][j] = PESO_MINIMO + rand() % 5; // peso 1..5
            }
        }
    }

    // Colocar al menos una salida accesible
    int intentos = 0;
    int salidaColocada = 0;
    while (!salidaColocada && intentos < 200) {
        int sx = rand() % filas;
        int sy = rand() % columnas;
        if (mapa->datos[sx][sy] != OBSTACULO) {
            mapa->datos[sx][sy] = SALIDA;
            salidaColocada = 1;

            // Despejar vecinos cardinales si fueran obstáculos
            const int dx[4] = {-1, 0, 1, 0};
            const int dy[4] = {0, 1, 0, -1};
            for (int k = 0; k < 4; k++) {
                int nx = sx + dx[k], ny = sy + dy[k];
                if (nx >= 0 && nx < filas && ny >= 0 && ny < columnas &&
                    mapa->datos[nx][ny] == OBSTACULO) {
                    mapa->datos[nx][ny] = PESO_MINIMO + rand() % 3;
                }
            }
        }
        intentos++;
    }

    return mapa;
}

void mostrarMapa(const Mapa* mapa) {
    printf("\n=== MAPA (%dx%d) ===\n", mapa->filas, mapa->columnas);
    printf("GUIA: # = Obstaculo, S = Salida, numeros = peso de la celda\n\n");

    // Encabezado de columnas
    printf("   ");
    for (int j = 0; j < mapa->columnas; j++) {
        printf("%2d ", j);
    }
    printf("\n");

        //CAMBIAMOS LOS PESOS -1 Y 0, ESTAN PUESTOS COMO OBSTACULO Y SALIDA PARA DARLE MÁS FLOW NOMÁS PORQUE APARECE EN UN TUTO INDIO JKLASDLJKASDLJKASD
    for (int i = 0; i < mapa->filas; i++) {
        printf("%2d ", i);
        for (int j = 0; j < mapa->columnas; j++) {
            if (mapa->datos[i][j] == OBSTACULO) {
                printf(" # ");
            } else if (mapa->datos[i][j] == SALIDA) {
                printf(" S ");
            } else {
                printf("%2d ", mapa->datos[i][j]);
            }
        }
        printf("\n");
    }
    printf("\n");
}

int esValida(const Mapa* mapa, int x, int y) {
    if (x < 0 || x >= mapa->filas || y < 0 || y >= mapa->columnas) return 0;
    if (mapa->datos[x][y] == OBSTACULO) return 0;
    return 1;
}

int esSalida(const Mapa* mapa, int x, int y) {
    if (x < 0 || x >= mapa->filas || y < 0 || y >= mapa->columnas) return 0;
    return mapa->datos[x][y] == SALIDA;
}

void liberarMapa(Mapa* mapa) {
    if (mapa) free(mapa);
}