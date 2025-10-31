#include <stdio.h>
#include <stdlib.h>
#include "rutas.h"
#include "mapa.h"

// Predecesores para reconstruir ruta
static int predX[MAX_FILAS][MAX_COLUMNAS];
static int predY[MAX_FILAS][MAX_COLUMNAS];

// Movimientos 4-dir
static const int DX[4] = {-1, 0, 1, 0};
static const int DY[4] = {0, 1, 0, -1};

// Cola de prioridad mínima (bin-heap) para Dijkstra
typedef struct {
    int x, y, d;
} HeapNode;

typedef struct {
    HeapNode* a;
    int n;       // tamaño actual
    int cap;     // capacidad
} MinHeap;

static void heap_swap(HeapNode* p, HeapNode* q) { HeapNode t = *p; *p = *q; *q = t; }

static void heap_up(MinHeap* h, int i) {
    while (i > 0) {
        int p = (i - 1) / 2;
        if (h->a[p].d <= h->a[i].d) break;
        heap_swap(&h->a[p], &h->a[i]);
        i = p;
    }
}

static void heap_down(MinHeap* h, int i) {
    while (1) {
        int l = 2 * i + 1, r = 2 * i + 2, m = i;
        if (l < h->n && h->a[l].d < h->a[m].d) m = l;
        if (r < h->n && h->a[r].d < h->a[m].d) m = r;
        if (m == i) break;
        heap_swap(&h->a[i], &h->a[m]);
        i = m;
    }
}

static int heap_init(MinHeap* h, int cap) {
    h->a = (HeapNode*)malloc(sizeof(HeapNode) * cap);
    h->n = 0;
    h->cap = cap;
    return h->a != NULL;
}

static void heap_push(MinHeap* h, int x, int y, int d) {
    if (h->n >= h->cap) return; // silenciar overflow (cap suficiente)
    h->a[h->n] = (HeapNode){x, y, d};
    heap_up(h, h->n);
    h->n++;
}

static int heap_pop(MinHeap* h, HeapNode* out) {
    if (h->n == 0) return 0;
    *out = h->a[0];
    h->n--;
    if (h->n > 0) {
        h->a[0] = h->a[h->n];
        heap_down(h, 0);
    }
    return 1;
}

static void heap_free(MinHeap* h) {
    free(h->a);
    h->a = NULL; h->n = h->cap = 0;
}

void calcularDistancias(const Mapa* mapa, int distancias[MAX_FILAS][MAX_COLUMNAS]) {
    // Inicializar distancias y predecesores
    for (int i = 0; i < mapa->filas; i++) {
        for (int j = 0; j < mapa->columnas; j++) {
            distancias[i][j] = INF;
            predX[i][j] = -1;
            predY[i][j] = -1;
        }
    }

    // Heap con capacidad suficiente
    MinHeap heap;
    if (!heap_init(&heap, MAX_FILAS * MAX_COLUMNAS + 5)) {
        // Si no hay memoria, dejamos distancias=INF y retornamos
        return;
    }

    // Insertar todas las salidas como origen (distancia 0)
    for (int i = 0; i < mapa->filas; i++) {
        for (int j = 0; j < mapa->columnas; j++) {
            if (mapa->datos[i][j] == SALIDA) {
                distancias[i][j] = 0;
                predX[i][j] = -1;
                predY[i][j] = -1;
                heap_push(&heap, i, j, 0);
            }
        }
    }

    // Dijkstra
    HeapNode cur;
    while (heap_pop(&heap, &cur)) {
        if (cur.d != distancias[cur.x][cur.y]) continue;

        for (int k = 0; k < 4; k++) {
            int nx = cur.x + DX[k];
            int ny = cur.y + DY[k];
            if (!esValida(mapa, nx, ny)) continue;           // descarta fuera de rango u obstáculo

            int peso = (mapa->datos[nx][ny] == SALIDA) ? 0 : mapa->datos[nx][ny];
            int nd = cur.d + peso;
            if (nd < distancias[nx][ny]) {
                distancias[nx][ny] = nd;
                predX[nx][ny] = cur.x;
                predY[nx][ny] = cur.y;
                heap_push(&heap, nx, ny, nd);
            }
        }
    }

    heap_free(&heap);
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

    // Si ya estamos en la salida
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