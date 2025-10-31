#ifndef JUGADORES_H
#define JUGADORES_H

#include "tipos.h"
#include "mapa.h"

void inicializarJugadores(Jugador* jugadores, int numJugadores);

// Valida y asigna posición
CodigoError asignarPosicionJugador(Jugador* jugador, int x, int y, const Mapa* mapa);

void calcularRutasJugadores(Jugador* jugadores, int numJugadores,
                            const Mapa* mapa, const int distancias[MAX_FILAS][MAX_COLUMNAS]);

Jugador* encontrarGanador(Jugador* jugadores, int numJugadores);

void liberarJugadores(Jugador* jugadores, int numJugadores);

#endif