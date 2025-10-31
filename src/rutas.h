#ifndef RUTAS_H
#define RUTAS_H

#include "tipos.h"
#include "mapa.h"

// Calcula distancias mínimas desde TODAS las salidas (0) a cada celda
void calcularDistancias(const Mapa* mapa, int distancias[MAX_FILAS][MAX_COLUMNAS]);

// Calcula distancias mínimas desde una celda específica (inicioX,inicioY)
void calcularDistanciasDesde(const Mapa* mapa, int inicioX, int inicioY, int distancias[MAX_FILAS][MAX_COLUMNAS]);

// Reconstruye la ruta desde (inicioX,inicioY) hasta la salida más cercana
Posicion* encontrarRuta(const Mapa* mapa,
                        const int distancias[MAX_FILAS][MAX_COLUMNAS],
                        int inicioX, int inicioY, int* longitudRuta);

#endif