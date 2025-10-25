#ifndef JUGADORES_H
#define JUGADORES_H

#include "tipos.h"
#include "mapa.h"

// Funciones para manejar jugadores
void inicializarJugadores(Jugador* jugadores, int numJugadores);
void asignarPosicionJugador(Jugador* jugador, int x, int y);
void calcularRutasJugadores(Jugador* jugadores, int numJugadores, 
                           const Mapa* mapa, const int distancias[MAX_FILAS][MAX_COLUMNAS]);
Jugador* encontrarGanador(Jugador* jugadores, int numJugadores);

#endif