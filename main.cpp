#include "tetris.h"
#include "pieza.h"
#include <iostream>

using namespace std;

int main() {
    TETRIS t;
    PIEZA p;

    crearTetris(t);
    inicializarTetris(t);
    crearBordes(t);
    nuevaPiezaAleatoria(t, p);

    char tecla;
    bool finDelJuego = false;

    while (!finDelJuego) {
        dibujarTetris(t);
        cout << "Accion: [A]Izq [D]Der [S]Bajar [W]Rotar [Q]Salir: ";
        cin >> tecla;

        switch (tecla) {
            case 'a': moverIzquierda(t, p); break;
            case 'd': moverDerecha(t, p); break;
            case 's': finDelJuego = moverAbajo(t, p); break;
            case 'w': rotarDerecha(t, p); break;
            case 'q': finDelJuego = true; break;
        }
    }

    cout << "--- GAME OVER ---" << endl;
    liberarTetris(t); // Limpieza de memoria dinámica
    return 0;
}
