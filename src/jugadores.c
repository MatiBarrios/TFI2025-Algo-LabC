#include <stdio.h>
#include <stdlib.h>
#include "jugadores.h"
#include "rutas.h"
#include "mapa.h"

void inicializarJugadores(Jugador* jugadores, int numJugadores) {
    for (int i = 0; i < numJugadores; i++) {
        jugadores[i].id = i + 1;
        jugadores[i].pos.x = -1;
        jugadores[i].pos.y = -1;
        jugadores[i].distanciaTotal = INF;
        jugadores[i].ruta = NULL;
        jugadores[i].longitudRuta = 0;
    }
}

CodigoError asignarPosicionJugador(Jugador* jugador, int x, int y, const Mapa* mapa) {
    if (x < 0 || x >= mapa->filas || y < 0 || y >= mapa->columnas) {
        return ERROR_COORDENADAS_FUERA_RANGO;
    }
    if (mapa->datos[x][y] == OBSTACULO) {
        return ERROR_POSICION_OBSTACULO;
    }
    if (mapa->datos[x][y] == SALIDA) {
        return ERROR_POSICION_EN_SALIDA;
    }
    jugador->pos.x = x;
    jugador->pos.y = y;
    return OK;
}

void calcularRutasJugadores(Jugador* jugadores, int numJugadores,
                            const Mapa* mapa, const int distancias[MAX_FILAS][MAX_COLUMNAS]) {
    for (int i = 0; i < numJugadores; i++) {
        if (jugadores[i].ruta) {
            free(jugadores[i].ruta);
            jugadores[i].ruta = NULL;
        }
        jugadores[i].ruta = encontrarRuta(mapa, distancias,
                                          jugadores[i].pos.x, jugadores[i].pos.y,
                                          &jugadores[i].longitudRuta);
        if (jugadores[i].ruta) {
            jugadores[i].distanciaTotal = distancias[jugadores[i].pos.x][jugadores[i].pos.y];
        } else {
            jugadores[i].distanciaTotal = INF;
        }
    }
}

Jugador* encontrarGanador(Jugador* jugadores, int numJugadores) {
    if (numJugadores <= 0) return NULL;
    Jugador* ganador = &jugadores[0];
    for (int i = 1; i < numJugadores; i++) {
        if (jugadores[i].distanciaTotal < ganador->distanciaTotal) {
            ganador = &jugadores[i];
        }
    }
    if (ganador->distanciaTotal == INF) return NULL;
    return ganador;
}

void liberarJugadores(Jugador* jugadores, int numJugadores) {
    for (int i = 0; i < numJugadores; i++) {
        if (jugadores[i].ruta) {
            free(jugadores[i].ruta);
            jugadores[i].ruta = NULL;
        }
    }
}