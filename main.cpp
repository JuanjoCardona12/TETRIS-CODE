#include "tetris.h"
#include <iostream>
#include "pieza.h"
using namespace std;

int main(){

    TETRIS t;
    PIEZA p;

    crearTetris(t);
    inicializarTetris(t);
    crearBordes(t);

    nuevaPiezaAleatoria(t,p);
    dibujarPieza(t,p);

    bool fin = false;
    char tecla;

    while(!fin){

        system("cls");

        dibujarTetris(t);

        cout<<"a izquierda.    |    d derecha.    |    s abajo.    |    w rotar a la derecha"<<endl;
        cin>>tecla;

        switch(tecla){

        case 'a':
            moverIzquierda(t,p);
            break;

        case 'd':
            moverDerecha(t,p);
            break;

        case 's':
            fin = moverAbajo(t,p);
            break;

        case 'w':
            rotarDerecha(t,p);
            break;

        
        }

    }

    system("cls");
    dibujarTetris(t);
    cout<<"GAME OVER"<<endl;

    liberarTetris(t);

    return 0;
}
