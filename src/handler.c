#include "handler.h"

// Returns a random number between min and max
int choose(int min, int max) { return rand() % (max - min + 1) + min; }

// Returns the direction of the flow
int direction(int line, int dir) { return dir ? line % 2 : (line + 1) % 2; }

// Checks if the item is finished in its journey.
int out_of_bounds(Item* item) {
    switch (item->direction) {
        case RIGHT: return item->column >= GAME_WIDTH; break;
        case LEFT:  return item->column + item->dimension <= 0; break;
        default:    return 0;
    }
}

// Checks if any item is above anotherone.
int is_above(Item* top, Item* bottom) {
    return (top->line == bottom->line) && (top->column >= bottom->column) && (top->column + (int)top->dimension <= bottom->column + (int)bottom->dimension);
}

// Checks if there is an item above an stream of Item (array) and returns the pointer to the item below.
Item* is_above_any(Item* item, Item* stream) {
    unsigned int max;
    switch (stream->type) {
        case CROCODILE: max = CROCODILE_MAX_NUM; break;
        case BULLET: max = BULLET_MAX_NUM; break;
        default: return NULL;
    }
    for (unsigned int i = 0; i < max; i++) {
        if (stream[i].id != 0 && is_above(item, &stream[i])) { return &stream[i]; }
    }
    return NULL;
}

// Returns the pointer to the crocodile in the Flow with the same id as the given item.
Item* get_item(Flow* flow, Item* item) {
    switch (item->type) {
        case CROCODILE: {
            for (unsigned int i = 0; i < CROCODILE_MAX_NUM; i++) {
                if (flow->crocodiles[i].id != 0 && flow->crocodiles[i].id == item->id) { return &flow->crocodiles[i]; }
            }
        } break;
        case BULLET: {
            for (unsigned int i = 0; i < BULLET_MAX_NUM; i++) {
                if (flow->bullets[i].id != 0 && flow->bullets[i].id == item->id) { return &flow->bullets[i]; }
            }
        } break;
        default: return NULL;
    }
    return NULL;
}

// Returns the pointer to a random crocodile in the Flow.
Item* random_crocodile(Flow* flow) {
    Item* crocodiles = (Item*)calloc(CROCODILE_MAX_NUM, sizeof(Item));
    unsigned int availables = 0;
    for (unsigned int i = 0; i < CROCODILE_MAX_NUM; i++) {
        if (flow->crocodiles[i].id != 0) {
            crocodiles[i] = flow->crocodiles[i];
            availables++;
        }
    }
    Item* choose = &crocodiles[rand() % availables];
    free(crocodiles);
    return choose;
}

// Returns a new array of flows.
Flow* new_flows() {
    Flow* flows = (Flow*)malloc(sizeof(Flow) * NUM_FLOWS);
    int draw = choose(LEFT, RIGHT); 
    for (int i = 0; i < NUM_FLOWS; i++) {
        Item* crocodiles = (Item*)calloc(CROCODILE_MAX_NUM, sizeof(Item));
        Item* bullets = (Item*)calloc(BULLET_MAX_NUM, sizeof(Item));
        Flow flow = (Flow){
            .line = i + DEN_HEIGHT,
            .direction = direction(i, draw),
            .speed = MIN_SPEED + rand() % (MIN_SPEED - MAX_SPEED + 1),
            .crocodiles = crocodiles,
            .bullets = bullets
        };
        flows[i] = flow;
    }
    return flows;
}

// Kills all the crocodiles in the game. Deallocates memory.
void free_flows(Flow* flows) {
    for (int i = 0; i < NUM_FLOWS; i++) {
        for (int j = 0; j < CROCODILE_MAX_NUM; j++) {
            if (flows[i].crocodiles[j].id != 0) {
                killTask(&flows[i].crocodiles[j]);
            }
        }
        free(flows[i].crocodiles);
    }
    free(flows);
}

// Returns a new frog.
Item new_frog(Buffer* buffer) {
    Item item = (Item){
        .line = DEN_HEIGHT + NUM_FLOWS,
        .column = (GAME_WIDTH - FROG_DIM) / 2,
        .type = FROG,
        .dimension = FROG_DIM,
        .speed = 0,
        .direction = 0,
        .id = 0
    };
    newTask(buffer, &frog, &item);
    return item;
}

