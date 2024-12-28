#include "processes.h"
#include "struct.h"
#include "handler.h"

int direction(int line, int first) { return first ? line % 2 : (line + 1) % 2; }

Game newGame() {
    Game game;
    game.Frog = (Item){10, 7, FROG, FROG_DIM};
    int draw = choose(LEFT, RIGHT); 

    for (int i=0; i < NUM_FLOWS; i++) {
        game.flows[i].id = i; 
        game.flows[i].direction = direction(i, draw); 
        game.flows[i].speed = choose(MIN_SPEED, MAX_SPEED);
        game.flows[i].num_croc_in_flow = 0; 
    }

    return game;
}

void manche() {
    Buffer buffer = newBuffer();
    Game game = newGame();
    newTask(&buffer, flow, (Item*)&game);

}





