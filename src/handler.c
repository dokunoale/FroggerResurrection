#include "handler.h"

// Returns a random number between min and max
int choose(int min, int max) {
    return rand() % (max - min + 1) + min;
}

// Returns the direction of the flow
int direction(int line, int dir) { return dir ? line % 2 : (line + 1) % 2; }

// Checks if the item is finished in its journey.
int out_of_bounds(Item* item) {
    switch (item->direction) {
        case RIGHT: return item->column >= GAME_WIDTH; break;
        case LEFT:  return item->column + CROCODILE_DIM <= 0; break;
        default:    return 0;
    }
}

// Returns the pointer to the crocodile in the Flow with the same id as the given item.
Item* get_crocodile(Flow* flow, Item* item) {
    for (unsigned int i = 0; i < CROCODILE_MAX_NUM; i++) {
        if (flow->crocodiles[i].id != 0 && flow->crocodiles[i].id == item->id) { return &flow->crocodiles[i]; }
    }
    return NULL;
}

// Updates the crocodile in the Flow with the same id as the given item.
void update_crocodile(Flow* flow, Item* item) {
    Item* crocodile = get_crocodile(flow, item);
    if (crocodile->id != 0) { *crocodile = *item; }
}

// Returns a new array of flows.
Flow* new_flows() {
    Flow* flows = (Flow*)malloc(sizeof(Flow) * NUM_FLOWS);
    int draw = choose(LEFT, RIGHT); 
    for (int i = 0; i < NUM_FLOWS; i++) {
        Item* crocodiles = (Item*)calloc(CROCODILE_MAX_NUM, sizeof(Item));
        Flow flow = (Flow){
            .line = i + DEN_HEIGHT,
            .direction = direction(i, draw),
            .speed = MIN_SPEED + rand() % (MIN_SPEED - MAX_SPEED + 1),
            .how_many_crocodiles = 0,
            .crocodiles = crocodiles
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
        .column = GAME_WIDTH / 2,
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

// Checks if there is any crocodile in the boundaries.
int boundary_check(Flow* flow) {
    for (unsigned int i = 0; i < CROCODILE_MAX_NUM; i++) {
        if (flow->crocodiles[i].id != 0) {
            if (flow->crocodiles[i].direction == RIGHT && flow->crocodiles[i].column <= 1) {
                return 0;
            } else if (flow->crocodiles[i].direction == LEFT && flow->crocodiles[i].column + CROCODILE_DIM >= GAME_WIDTH - 1) {
                return 0;
            }
        }
    }
    return 1;
}

void fill_flow(Buffer* buffer, Flow* flow) {
    if (flow->how_many_crocodiles < CROCODILE_MAX_NUM) {
        // Check if there is a free space for a new crocodile
        for (unsigned int i = 0; i < CROCODILE_MAX_NUM; i++) {
            if (flow->crocodiles[i].id == 0 && boundary_check(flow) && choose(0, ODDS) == 0) {
                flow->crocodiles[i] = new_crocodile(buffer, flow);
                flow->how_many_crocodiles++;
                break;
            }
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

        for (unsigned int i = 0; i < NUM_FLOWS; i++) {
            fill_flow(&buffer, &flows[i]);
        }
        
        switch (receveid.type) {
            case FROG: {
                displayItem(win, &frog_item, &receveid);
                frog_item = receveid;
            } break;
            case CROCODILE: {
                Item* stored = get_crocodile(flow, &receveid);
                displayItem(win, stored, &receveid);
                *stored = receveid;
                if (out_of_bounds(stored)) { 
                    killTask(stored);
                    flow->how_many_crocodiles--;
                }
            } break;                
            case BULLET:
                // TODO
                break;
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