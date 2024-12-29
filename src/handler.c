#include "utils.h"
#include "struct.h"
#include "handler.h"
#include "display.h"
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

// Checks if the frog is above a specific crocodile
int is_frog_above(Item *frog, Item *crocodile) {
    return (frog->line == crocodile->line && 
            frog->column >= crocodile->column &&
            frog->column + frog->dimension <= crocodile->column + crocodile->dimension);
}

// Checks if the frog is drawned in a specific flow
int is_frog_drawned(Item *frog, Flow *flow) { 
    for (int i = 0; i < flow->how_many_crocodiles; i++) {
        if (is_frog_above(frog, &flow->crocodiles[i])) {
            return 0;
        }
    }
    return 1;
}

Item *get_crocodile(Flow *flow, Item *new) {
    for (int i = 0; i < flow->how_many_crocodiles; i++) {
        if (flow->crocodiles[i].id == new->id) {
            return &flow->crocodiles[i];
        }
    }
    return NULL;
}

// Move the frog in a specific direction
void drag_frog(Item *frog, int direction) {
    switch(direction) {
        case LEFT:  if(frog->column > 0) frog->column--; break;
        case RIGHT: if(frog->column + frog->dimension < GAME_WITDH) frog->column++; break;
    }
}

// implements the Flow struct as a queue
void rotate(Flow *flow) { 
    for (int i = 0; i < flow->how_many_crocodiles - 1; i++) {
        flow->crocodiles[i] = flow->crocodiles[i + 1];
    }
    flow->how_many_crocodiles--;
}

// Initializes a new manche
Game new_manche() {
    Game game;
    game.Frog = (Item){
        .line = GAME_HEIGHT - 1,
        .column = GAME_WITDH / 2,
        .type = FROG,
        .dimension = FROG_DIM,
        .speed = 0,
        .direction = 0,
        .id = NULL
    };
    
    int draw = choose(LEFT, RIGHT); 
    for (int i=0; i < NUM_FLOWS; i++) {
        game.flows[i].line = i; 
        game.flows[i].direction = direction(i, draw); 
        game.flows[i].speed = choose(MIN_SPEED, MAX_SPEED);
        game.flows[i].how_many_crocodiles = 0; 
    }

    return game;
}

// Ends the manche by killing all the processes
void end_manche(Game *game, Buffer *buffer) {
    for (int i=0; i < NUM_FLOWS; i++) {
        for (int j=0; j < game->flows[i].how_many_crocodiles; j++) {
            killTask(&buffer, &game->flows[i].crocodiles[j]);
        }
    }
    killTask(&buffer, &game->Frog);
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
void manche(WINDOW *game_win) {
    Buffer buffer = newBuffer();
    Game game = new_manche();
    newTask(&buffer, &frog, &game.Frog);

    while (1) {
        Item item;
        readItem(&buffer, &item);
        newCrocodiles(&game, &buffer);

        switch (item.type) {
            case FROG:
                if (is_frog_drawned(&item, &game.flows[item.line])) {
                    end_manche(&game, &buffer);
                    // TODO: implement other functions
                    return;
                } else {
                    displayItem(game_win, &game.Frog, &item);
                    game.Frog = item;
                }
                break;
            case CROCODILE:
                displayItem(game_win, get_crocodile(&game.flows[item.line], &item), &item);
                if (out_of_bounds(item)) { 
                    killTask(&buffer, &item);
                    rotate(&game.flows[item.line]);
                }
                if (is_frog_above(&game.Frog, &item)) {
                    drag_frog(&game.Frog, item.direction);
                    displayItem(game_win, &game.Frog, &item);
                }
                break;
        }
    }
}





