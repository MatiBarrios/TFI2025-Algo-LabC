#ifndef IO_H
#define IO_H

#include "tipos.h"
#include "mapa.h"
#include "jugadores.h"

CodigoError leer_posicion_jugador(const Mapa* mapa, int* x, int* y, int jugadorId);
void mostrar_resultado_jugador(const Jugador* jugador, const Mapa* mapa);
void mostrar_error(CodigoError codigo);
void guardar_mejor_jugador(const Jugador* jugador);
void mostrar_mejores_archivo();

#endif