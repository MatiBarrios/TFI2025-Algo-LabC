#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "mapa.h"

Mapa* cargarMapa(const char* nombreArchivo) {
    FILE* archivo = fopen(nombreArchivo, "r");
    if (!archivo) {
        printf("Error al abrir el archivo %s\n", nombreArchivo);
        return NULL;
    }

    Mapa* mapa = (Mapa*)malloc(sizeof(Mapa));
    mapa->filas = 0;
    mapa->columnas = 0;

    // Lee el mapa línea por línea
    char linea[1024];
    while (fgets(linea, sizeof(linea), archivo) && mapa->filas < MAX_FILAS) {
        char* token = strtok(linea, " \n");
        mapa->columnas = 0;
        
        while (token && mapa->columnas < MAX_COLUMNAS) {
            mapa->datos[mapa->filas][mapa->columnas] = atoi(token);
            token = strtok(NULL, " \n");
            mapa->columnas++;
        }
        mapa->filas++;
    }

    fclose(archivo);
    return mapa;
}

Mapa* generarMapaAleatorio(int filas, int columnas) {
    if (filas <= 0 || columnas <= 0 || filas > MAX_FILAS || columnas > MAX_COLUMNAS) return NULL;

    Mapa* mapa = (Mapa*)malloc(sizeof(Mapa));
    if (!mapa) return NULL;

    mapa->filas = filas;
    mapa->columnas = columnas;

    // inicializar semilla
    srand((unsigned) time(NULL));

    // rellenar con valores entre 1 y 5
    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < columnas; j++) {
            mapa->datos[i][j] = 1 + rand() % 5;
        }
    }

    // colocar al menos una salida (peso 0) en posición aleatoria
    int sx = rand() % filas;
    int sy = rand() % columnas;
    mapa->datos[sx][sy] = 0;

    return mapa;
}

void mostrarMapa(const Mapa* mapa) {
    printf("\nMapa (%dx%d):\n", mapa->filas, mapa->columnas);
    for (int i = 0; i < mapa->filas; i++) {
        for (int j = 0; j < mapa->columnas; j++) {
            printf("%2d ", mapa->datos[i][j]);
        }
        printf("\n");
    }
}

int esValida(const Mapa* mapa, int x, int y) {
    return x >= 0 && x < mapa->filas && y >= 0 && y < mapa->columnas;
}

int esSalida(const Mapa* mapa, int x, int y) {
    return mapa->datos[x][y] == 0;
}