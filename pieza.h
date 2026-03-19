#ifndef PIEZA_H
#define PIEZA_H

#include "tetris.h"

struct BLOQUE{
    int x;
    int y;
};

struct PIEZA{
    BLOQUE bloques[4];
    int posX;
    int posY;
};
//añadir pieza.h con estructuras BLOQUE/PIEZA y funciones de movimiento, rotación y generación aleatoria

void dibujarPieza(TETRIS &t,PIEZA &p);
void borrarPieza(TETRIS &t,PIEZA &p);
bool colision(TETRIS &t,PIEZA &p);
void moverIzquierda(TETRIS &t,PIEZA &p);
void moverDerecha(TETRIS &t,PIEZA &p);
bool moverAbajo(TETRIS &t,PIEZA &p);
void rotarDerecha(TETRIS &t,PIEZA &p);
void nuevaPiezaAleatoria(TETRIS &t,PIEZA &p);

#endif
