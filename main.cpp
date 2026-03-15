#include "tetris.h"

int main(){

    TETRIS t;

    crearTetris(t);

    inicializarTetris(t);

    crearBordes(t);

    dibujarTetris(t);

    liberarTetris(t);

    return 0;
}
