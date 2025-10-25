#ifndef IO_H
#define IO_H

#include "tipos.h"
#include "mapa.h"

void leer_posicion_jugador(int *x, int *y);
void mostrar_resultado_jugador(const Jugador* jugador);
void mostrar_error(const char *mensaje);
void mostrarMapaIO(const Mapa* mapa);

// Funciones para manejo de mejores jugadores
// Esto me sacó de quicio
void guardar_mejor_jugador(const Jugador* jugador, int ronda);
void mostrar_mejores_archivo(void);

#endif