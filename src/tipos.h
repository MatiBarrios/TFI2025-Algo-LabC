#ifndef TIPOS_H
#define TIPOS_H

#define MAX_FILAS 100
#define MAX_COLUMNAS 100
#define MAX_JUGADORES 10
#define INF 99999

// Estructura para representar una posición en el mapa
typedef struct {
    int x;
    int y;
} Posicion;

// Estructura para almacenar información de cada jugador
typedef struct {
    int id;             // Identificador único del jugador
    Posicion pos;       // Posición actual del jugador
    int distanciaTotal; // Distancia total a recorrer
    Posicion* ruta;     // Array de posiciones que forman la ruta
    int longitudRuta;   // Cantidad de pasos en la ruta
} Jugador;

#endif