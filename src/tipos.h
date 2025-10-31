#ifndef TIPOS_H
#define TIPOS_H

// Límites globales
#define MAX_FILAS 50
#define MAX_COLUMNAS 50
#define MAX_JUGADORES 10
#define INF 1000000000

// Valores de celdas
#define OBSTACULO (-1)
#define SALIDA 0
#define PESO_MINIMO 1
#define PESO_MAXIMO 10

// Errores
typedef enum {
    OK = 0,
    ERROR_ARCHIVO_NO_ENCONTRADO,
    ERROR_FORMATO_INVALIDO,
    ERROR_DIMENSIONES_INVALIDAS,
    ERROR_VALOR_CELDA_INVALIDO,
    ERROR_SIN_SALIDA,
    ERROR_COORDENADAS_FUERA_RANGO,
    ERROR_POSICION_OBSTACULO,
    ERROR_POSICION_EN_SALIDA,
    ERROR_RUTA_IMPOSIBLE,
    ERROR_MEMORIA
} CodigoError;

// Tipos básicos
typedef struct {
    int x;
    int y;
} Posicion;

// Jugador (id solo para identificar/mostrar y guardar resultados)
typedef struct {
    int id;
    Posicion pos;
    Posicion* ruta;
    int longitudRuta;
    int distanciaTotal;
} Jugador;

#endif