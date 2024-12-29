#include "utils.h"
#include "struct.h"
#include "handler.h"
#include "entities.h"

int direction(int line, int first) { return first ? line % 2 : (line + 1) % 2; }

Game newGame() {
    Game game;
    game.Frog = (Item){10, 7, FROG, FROG_DIM};
    int draw = choose(LEFT, RIGHT); 

    for (int i=0; i < NUM_FLOWS; i++) {
        game.flows[i].line = i; 
        game.flows[i].direction = direction(i, draw); 
        game.flows[i].speed = choose(MIN_SPEED, MAX_SPEED);
        game.flows[i].how_many_crocodiles = 0; 
    }

    return game;
}

void newCrocodile(Game *game, Buffer *buffer, int direction) {
    // todo: implement the newCrocodile function
}

void newCrocodiles(Game *game, Buffer *buffer) {
    for (int i=0; i < NUM_FLOWS; i++) { // for each flow
        if (game->flows[i].how_many_crocodiles < CROCODILE_MAX_NUM) {
            Item *crocodiles = game->flows[i].crocodiles;
            Item last_crocodile = crocodiles[game->flows[i].how_many_crocodiles - 1];

            switch(game->flows[i].direction) {
                case LEFT:
                    if (last_crocodile.column + CROCODILE_DIM < GAME_WITDH && choose(1, PROBABILITY) == 1) {
                        crocodiles[game->flows[i].how_many_crocodiles] = (Item){
                            .line = i,
                            .column = GAME_WITDH - 1,
                            .type = CROCODILE,
                            .dimension = CROCODILE_DIM,
                            .speed = game->flows[i].speed,
                            .direction = LEFT,
                            .id = NULL
                        };
                        game->flows[i].how_many_crocodiles++;
                        newTask(buffer, &crocodile, &crocodiles[game->flows[i].how_many_crocodiles - 1]);
                    }
                    break;
                case RIGHT:
                    // todo: implement the right direction
                    break;
            }
        }
    }
}

// implements the Flow struct as a queue
void rotate(Flow *flow) {
    for (int i = 0; i < flow->croc_in_flow - 1; i++) {
        flow->crocodiles[i] = flow->crocodiles[i + 1];
    }
    flow->croc_in_flow--;
}

void manche() {
    Buffer buffer = newBuffer();
    Game game = newGame();
    newTask(&buffer, &frog, &game.Frog);

    while (1) {
        Item item;
        readItem(&buffer, &item);
        newCrocodiles(&game, &buffer);

        switch (item.type) {
            case FROG:
                break;
            case CROCODILE:
                if out_of_bounds(item) { 
                    // TODO: implement the out_of_bounds function
                    // remove the crocodile
                    killTask(&buffer, &item);
                    rotate(&game.flows[item.line]);
                }
                break;
        }
    }
}





