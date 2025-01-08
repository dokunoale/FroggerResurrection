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
        case LEFT:  return item->column + item->dimension <= 0; break;
        default:    return 0;
    }
}

int is_frog_above(Item* frog, Item* crocodile) {
    return (frog->line == crocodile->line) && (frog->column >= crocodile->column) && (frog->column <= crocodile->column + (int)crocodile->dimension);
}

int is_croc_below(Item* frog, Flow* flow) {
    for (unsigned int i = 0; i < CROCODILE_MAX_NUM; i++) {
        if (flow->crocodiles[i].id != 0 && is_frog_above(frog, &flow->crocodiles[i])) { return 1; }
    }
    return 0;
}

// Returns the pointer to the crocodile in the Flow with the same id as the given item.
Item* get_crocodile(Flow* flow, Item* item) {
    for (unsigned int i = 0; i < CROCODILE_MAX_NUM; i++) {
        if (flow->crocodiles[i].id != 0 && flow->crocodiles[i].id == item->id) { return &flow->crocodiles[i]; }
    }
    return NULL;
}

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
void new_bullet(Buffer* buffer, Item* source) {
    Item item = (Item){
        .line = source->line,
        .column = source->direction == RIGHT ? source->column + CROCODILE_DIM : source->column - BULLET_DIM,
        .type = BULLET,
        .dimension = BULLET_DIM,
        .speed = source->speed,
        .direction = source->direction,
        .id = 0
    };
    newTask(buffer, &bullet, &item);
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
                if (receveid.line <= DEN_HEIGHT) { displayItem(win, &frog_item, &receveid, WATER_COLOR); }
                else if (receveid.line == DEN_HEIGHT + NUM_FLOWS) { displayItem(win, &frog_item, &receveid, WATER_COLOR); }
                else if (is_croc_below(&frog_item, flow)) { displayItem(win, &frog_item, &receveid, CROCODILE_COLOR); }
                else { displayItem(win, &frog_item, &receveid, WATER_COLOR); }
                frog_item = receveid;
            } break;

            case CROCODILE: {
                Item* stored = get_crocodile(flow, &receveid);
                displayItem(win, stored, &receveid, WATER_COLOR);
                if (is_frog_above(&frog_item, stored)) { displayItem(win, &frog_item, &frog_item, CROCODILE_COLOR); }

                *stored = receveid;
                if (out_of_bounds(stored)) { killTask(stored); } 
                else if (bullet_check(flow)) { new_bullet(&buffer, stored); }
            } break;     

            case BULLET:
                if (out_of_bounds(&receveid)) { killTask(&receveid); }
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