// Returns a new crocodile.
Item new_crocodile(Buffer* buffer, Flow* flow) {
    Item item = (Item){
        .line = flow->line,
        .column = flow->direction == RIGHT ? CROC_LEFT_START : CROC_RIGHT_START,
        .type = CROCODILE,
        .dimension = CROCODILE_DIM,
        .speed = flow->speed,
        .direction = flow->direction,
        .id = 0
    };
    newTask(buffer, &crocodile, &item);
    return item;
}

// Returns a new bullet.
Item new_bullet(Buffer* buffer, Item* source) {
    Item item = (Item){
        .line = source->line,
        .column = source->direction == RIGHT ? source->column + CROCODILE_DIM : source->column - BULLET_DIM,
        .type = BULLET,
        .dimension = BULLET_DIM,
        .speed = BULLET_SPEED,
        .direction = source->direction,
        .id = 0
    };
    newTask(buffer, &bullet, &item);
    return item;
}

// Checks if there is any crocodile in the boundaries. Returns FALSE if there is.
int boundary_check(Flow* flow) {
    for (unsigned int i = 0; i < CROCODILE_MAX_NUM; i++) {
        if (flow->crocodiles[i].id != 0) {
            if (flow->crocodiles[i].direction == RIGHT && flow->crocodiles[i].column <= (CROC_SEP * choose(1, ODDS))) {
                return 0;
            } else if (flow->crocodiles[i].direction == LEFT && flow->crocodiles[i].column + CROCODILE_DIM >= GAME_WIDTH - (CROC_SEP * choose(1, ODDS))) {
                return 0;
            }
        }
    }
    return 1;
}

// Checks if there is space for a new bullet. Returns TRUE if there is.
int bullet_check(Flow* flow) {
    for (unsigned int i = 0; i < BULLET_MAX_NUM; i++) {
        if (flow->bullets[i].id == 0) {
            return 1;
        }
    }
    return 0;
}

// Fills the flow with crocodiles if there is space. TODO: aggiungere randomicità
void fill_flow(Buffer* buffer, Flow* flow) {
    for (unsigned int i = 0; i < CROCODILE_MAX_NUM; i++) {
        if (flow->crocodiles[i].id == 0 && boundary_check(flow)) {
            flow->crocodiles[i] = new_crocodile(buffer, flow);
            break;
        }
    }
}

void shot(Flow* flow, Buffer* buffer, Item* item) {
    for (unsigned int i = 0; i < BULLET_MAX_NUM; i++) {
        if (flow->bullets[i].id == 0) {
            flow->bullets[i] = new_bullet(buffer, item);
            break;
        }
    }
}

/**
 * Manages the game.
 * @note Defined in handler.h
 */
void manche(WINDOW* win) {
    srand(time(NULL));
    Buffer buffer = newBuffer();
    Flow* flows = new_flows();
    Item frog_item = new_frog(&buffer);

    while(1) {
        Item receveid;
        readItem(&buffer, &receveid, MAIN_PIPE);
        Flow* flow = &flows[receveid.line - DEN_HEIGHT];

        // Creates new random crocodiles if there is space on each flow
        for (unsigned int i = 0; i < NUM_FLOWS; i++) { fill_flow(&buffer, &flows[i]); }
        
        switch (receveid.type) {
            case FROG: {
                if (receveid.line <= DEN_HEIGHT || receveid.line >= DEN_HEIGHT + NUM_FLOWS) { 
                    // TODO
                }
                displayItem(win, &frog_item, &receveid);
                frog_item = receveid;
            } break;

            case CROCODILE: {
                Item* stored = get_item(flow, &receveid);
                displayItem(win, stored, &receveid);

                *stored = receveid;
                if (is_above(&frog_item, stored)) { 
                    moveFrog(&buffer, frog_item, flow->direction);
                }
                if (out_of_bounds(&receveid)) { killTask(stored); } 
                else if (bullet_check(flow)) { shot(flow, &buffer, stored); }
            } break;     

            case BULLET: {
                Item* stored = get_item(flow, &receveid);
                displayItem(win, stored, &receveid);

                *stored = receveid;
                if (out_of_bounds(&receveid)) { killTask(stored); }
            } break;

            case GRANADE:
                // TODO
                break;

            case EXIT:
                break;
        }

        if (receveid.type == EXIT) { break; }
    }

    free_flows(flows);
    killTask(&frog_item);
}