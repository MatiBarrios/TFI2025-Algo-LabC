#ifndef MAPA_H
#define MAPA_H

#include <stddef.h>
#include "tipos.h"


// Estructura para representar el mapa del juego
typedef struct {
    int datos[MAX_FILAS][MAX_COLUMNAS]; // Matriz con los pesos/obstáculos
    int filas;                          // Número de filas del mapa
    int columnas;                       // Número de columnas del mapa
} Mapa;

// Funciones para manejar el mapa
Mapa* cargarMapa(const char* nombreArchivo);
void mostrarMapa(const Mapa* mapa);
int esValida(const Mapa* mapa, int x, int y);
int esSalida(const Mapa* mapa, int x, int y);
Mapa* generarMapaAleatorio(int filas, int columnas);

#endif