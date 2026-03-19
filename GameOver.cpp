#include "gameover.h"
bool GameOver(TETRIS &t, PIEZA &p){

    // Verifica si la pieza nueva colisiona al aparecer
    for(int i=0;i<4;i++){

        int x = p.posX + p.bloques[i].x;
        int y = p.posY + p.bloques[i].y;

        if(leerBit(t,y,x))
            return true;
    }

    return false;
}
