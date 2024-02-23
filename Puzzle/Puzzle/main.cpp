#include "Game.h"
#include <iostream>

Game* game = NULL;
const int WINDOW_WIDTH = 1200;
const int WINDOW_HEIGHT = 600;

int main(int argc, char* argv[]) {

    game = new Game(); // create
    game->init("Puzzle", // initialize
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT,
        SDL_WINDOW_RESIZABLE);
    cout << "----------------------------------------------------\n";
    cout << "Click with Left Mouse Button to show/hide images\n";
 
    cout << "----------------------------------------------------\n";
    while (game->isRunning()) { // cycle
        game->handleEvents(); // listen
        game->update(); // update (Game members?)
        game->render(); // draw
    }

    game->clean(); // destroy

    return 0;
}