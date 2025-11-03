#ifndef MAPA_H
#define MAPA_H

#include "tipos.h"

// Representar mapa
typedef struct {
    int datos[MAX_FILAS][MAX_COLUMNAS];
    int filas;
    int columnas;
} Mapa;

// Alias opcional para compatibilidad si algún archivo usa celdas
#ifndef MAPA_CELDAS_ALIAS
#define MAPA_CELDAS_ALIAS
#define celdas datos
#endif

// API
CodigoError cargarMapa(const char* nombreArchivo, Mapa* mapa);
Mapa* generarMapaAleatorio(int filas, int columnas);
void mostrarMapa(const Mapa* mapa);
int esValida(const Mapa* mapa, int x, int y);
int esSalida(const Mapa* mapa, int x, int y);
void liberarMapa(Mapa* mapa);

#endif