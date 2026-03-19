#include "tetris.h"
#include "pieza.h"
#include <iostream>
#include <random>

using namespace std;

// Configuración de aleatoriedad uniforme para la generación de piezas
static mt19937 rng(random_device{}());
static uniform_int_distribution<int> dist(0, 6);

void crearTetris(TETRIS &t) {
    // Validación de dimensiones: El ancho debe ser múltiplo de 8 para la gestión de bytes
    cout << "Ingrese ancho (multiplo de 8, min 8): ";
    cin >> t.ancho;
    while (t.ancho < 8 || t.ancho % 8 != 0) {
        cout << "Invalido. Debe ser multiplo de 8: ";
        cin >> t.ancho;
    }

    cout << "Ingrese alto (min 8): ";
    cin >> t.alto;
    while (t.alto < 8) {
        cout << "Invalido. Minimo 8: ";
        cin >> t.alto;
    }

    t.bytesPorFila = t.ancho / 8;
    // Asignación de memoria dinámica 
    t.pantalla = new unsigned char*[t.alto]; 
    for (int i = 0; i < t.alto; i++)
        t.pantalla[i] = new unsigned char[t.bytesPorFila];
}

void inicializarTetris(TETRIS &t) {
    for (int i = 0; i < t.alto; i++)
        for (int j = 0; j < t.bytesPorFila; j++)
            t.pantalla[i][j] = 0; // Limpieza inicial de bits
}

void encenderBit(TETRIS &t, int y, int x) {
    // Operación OR (|) para activar el bit correspondiente
    t.pantalla[y][x / 8] |= (1 << (7 - (x % 8)));
}

void apagarBit(TETRIS &t, int y, int x) {
    // Operación AND con NOT (& ~) para desactivar el bit
    t.pantalla[y][x / 8] &= ~(1 << (7 - (x % 8)));
}

bool leerBit(TETRIS &t, int y, int x) {
    // Verificación de estado mediante máscara de bits
    return t.pantalla[y][x / 8] & (1 << (7 - (x % 8)));
}

void dibujarTetris(TETRIS &t) {
    for (int i = 0; i < t.alto; i++) {
        for (int j = 0; j < t.ancho; j++) {
            cout << (leerBit(t, i, j) ? "#" : "."); 
        }
        cout << endl;
    }
}

void liberarTetris(TETRIS &t) {
    // Liberación  de la memoria dinámica
    for (int i = 0; i < t.alto; i++)
        delete[] t.pantalla[i];
    delete[] t.pantalla;
}

bool colision(TETRIS &t, PIEZA &p) {
    for (int i = 0; i < 4; i++) {
        int x = p.posX + p.bloques[i].x;
        int y = p.posY + p.bloques[i].y;
        if (leerBit(t, y, x)) return true;
    }
    return false;
}

void rotarDerecha(TETRIS &t, PIEZA &p) {
    borrarPieza(t, p);
    BLOQUE copia[4];
    for(int i=0; i<4; i++) copia[i] = p.bloques[i];

    for (int i = 0; i < 4; i++) {
        // Rotación de 90 grados: (x, y) -> (-y, x)
        int temp = p.bloques[i].x;
        p.bloques[i].x = -p.bloques[i].y;
        p.bloques[i].y = temp;
    }

    if (colision(t, p)) {
        for(int i=0; i<4; i++) p.bloques[i] = copia[i];
    }
    dibujarPieza(t, p);
}

void eliminarFila(TETRIS &t, int y) {
    for (int i = y; i > 1; i--) {
        for (int j = 0; j < t.bytesPorFila; j++)
            t.pantalla[i][j] = t.pantalla[i - 1][j];
    }
    for(int j=0; j < t.bytesPorFila; j++) t.pantalla[1][j] = 0;
    encenderBit(t, 1, 0); 
    encenderBit(t, 1, t.ancho-1);
}

void verificarLineas(TETRIS &t) {
    for (int y = 1; y < t.alto - 1; y++) {
        if (filaCompleta(t, y)) eliminarFila(t, y);
    }
}

bool moverAbajo(TETRIS &t, PIEZA &p) {
    borrarPieza(t, p);
    p.posY++;
    if (colision(t, p)) {
        p.posY--; 
        dibujarPieza(t, p);
        verificarLineas(t);
        nuevaPiezaAleatoria(t, p);
        if (colision(t, p)) return true; // Fin del juego
    }
    dibujarPieza(t, p);
    return false;
}
