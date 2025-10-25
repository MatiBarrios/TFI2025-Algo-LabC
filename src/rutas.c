
#include <stdio.h>
#include <stdlib.h>
#include "rutas.h"

// Nodo para heap mínimo
typedef struct {
    int x, y;
    int distancia;
} NodoCola;

// Movimientos 4-direcciones
static const int dx[] = {-1, 0, 1, 0};
static const int dy[] = {0, 1, 0, -1};

// Cola de prioridad (heap mínimo)
typedef struct {
    NodoCola datos[MAX_FILAS * MAX_COLUMNAS];
    int tamano;
} ColaPrioridad;

static void inicializarCola(ColaPrioridad* cola) {
    cola->tamano = 0;
}

static void insertar(ColaPrioridad* cola, int x, int y, int distancia) {
    int i = cola->tamano++;
    // subir en el heap
    while (i > 0) {
        int padre = (i - 1) / 2;
        if (cola->datos[padre].distancia <= distancia) break;
        cola->datos[i] = cola->datos[padre];
        i = padre;
    }
    cola->datos[i].x = x;
    cola->datos[i].y = y;
    cola->datos[i].distancia = distancia;
}

static NodoCola extraerMinimo(ColaPrioridad* cola) {
    NodoCola vacio = { -1, -1, INF };
    if (cola->tamano == 0) return vacio;

    NodoCola resultado = cola->datos[0];
    NodoCola ultimo = cola->datos[--cola->tamano];

    int i = 0;
    while (2 * i + 1 < cola->tamano) {
        int hijoIzq = 2 * i + 1;
        int hijoDer = 2 * i + 2;
        int hijo = hijoIzq;
        if (hijoDer < cola->tamano && cola->datos[hijoDer].distancia < cola->datos[hijoIzq].distancia)
            hijo = hijoDer;
        if (ultimo.distancia <= cola->datos[hijo].distancia) break;
        cola->datos[i] = cola->datos[hijo];
        i = hijo;
    }
    cola->datos[i] = ultimo;
    return resultado;
}

// Predecesores para reconstrucción (llenados en calcularDistancias)
static int predX[MAX_FILAS][MAX_COLUMNAS];
static int predY[MAX_FILAS][MAX_COLUMNAS];

void calcularDistancias(const Mapa* mapa, int distancias[MAX_FILAS][MAX_COLUMNAS]) {
    ColaPrioridad cola;
    inicializarCola(&cola);

    // inicializar
    for (int i = 0; i < mapa->filas; i++) {
        for (int j = 0; j < mapa->columnas; j++) {
            distancias[i][j] = INF;
            predX[i][j] = -1;
            predY[i][j] = -1;
        }
    }

    // insertar todas las salidas (celdas con peso 0)
    for (int i = 0; i < mapa->filas; i++) {
        for (int j = 0; j < mapa->columnas; j++) {
            if (mapa->datos[i][j] == 0) {
                distancias[i][j] = 0;
                predX[i][j] = -1; // marca salida
                predY[i][j] = -1;
                insertar(&cola, i, j, 0);
            }
        }
    }

    // Dijkstra multi-origen (coste mover u->v = peso de v)
    while (cola.tamano > 0) {
        NodoCola actual = extraerMinimo(&cola);
        if (actual.x < 0) break; // safety

        if (actual.distancia > distancias[actual.x][actual.y]) continue;

        for (int dir = 0; dir < 4; dir++) {
            int nx = actual.x + dx[dir];
            int ny = actual.y + dy[dir];

            if (!esValida(mapa, nx, ny)) continue;
            if (mapa->datos[nx][ny] == -1) continue; // obstáculo

            int pesoVecino = mapa->datos[nx][ny];
            int nuevaDist = actual.distancia + pesoVecino;

            if (nuevaDist < distancias[nx][ny]) {
                distancias[nx][ny] = nuevaDist;
                predX[nx][ny] = actual.x;
                predY[nx][ny] = actual.y;
                insertar(&cola, nx, ny, nuevaDist);
            }
        }
    }
}

Posicion* encontrarRuta(const Mapa* mapa, const int distancias[MAX_FILAS][MAX_COLUMNAS],
                       int inicioX, int inicioY, int* longitudRuta) {
    // validaciones
    if (!esValida(mapa, inicioX, inicioY) || distancias[inicioX][inicioY] == INF) {
        *longitudRuta = 0;
        return NULL;
    }

    // reservar buffer para ruta
    int cap = mapa->filas * mapa->columnas;
    Posicion* ruta = malloc(sizeof(Posicion) * cap);
    if (!ruta) {
        *longitudRuta = 0;
        return NULL;
    }

    int len = 0;
    int x = inicioX;
    int y = inicioY;

    // agregar inicio
    ruta[len].x = x;
    ruta[len].y = y;
    len++;

    // si inicio ya es salida
    if (mapa->datos[x][y] == 0) {
        *longitudRuta = len;
        return ruta;
    }

    // seguir predecesores hacia la salida
    while (predX[x][y] != -1 && predY[x][y] != -1) {
        int nx = predX[x][y];
        int ny = predY[x][y];

        // seguridad contra bucle infinito
        if (len >= cap) break;

        ruta[len].x = nx;
        ruta[len].y = ny;
        len++;

        x = nx;
        y = ny;

        if (mapa->datos[x][y] == 0) break; // alcanzada salida
    }

    // verificar que terminamos en una salida
    if (mapa->datos[x][y] != 0) {
        free(ruta);
        *longitudRuta = 0;
        return NULL;
    }

    *longitudRuta = len;
    return ruta;
}