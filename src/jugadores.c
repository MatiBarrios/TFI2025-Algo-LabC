#include <stdio.h>
#include <stdlib.h>
#include "jugadores.h"
#include "rutas.h"

void inicializarJugadores(Jugador* jugadores, int numJugadores) {
    for (int i = 0; i < numJugadores; i++) {
        jugadores[i].id = i + 1;
        jugadores[i].distanciaTotal = INF;
        jugadores[i].ruta = NULL;
        jugadores[i].longitudRuta = 0;
    }
}

void asignarPosicionJugador(Jugador* jugador, int x, int y) {
    jugador->pos.x = x;
    jugador->pos.y = y;
}

void calcularRutasJugadores(Jugador* jugadores, int numJugadores, 
                           const Mapa* mapa, const int distancias[MAX_FILAS][MAX_COLUMNAS]) {
    for (int i = 0; i < numJugadores; i++) {
        if (jugadores[i].ruta != NULL) {
            free(jugadores[i].ruta);
        }
        
        jugadores[i].ruta = encontrarRuta(mapa, distancias, 
                                         jugadores[i].pos.x, jugadores[i].pos.y,
                                         &jugadores[i].longitudRuta);
                                         
        if (jugadores[i].ruta != NULL) {
            jugadores[i].distanciaTotal = distancias[jugadores[i].pos.x][jugadores[i].pos.y];
        } else {
            jugadores[i].distanciaTotal = INF;
        }
    }
}

Jugador* encontrarGanador(Jugador* jugadores, int numJugadores) {
    Jugador* ganador = &jugadores[0];
    for (int i = 1; i < numJugadores; i++) {
        if (jugadores[i].distanciaTotal < ganador->distanciaTotal) {
            ganador = &jugadores[i];
        }
    }
    return ganador;
}