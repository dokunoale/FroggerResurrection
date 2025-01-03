#include "handler.h"
#define ITEM_INDEX (item.line -2)

int choose(int min, int max) { return rand() % (max - min + 1) + min; }

int direction(int line, int dir) { return dir ? line % 2 : (line + 1) % 2; }

int is_flow_full(Flow *flow) { return flow->how_many_crocodiles >= CROCODILE_MAX_NUM; }

int is_edge_free(Flow *flow, unsigned int direction) { 
    switch(direction) {
        case LEFT:  return flow->how_many_crocodiles == 0 || flow->crocodiles[flow->how_many_crocodiles - 1].column + CROCODILE_DIM < GAME_WITDH - 1; 
        case RIGHT: return flow->how_many_crocodiles == 0 || flow->crocodiles[flow->how_many_crocodiles - 1].column - CROCODILE_DIM > 0;
        default: return 0;
    }
}

int out_of_bounds(Item item) { 
    switch(item.direction) {
        case LEFT:  return item.column + item.dimension < 0;
        case RIGHT: return item.column > GAME_WITDH - 1;
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
    for (unsigned int i = 0; i < flow->how_many_crocodiles; i++) {
        if (is_frog_above(frog, &flow->crocodiles[i])) {
            return 0;
        }
    }
    return 1;
}

Item *get_crocodile(Flow *flow, Item *new) {
    for (unsigned int i = 0; i < flow->how_many_crocodiles; i++) {
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

void update_crocodile(Flow *flow, Item *crocodile) {
    for (unsigned int i=0; i<flow->how_many_crocodiles - 1; i++) {
        if (flow->crocodiles[i].id == crocodile->id) {
            flow->crocodiles[i] = *crocodile;
            return;
        }
    }
}

// implements the Flow struct as a queue
void rotate(Flow *flow) { 
    for (unsigned int i = 0; i < flow->how_many_crocodiles - 1; i++) {
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
        .id = 0
    };

    int draw = choose(LEFT, RIGHT); 
    for (int i=0; i < NUM_FLOWS; i++) {
        game.flows[i].line = i + 2; 
        game.flows[i].direction = direction(i, draw); 
        game.flows[i].speed = choose(MIN_SPEED, MAX_SPEED);
        game.flows[i].how_many_crocodiles = 0; 
    }

    return game;
}

// Ends the manche by killing all the processes
void end_manche(Game *game, Buffer *buffer) {
    for (unsigned int i=0; i < NUM_FLOWS; i++) {
        for (unsigned int j=0; j < game->flows[i].how_many_crocodiles; j++) {
            killTask(buffer, &game->flows[i].crocodiles[j]);
        }
    }
    killTask(buffer, &game->Frog);
}

/**
 * Generates new crocodiles if all the conditions are met.
 * 
 * @param game The game struct.
 * @param buffer The buffer struct.
 * @note Defined in handler.h
 */
void newCrocodiles(Game *game, Buffer *buffer) {
    // for each flow
    for (unsigned int i=0; i < NUM_FLOWS; i++) {
        if (!is_flow_full(&game->flows[i]) && is_edge_free(&game->flows[i], game->flows[i].direction)) {
            if (1) { // choose(1, PROBABILITY) == 1 // TODO: implement probability
                Item new_croc = (Item){
                    .line = i + 2,
                    .column = game->flows[i].direction ? 5 : 6, // -CROCODILE_DIM + 1 : GAME_WITDH - 2, 
                    .type = CROCODILE,
                    .dimension = CROCODILE_DIM,
                    .speed = game->flows[i].speed,
                    .direction = game->flows[i].direction,
                    .id = 0
                };
                game->flows[i].crocodiles[game->flows[i].how_many_crocodiles] = new_croc;
                game->flows[i].how_many_crocodiles++;
                newTask(buffer, &crocodile, &new_croc);
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

    wrefresh(game_win);

    Buffer buffer = newBuffer();
    Game game = new_manche();
    newTask(&buffer, &frog, &game.Frog);

    Item item;
    // newCrocodiles(&game, &buffer);

    Item tempitem = (Item){
            .line = 3,
            .column = 2,
            .type = CROCODILE,
            .dimension = CROCODILE_DIM,
            .speed = 1,
            .direction = RIGHT,
            .id = 0
        };
    newTask(&buffer, &crocodile, &tempitem);

    while (TRUE) {
        wrefresh(game_win);

        readItem(&buffer, &item, MAIN_PIPE);

        mvwprintw(game_win, 0, 0, "read -> %d", item.type);
        wrefresh(game_win);

        switch (item.type) {
            case EXIT:
                end_manche(&game, &buffer);
                return;
            case FROG:
                displayItem(game_win, &game.Frog, &item);
                game.Frog = item;

                if (item.line != GAME_HEIGHT - 1) {
                    if (is_frog_drawned(&item, &game.flows[ITEM_INDEX])) {
                        end_manche(&game, &buffer);
                        // TODO: implement other functions
                        return;
                    }
                }
                break;
            case CROCODILE:
                mvwprintw(game_win, 1, 0, "read croc -> %d", item.type);
                wrefresh(game_win);

                update_crocodile(&game.flows[ITEM_INDEX], &item);
                displayItem(game_win, get_crocodile(&game.flows[ITEM_INDEX], &item), &item);

                if (out_of_bounds(item)) { 
                    killTask(&buffer, &item);
                    rotate(&game.flows[ITEM_INDEX]);
                }
                if (is_frog_above(&game.Frog, &item) && 0) {
                    drag_frog(&game.Frog, item.direction);
                    writeItem(&buffer, &game.Frog, REVERSE_PIPE);
                    displayItem(game_win, &game.Frog, &item);
                }
                break;
        }
    }
}