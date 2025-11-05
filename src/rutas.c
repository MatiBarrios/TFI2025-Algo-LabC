#include <stdio.h>
#include <stdlib.h>
#include "rutas.h"
#include "mapa.h"
#include <limits.h> 

// Predecesores para reconstruir ruta
static int predX[MAX_FILAS][MAX_COLUMNAS];
static int predY[MAX_FILAS][MAX_COLUMNAS];

// Movimientos 4-dir
static const int DX[4] = {-1, 0, 1, 0};
static const int DY[4] = {0, 1, 0, -1};

// no me gusta usar esto global, pero es lo que hay
// no sabía que nombre colocarle al nodo, se llamaba nododeodo
// pero la IA me dijo que no era adecuado
typedef struct {
    int x, y, d;
} NodoFrontera;

typedef struct {
    NodoFrontera* a;
    int n;    // tam
    int cap;
} Frontera;

//lo mismo para estas weas
static int frontera_inicializar(Frontera* f, int cap) {
    f->a = (NodoFrontera*)malloc(sizeof(NodoFrontera) * cap);
    f->n = 0;
    f->cap = cap;
    return f->a != NULL;
}

static void frontera_agregar(Frontera* f, int x, int y, int d) {
    if (f->n >= f->cap) return;
    int pos = 0;
    while (pos < f->n && f->a[pos].d <= d) pos++;
    for (int j = f->n; j > pos; j--) f->a[j] = f->a[j - 1];
    f->a[pos] = (NodoFrontera){x, y, d};
    f->n++;
}

static int frontera_tomar(Frontera* f, NodoFrontera* out) {
    if (f->n == 0) return 0;
    *out = f->a[0];
    for (int i = 0; i + 1 < f->n; i++) f->a[i] = f->a[i + 1];
    f->n--;
    return 1;
}

static void frontera_liberar(Frontera* f) {
    free(f->a);
    f->a = NULL; f->n = f->cap = 0;
}

void calcularDistancias(const Mapa* mapa, int distancias[MAX_FILAS][MAX_COLUMNAS]) {
    // Inicializar distancias
    for (int i = 0; i < mapa->filas; i++) {
        for (int j = 0; j < mapa->columnas; j++) {
            distancias[i][j] = INF;
            predX[i][j] = -1;
            predY[i][j] = -1;
        }
    }

    Frontera frontera;
    if (!frontera_inicializar(&frontera, MAX_FILAS * MAX_COLUMNAS + 5)) {
        return;
    }

    // temgp sie´pp
    for (int i = 0; i < mapa->filas; i++) {
        for (int j = 0; j < mapa->columnas; j++) {
            if (mapa->datos[i][j] == SALIDA) {
                distancias[i][j] = 0;
                predX[i][j] = -1;
                predY[i][j] = -1;
                frontera_agregar(&frontera, i, j, 0);
            }
        }
    }

    // Dijkstra
    NodoFrontera cur;
    while (frontera_tomar(&frontera, &cur)) {
        if (cur.d != distancias[cur.x][cur.y]) continue;

        for (int k = 0; k < 4; k++) {
            int nx = cur.x + DX[k];
            int ny = cur.y + DY[k];
            if (!esValida(mapa, nx, ny)) continue;

            int peso = (mapa->datos[nx][ny] == SALIDA) ? 0 : mapa->datos[nx][ny];
            int nd = cur.d + peso;
            if (nd < distancias[nx][ny]) {
                distancias[nx][ny] = nd;
                predX[nx][ny] = cur.x;
                predY[nx][ny] = cur.y;
                frontera_agregar(&frontera, nx, ny, nd);
            }
        }
    }

    frontera_liberar(&frontera);
}

