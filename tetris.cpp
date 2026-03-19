#include "tetris.h"
#include <iostream>
#include <random>
#include "pieza.h"
#include "gameover.h"
using namespace std;

mt19937 rng(random_device{}());
uniform_int_distribution<int> dist(0,6);

void crearTetris(TETRIS &t){

    cout<<"Ingrese ancho: ";
    cin>>t.ancho;
//validacion para una resolucion 1080
    while(t.ancho < 8 || t.ancho % 8 !=0 || t.ancho > 144){
        cout<<"Ancho invalido (maximo 144 y multiplo de 8),resolucion dada para 1200p: ";
        cin>>t.ancho;
    }

    cout<<"Ingrese alto: ";
    cin>>t.alto;

    while(t.alto < 8 || t.alto > 40){
        cout<<"Alto invalido (minimo 8, maximo 40) resolucion dada para 1200p: ";
        cin>>t.alto;
    }


    t.bytesPorFila = t.ancho/8;

    t.pantalla = new unsigned char*[t.alto];

    for(int i=0;i<t.alto;i++)
        t.pantalla[i] = new unsigned char[t.bytesPorFila];
}

void inicializarTetris(TETRIS &t){

    for(int i=0;i<t.alto;i++)
        for(int j=0;j<t.bytesPorFila;j++)
            t.pantalla[i][j]=0;
}

void encenderBit(TETRIS &t,int y,int x){
    t.pantalla[y][x/8] |= (1<<(7-(x%8)));
}

void apagarBit(TETRIS &t,int y,int x){
    t.pantalla[y][x/8] &= ~(1<<(7-(x%8)));
}

bool leerBit(TETRIS &t,int y,int x){
    return t.pantalla[y][x/8] & (1<<(7-(x%8)));
}

void crearBordes(TETRIS &t){

    for(int x=0;x<t.ancho;x++){
        encenderBit(t,0,x);
        encenderBit(t,t.alto-1,x);
    }

    for(int y=0;y<t.alto;y++){
        encenderBit(t,y,0);
        encenderBit(t,y,t.ancho-1);
    }
}

void dibujarTetris(TETRIS &t){

    for(int i=0;i<t.alto;i++){

        for(int j=0;j<t.ancho;j++){

            if(leerBit(t,i,j))
                cout<<"*";
            else
                cout<<" ";
        }

        cout<<endl;
    }
}

void liberarTetris(TETRIS &t){

    for(int i=0;i<t.alto;i++)
        delete[] t.pantalla[i];

    delete[] t.pantalla;
}

void dibujarPieza(TETRIS &t,PIEZA &p){

    for(int i=0;i<4;i++){

        int x=p.posX+p.bloques[i].x;
        int y=p.posY+p.bloques[i].y;

        encenderBit(t,y,x);
    }
}

void borrarPieza(TETRIS &t,PIEZA &p){

    for(int i=0;i<4;i++){

        int x=p.posX+p.bloques[i].x;
        int y=p.posY+p.bloques[i].y;

        apagarBit(t,y,x);
    }
}

bool colision(TETRIS &t,PIEZA &p){

    for(int i=0;i<4;i++){

        int x=p.posX+p.bloques[i].x;
        int y=p.posY+p.bloques[i].y;

        if(leerBit(t,y,x))
            return true;
    }

    return false;
}

void moverIzquierda(TETRIS &t,PIEZA &p){

    borrarPieza(t,p);

    p.posX--;

    if(colision(t,p))
        p.posX++;

    dibujarPieza(t,p);
}

void moverDerecha(TETRIS &t,PIEZA &p){

    borrarPieza(t,p);

    p.posX++;

    if(colision(t,p))
        p.posX--;

    dibujarPieza(t,p);
}

void rotarDerecha(TETRIS &t,PIEZA &p){

    borrarPieza(t,p);

    for(int i=0;i<4;i++){

        int nx = p.bloques[i].y;
        int ny = -p.bloques[i].x;

        int tableroX = p.posX + nx;
        int tableroY = p.posY + ny;

        if(leerBit(t,tableroY,tableroX)){
            dibujarPieza(t,p);
            return;
        }
    }

    for(int i=0;i<4;i++){

        int temp = p.bloques[i].x;

        p.bloques[i].x = p.bloques[i].y;
        p.bloques[i].y = -temp;
    }

    dibujarPieza(t,p);
}

bool filaCompleta(TETRIS &t,int y){

    for(int x=1;x<t.ancho-1;x++){

        if(!leerBit(t,y,x))
            return false;
    }

    return true;
}

void eliminarFila(TETRIS &t,int y){

    for(int i=y;i>1;i--){

        for(int j=0;j<t.bytesPorFila;j++)
            t.pantalla[i][j]=t.pantalla[i-1][j];
    }

    for(int j=0;j<t.bytesPorFila;j++)
        t.pantalla[1][j]=0;
}

void verificarLineas(TETRIS &t){

    for(int y=1;y<t.alto-1;y++){

        if(filaCompleta(t,y))
            eliminarFila(t,y);
    }
}

/*bool GameOver(TETRIS &t, PIEZA &p){

    // Verifica si la pieza nueva colisiona al aparecer
    for(int i=0;i<4;i++){

        int x = p.posX + p.bloques[i].x;
        int y = p.posY + p.bloques[i].y;

        if(leerBit(t,y,x))
            return true;
    }

    return false;
}
*/
void nuevaPiezaAleatoria(TETRIS &t,PIEZA &p){

    int tipo=dist(rng);

    p.posX=t.ancho/2;
    p.posY=1;

    switch(tipo){

    case 0:
        p.bloques[0]={-1,0};
        p.bloques[1]={0,0};
        p.bloques[2]={1,0};
        p.bloques[3]={2,0};
        break;

    case 1:
        p.bloques[0]={0,0};
        p.bloques[1]={1,0};
        p.bloques[2]={0,1};
        p.bloques[3]={1,1};
        break;

    case 2:
        p.bloques[0]={-1,0};
        p.bloques[1]={0,0};
        p.bloques[2]={1,0};
        p.bloques[3]={0,1};
        break;

    case 3:
        p.bloques[0]={-1,0};
        p.bloques[1]={0,0};
        p.bloques[2]={1,0};
        p.bloques[3]={1,1};
        break;

    case 4:
        p.bloques[0]={-1,1};
        p.bloques[1]={-1,0};
        p.bloques[2]={0,0};
        p.bloques[3]={1,0};
        break;

    case 5:
        p.bloques[0]={0,0};
        p.bloques[1]={1,0};
        p.bloques[2]={-1,1};
        p.bloques[3]={0,1};
        break;

    case 6:
        p.bloques[0]={-1,0};
        p.bloques[1]={0,0};
        p.bloques[2]={0,1};
        p.bloques[3]={1,1};
        break;
    }
}

bool moverAbajo(TETRIS &t,PIEZA &p){

    borrarPieza(t,p);

    p.posY++;

    if(colision(t,p)){

        p.posY--;

        dibujarPieza(t,p);

        verificarLineas(t);

        nuevaPiezaAleatoria(t,p);

        if(GameOver(t,p)){
            return true;
        }

        dibujarPieza(t,p);

        return false;
    }

    dibujarPieza(t,p);

    return false;
}

