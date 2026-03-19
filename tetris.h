
#ifndef TETRIS_H
#define TETRIS_H

struct TETRIS{
    int ancho;
    int alto;
    int bytesPorFila;
    unsigned char **pantalla;
};

// funciones
void crearTetris(TETRIS &t);
void inicializarTetris(TETRIS &t);
void dibujarTetris(TETRIS &t);
void encenderBit(TETRIS &t,int y,int x);
void apagarBit(TETRIS &t,int y,int x);
bool leerBit(TETRIS &t,int y,int x);
void crearBordes(TETRIS &t);
void liberarTetris(TETRIS &t);
bool filaCompleta(TETRIS &t, int y);
void eliminarFila(TETRIS &t, int y);
void verificarLineas(TETRIS &t);

#endif
#endif