Posicion* encontrarRuta(const Mapa* mapa,
                        const int distancias[MAX_FILAS][MAX_COLUMNAS],
                        int inicioX, int inicioY, int* longitudRuta) {
    *longitudRuta = 0;

    if (!esValida(mapa, inicioX, inicioY)) return NULL;
    if (distancias[inicioX][inicioY] == INF) return NULL;

    int capacidad = mapa->filas * mapa->columnas;
    Posicion* ruta = (Posicion*)malloc(sizeof(Posicion) * capacidad);
    if (!ruta) return NULL;

    int len = 0;
    int x = inicioX, y = inicioY;

    // Agregar inicio
    ruta[len++] = (Posicion){x, y};

    // Si ya se encuentra en la salida
    if (mapa->datos[x][y] == SALIDA) {
        *longitudRuta = len;
        return ruta;
    }

    // Seguir predecesores
    while (predX[x][y] != -1 && predY[x][y] != -1) {
        int nx = predX[x][y];
        int ny = predY[x][y];

        if (len >= capacidad) break;
        ruta[len++] = (Posicion){nx, ny};

        x = nx; y = ny;
        if (mapa->datos[x][y] == SALIDA) break;
    }

    if (mapa->datos[x][y] != SALIDA) {
        free(ruta);
        return NULL;
    }

    *longitudRuta = len;
    return ruta;
}

void calcularDistanciasDesde(const Mapa* mapa, int inicioX, int inicioY, int distancias[MAX_FILAS][MAX_COLUMNAS]) {
    // Inicializar distancias y predecesores
    for (int i = 0; i < mapa->filas; i++) {
        for (int j = 0; j < mapa->columnas; j++) {
            distancias[i][j] = INF;
            predX[i][j] = -1;
            predY[i][j] = -1;
        }
    }

    if (inicioX < 0 || inicioX >= mapa->filas || inicioY < 0 || inicioY >= mapa->columnas) return;
    if (mapa->datos[inicioX][inicioY] != SALIDA) return; // Se espera una salida como fuente

    // Frontera con capacidad suficiente
    Frontera frontera;
    if (!frontera_inicializar(&frontera, MAX_FILAS * MAX_COLUMNAS + 5)) return;

    // Insertar solo la salida seleccionada como origen (distancia 0)
    distancias[inicioX][inicioY] = 0;
    predX[inicioX][inicioY] = -1;
    predY[inicioX][inicioY] = -1;
    frontera_agregar(&frontera, inicioX, inicioY, 0);

    // Dijkstra (mismo cuerpo que calcularDistancias)
    NodoFrontera cur;
    while (frontera_tomar(&frontera, &cur)) {
        if (cur.d != distancias[cur.x][cur.y]) continue;
        for (int k = 0; k < 4; k++) {
            int nx = cur.x + DX[k];
            int ny = cur.y + DY[k];
            if (!esValida(mapa, nx, ny)) continue;
            int peso = (mapa->datos[nx][ny] == SALIDA) ? 0 : mapa->datos[nx][ny];
            int nd = cur.d + peso;
            if (nd < distancias[nx][ny]) {
                distancias[nx][ny] = nd;
                predX[nx][ny] = cur.x;
                predY[nx][ny] = cur.y;
                frontera_agregar(&frontera, nx, ny, nd);
            }
        }
    }

    frontera_liberar(&frontera);
}


int determinar_ganadores(Jugador* jugadores, int num_jugadores, int indices[], int max_indices) {
    if (num_jugadores <= 0 || !jugadores || !indices || max_indices <= 0) return 0;

    int bestDist = INF;
    for (int i = 0; i < num_jugadores; i++) {
        if (jugadores[i].distanciaTotal < bestDist) bestDist = jugadores[i].distanciaTotal;
    }

    if (bestDist == INF) return 0; // Comunicar que nadie alcanzó una salida

    // Buscar menor cantidad de pasos entre los que tienen bestDist
    int minSteps = INT_MAX;
    for (int i = 0; i < num_jugadores; i++) {
        if (jugadores[i].distanciaTotal == bestDist) {
            int steps = (jugadores[i].longitudRuta > 0) ? jugadores[i].longitudRuta : INT_MAX;
            if (steps < minSteps) minSteps = steps;
        }
    }

    // Colectar todos los jugadores que cumplen bestDist && minSteps
    int cnt = 0;
    for (int i = 0; i < num_jugadores && cnt < max_indices; i++) {
        if (jugadores[i].distanciaTotal == bestDist) {
            int steps = (jugadores[i].longitudRuta > 0) ? jugadores[i].longitudRuta : INT_MAX;
            if (steps == minSteps) {
                indices[cnt++] = i;
            }
        }
    }
    return cnt;
}

//si los comentarios son demasiado lindos es responsabilidad de la IA
// y de Yara