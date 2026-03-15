#include "tetris.h"

#include <iostream>
using namespace std;
void crearTetris(TETRIS &t){

    cout<<"Ingrese ancho: ";
    cin>>t.ancho;
//validaciones para la dimension de tablero
    while(t.ancho < 8 || t.ancho % 8 != 0){
        cout<<"El ancho debe ser multiplo de 8: ";
        cin>>t.ancho;
    }

    cout<<"Ingrese alto: ";
    cin>>t.alto;

    while(t.alto < 8){
        cout<<"El alto minimo es 8: ";
        cin>>t.alto;
    }

    t.bytesPorFila = t.ancho / 8;// guarda la cantidad de bytes (la celdas del tabelro)

    t.pantalla = new unsigned char*[t.alto];

    for(int i=0;i<t.alto;i++)
        t.pantalla[i] = new unsigned char[t.bytesPorFila];//RESERVA EL TAMAÑO PARA LAS FILAS
}

void inicializarTetris(TETRIS &t){

    for(int i=0;i<t.alto;i++)//RECORRO LA COLUMNA
        for(int j=0;j<t.bytesPorFila;j++)//BYTES(CELDAS DEL TABLERO)
            t.pantalla[i][j] = 0;//INICIALIZADO EN 0 PARA PUES REFERIR QUE ES UN ESPACIO VACIO DEL TAB
}

void encenderBit(TETRIS &t,int y,int x){
    t.pantalla[y][x/8] |= (1 << (7-(x%8)));
}

void apagarBit(TETRIS &t,int y,int x){
    t.pantalla[y][x/8] &= ~(1 << (7-(x%8)));
}// Cada byte tiene 8 bits (posiciones 7..0).
 // x/8 -> qué byte contiene la columna x
// x%8 -> posición dentro del byte
// 7-(x%8) -> convierte esa posición al orden real del bit

bool leerBit(TETRIS &t,int y,int x){
    return t.pantalla[y][x/8] & (1 << (7-(x%8)));
}
// Verifica si el bit correspondiente a la posición (y,x) está encendido.
// Si el bit es 1 devuelve true (bloque ocupado), si es 0 devuelve false.

void crearBordes(TETRIS &t){

    for(int x=0;x<t.ancho;x++){
        encenderBit(t,0,x);
        encenderBit(t,t.alto-1,x);
    }

    for(int y=0;y<t.alto;y++){
        encenderBit(t,y,0);
        encenderBit(t,y,t.ancho-1);
    }
}//aqui si es breve no mas lo que hice fue qu ese encienda los bits en lo ancho y largo  y uso t.alto-1,t.ancho-1 pq no existe la fila/columna  8

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
}//loco solo itera y rellena si el bloque esta o no cupado no hay mucha ciencia

void liberarTetris(TETRIS &t){

    for(int i=0;i<t.alto;i++)
        delete[] t.pantalla[i];

    delete[] t.pantalla;
}//libera lA amemoria
