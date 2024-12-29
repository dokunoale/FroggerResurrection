#include "utils.h"
#include "struct.h"
#include "handler.h"
#include "entities.h"

int direction(int line, int first) { return first ? line % 2 : (line + 1) % 2; }

int is_flow_full(Flow *flow) { return flow->how_many_crocodiles >= CROCODILE_MAX_NUM; }

int is_edge_free(Flow *flow, int direction) { 
    switch(direction) {
        case LEFT:  return flow->how_many_crocodiles == 0 || flow->crocodiles[flow->how_many_crocodiles - 1].column + CROCODILE_DIM < GAME_WITDH; 
        case RIGHT: return flow->how_many_crocodiles == 0 || flow->crocodiles[flow->how_many_crocodiles - 1].column - CROCODILE_DIM > 0;
        default: return 0;
    }
}

int out_of_bounds(Item item) { 
    switch(item.direction) {
        case LEFT:  return item.column + item.dimension < 0;
        case RIGHT: return item.column + item.dimension > GAME_WITDH;
        default: return 0;
    }
}

int is_frog_above(Item *frog, Item *crocodile) {
    return (frog->line == crocodile->line && 
            frog->column >= crocodile->column &&
            frog->column + frog->dimension <= crocodile->column + crocodile->dimension);
}

void rotate(Flow *flow) { // implements the Flow struct as a queue
    for (int i = 0; i < flow->how_many_crocodiles - 1; i++) {
        flow->crocodiles[i] = flow->crocodiles[i + 1];
    }
    flow->how_many_crocodiles--;
}

/**
 * Initializes a new Game struct.
 * 
 * @note Defined in handler.h
 */
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

/**
 * Generates new crocodiles if all the conditions are met.
 * 
 * @param game The game struct.
 * @param buffer The buffer struct.
 * @note Defined in handler.h
 */
void newCrocodiles(Game *game, Buffer *buffer) {
    for (int i=0; i < NUM_FLOWS; i++) {
        if (!is_flow_full(&game->flows[i] && is_edge_free(&game->flows[i], game->flows[i].direction))) {
            if (choose(1, PROBABILITY) == 1) {
                game->flows[i].crocodiles[game->flows[i].how_many_crocodiles] = (Item){
                    .line = i,
                    .column = GAME_WITDH - 1,
                    .type = CROCODILE,
                    .dimension = CROCODILE_DIM,
                    .speed = game->flows[i].speed,
                    .direction = game->flows[i].direction,
                    .id = NULL
                };
                game->flows[i].how_many_crocodiles++;
                newTask(buffer, &crocodile, &game->flows[i].crocodiles[game->flows[i].how_many_crocodiles - 1]);
            }
        }
    }
}

/**
 * Manages the game.
 * Checks for collisions and out of bounds crocodiles.
 * Updates the screen accordingly.
 * 
 * @note Defined in handler.h
 */
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
                displayFrog(/* TODO */);
                break;
            case CROCODILE:
                displayCrocodile(/* TODO */);
                if (out_of_bounds(item)) { 
                    // TODO: implement other functions
                    killTask(&buffer, &item);
                    rotate(&game.flows[item.line]);
                }
                if (is_frog_above(&game.Frog, &item)) {
                    move_frog(&game.Frog, item.direction);
                }
                break;
        }
    }
}





