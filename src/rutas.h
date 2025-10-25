#ifndef RUTAS_H
#define RUTAS_H

#include "tipos.h"
#include "mapa.h"

// Calcula las distancias mínimas desde cada punto hasta la salida más cercana
void calcularDistancias(const Mapa* mapa, int distancias[MAX_FILAS][MAX_COLUMNAS]);

// Encuentra la ruta óptima desde un punto inicial hasta la salida más cercana
Posicion* encontrarRuta(const Mapa* mapa, const int distancias[MAX_FILAS][MAX_COLUMNAS], 
                       int inicioX, int inicioY, int* longitudRuta);

#endif