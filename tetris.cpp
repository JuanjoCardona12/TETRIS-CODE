#include "tetris.h"
#include <iostream>
#include <random>
#include "pieza.h"
#include "gameover.h"

using namespace std;

mt19937 rng(random_device{}());
uniform_int_distribution<int> dist(0,6);

void crearTetris(TETRIS &t){
    cout << "Ingrese ancho: ";
    cin >> t.ancho;
    //se ajusta para la resolucion 1200p
    while(t.ancho < 8 || t.ancho % 8 != 0 || t.ancho > 144){
        cout << "Ancho invalido (multiplo de 8): ";
        cin >> t.ancho;
    }

    cout << "Ingrese alto: ";
    cin >> t.alto;
    while(t.alto < 8 || t.alto > 40){
        cout << "Alto invalido (8-40): ";
        cin >> t.alto;
    }

    t.bytesPorFila = t.ancho / 8;
    t.pantalla = new unsigned char*[t.alto];
    for(int i = 0; i < t.alto; i++)
        t.pantalla[i] = new unsigned char[t.bytesPorFila];
}

void inicializarTetris(TETRIS &t){
    for(int i = 0; i < t.alto; i++)
        for(int j = 0; j < t.bytesPorFila; j++)
            t.pantalla[i][j] = 0;
}

void encenderBit(TETRIS &t, int y, int x){
    t.pantalla[y][x/8] |= (1 << (7 - (x % 8)));
}

void apagarBit(TETRIS &t, int y, int x){
    t.pantalla[y][x/8] &= ~(1 << (7 - (x % 8)));
}

bool leerBit(TETRIS &t, int y, int x){
    if (y < 0 || y >= t.alto || x < 0 || x >= t.ancho) return true;
    return t.pantalla[y][x/8] & (1 << (7 - (x % 8)));
}

void crearBordes(TETRIS &t){
    for(int x = 0; x < t.ancho; x++){
        encenderBit(t, 0, x);
        encenderBit(t, t.alto - 1, x);
    }
    for(int y = 0; y < t.alto; y++){
        encenderBit(t, y, 0);
        encenderBit(t, y, t.ancho - 1);
    }
}

void dibujarTetris(TETRIS &t){
    for(int i = 0; i < t.alto; i++){
        for(int j = 0; j < t.ancho; j++){
            if(leerBit(t, i, j)) cout << "#";
            else cout << ".";
        }
        cout << endl;
    }
}

void liberarTetris(TETRIS &t){
    for(int i = 0; i < t.alto; i++)
        delete[] t.pantalla[i];
    delete[] t.pantalla;
}

void dibujarPieza(TETRIS &t, PIEZA &p){
    for(int i = 0; i < 4; i++)
        encenderBit(t, p.posY + p.bloques[i].y, p.posX + p.bloques[i].x);
}

void borrarPieza(TETRIS &t, PIEZA &p){
    for(int i = 0; i < 4; i++)
        apagarBit(t, p.posY + p.bloques[i].y, p.posX + p.bloques[i].x);
}

bool colision(TETRIS &t, PIEZA &p){
    for(int i = 0; i < 4; i++){
        if(leerBit(t, p.posY + p.bloques[i].y, p.posX + p.bloques[i].x))
            return true;
    }
    return false;
}

void moverIzquierda(TETRIS &t, PIEZA &p){
    borrarPieza(t, p);
    p.posX--;
    if(colision(t, p)) p.posX++;
    dibujarPieza(t, p);
}

void moverDerecha(TETRIS &t, PIEZA &p){
    borrarPieza(t, p);
    p.posX++;
    if(colision(t, p)) p.posX--;
    dibujarPieza(t, p);
}

void rotarDerecha(TETRIS &t, PIEZA &p) {
    borrarPieza(t, p);
    BLOQUE nuevos[4];
    bool posible = true;

    for (int i = 0; i < 4; i++) {
        int nx = p.bloques[i].y;
        int ny = -p.bloques[i].x;
        if (leerBit(t, p.posY + ny, p.posX + nx)) {
            posible = false;
            break;
        }
        nuevos[i] = {nx, ny};
    }

    if (posible) {
        for (int i = 0; i < 4; i++) p.bloques[i] = nuevos[i];
    }
    dibujarPieza(t, p);
}

bool filaCompleta(TETRIS &t, int y){
    for(int x = 1; x < t.ancho - 1; x++){
        if(!leerBit(t, y, x)) return false;
    }
    return true;
}

void eliminarFila(TETRIS &t, int y) {
    for (int i = y; i > 1; i--) {
        for (int j = 0; j < t.bytesPorFila; j++)
            t.pantalla[i][j] = t.pantalla[i - 1][j];
    }
    for (int j = 0; j < t.bytesPorFila; j++) t.pantalla[1][j] = 0;
    crearBordes(t); 
}

void verificarLineas(TETRIS &t){
    for(int y = 1; y < t.alto - 1; y++){
        if(filaCompleta(t, y)) eliminarFila(t, y);
    }
}

void nuevaPiezaAleatoria(TETRIS &t, PIEZA &p){
    int tipo = dist(rng);
    p.posX = t.ancho / 2;
    p.posY = 1;
    switch(tipo){
        case 0: p.bloques[0]={-1,0}; p.bloques[1]={0,0}; p.bloques[2]={1,0}; p.bloques[3]={2,0}; break;
        case 1: p.bloques[0]={0,0}; p.bloques[1]={1,0}; p.bloques[2]={0,1}; p.bloques[3]={1,1}; break;
        case 2: p.bloques[0]={-1,0}; p.bloques[1]={0,0}; p.bloques[2]={1,0}; p.bloques[3]={0,1}; break;
        case 3: p.bloques[0]={-1,0}; p.bloques[1]={0,0}; p.bloques[2]={1,0}; p.bloques[3]={1,1}; break;
        case 4: p.bloques[0]={-1,1}; p.bloques[1]={-1,0}; p.bloques[2]={0,0}; p.bloques[3]={1,0}; break;
        case 5: p.bloques[0]={0,0}; p.bloques[1]={1,0}; p.bloques[2]={-1,1}; p.bloques[3]={0,1}; break;
        case 6: p.bloques[0]={-1,0}; p.bloques[1]={0,0}; p.bloques[2]={0,1}; p.bloques[3]={1,1}; break;
    }
}

bool moverAbajo(TETRIS &t, PIEZA &p){
    borrarPieza(t, p);
    p.posY++;
    if(colision(t, p)){
        p.posY--;
        dibujarPieza(t, p);
        verificarLineas(t);
        nuevaPiezaAleatoria(t, p);
        if(GameOver(t, p)) return true;
        dibujarPieza(t, p);
        return false;
    }
    dibujarPieza(t, p);
    return false;
}

