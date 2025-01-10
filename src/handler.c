#include "utils.h"
#include "display.h"
#include "entities.h"
#include "handler.h"

// Returns a random number between min and max
int choose(int min, int max) { return rand() % (max - min + 1) + min; }

// Returns the direction of the flow
int direction(int line, int dir) { return dir ? line % 2 : (line + 1) % 2; }

// Checks if the item is finished in its journey.
int out_of_bounds(Item* item) {
    switch (item->direction) {
        case RIGHT: return item->column >= GAME_WIDTH; break;
        case LEFT:  return item->column + (int)item->dimension <= 0; break;
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
        case DEN: max = 5; break;
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
        case GRANADE: {
            for (unsigned int i = 0; i < GRANADE_MAX_NUM; i++) {
                if (flow->granades[i].id != 0 && flow->granades[i].id == item->id) { return &flow->granades[i]; }
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
        Item* granades = (Item*)calloc(GRANADE_MAX_NUM, sizeof(Item));
        Flow flow = (Flow){
            .line = i + DEN_HEIGHT,
            .direction = direction(i, draw),
            .speed = MIN_SPEED + rand() % (MIN_SPEED - MAX_SPEED + 1),
            .crocodiles = crocodiles,
            .bullets = bullets,
            .granades = granades
        };
        flows[i] = flow;
    }
    return flows;
}

// Kills all the crocodiles in the game. Deallocates memory.
void free_flows(Flow* flows) {
    for (int i = 0; i < NUM_FLOWS; i++) {
        for (int j = 0; j < CROCODILE_MAX_NUM; j++) {
            if (flows[i].crocodiles[j].id != 0) { killTask(&flows[i].crocodiles[j]); }
        }
        for (int j = 0; j < BULLET_MAX_NUM; j++) {
            if (flows[i].bullets[j].id != 0) { killTask(&flows[i].bullets[j]); }
        }
        for (int j = 0; j < GRANADE_MAX_NUM; j++) {
            if (flows[i].granades[j].id != 0) { killTask(&flows[i].granades[j]); }
        }
        free(flows[i].crocodiles);
        free(flows[i].bullets);
        free(flows[i].granades);
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

Item new_granade(Buffer* buffer, Item* source, int direction) {
    Item item = (Item){
        .line = source->line,
        .column = direction == RIGHT ? source->column + CROCODILE_DIM : source->column - BULLET_DIM,
        .type = GRANADE,
        .dimension = GRANADE_DIM,
        .speed = GRANADE_SPEED,
        .direction = direction,
        .id = 0
    };
    newTask(buffer, &bullet, &item);
    return item;
}

Item new_den(int index) {
    Item item = (Item){
        .line = 0,
        .column = ((index * 3) - 2) * WIN_WIDTH_RATIO,
        .type = DEN,
        .dimension = FROG_DIM,
        .speed = 0,
        .direction = 0,
        .id = 1
    };
    return item;
}

Item new_heart(int index) {
    Item item = (Item){
        .line = 0,
        .column = index * WIN_WIDTH_RATIO,
        .type = HEART,
        .dimension = HEART_DIM,
        .speed = 0,
        .direction = 0,
        .id = 1
    };
    return item;
}

Item new_timer(Buffer* buffer) {
    Item item = (Item){
        .line = 0,
        .column = TIMER_START,
        .type = TIMER,
        .dimension = 1,
        .speed = TIMER_SPEED,
        .direction = LEFT,
        .id = 0
    };
    newTask(buffer, &timer, &item);
    return item;
}

// Checks if there is any crocodile in the boundaries. Returns FALSE if there is.
int boundary_check(Flow* flow, int odds) {
    for (unsigned int i = 0; i < CROCODILE_MAX_NUM; i++) {
        if (flow->crocodiles[i].id != 0) {
            if (flow->crocodiles[i].direction == RIGHT && flow->crocodiles[i].column <= (CROC_SEP * choose(1, odds))) {
                return 0;
            } else if (flow->crocodiles[i].direction == LEFT && flow->crocodiles[i].column + CROCODILE_DIM >= GAME_WIDTH - (CROC_SEP * choose(1, odds))) {
                return 0;
            }
        }
    }
    return 1;
}

// Fills the flow with crocodiles if there is space. TODO: aggiungere randomicità
void fill_flow(Buffer* buffer, Flow* flow) {
    for (unsigned int i = 0; i < CROCODILE_MAX_NUM; i++) {
        if (flow->crocodiles[i].id == 0 && boundary_check(flow, CROCODILE_ODDS)) {
            flow->crocodiles[i] = new_crocodile(buffer, flow);
            break;
        }
    }
}

void crocodile_shot(Flow* flow, Buffer* buffer, Item* item) {
    for (unsigned int i = 0; i < BULLET_MAX_NUM; i++) {
        if (flow->bullets[i].id == 0 && boundary_check(flow, BULLET_ODDS)) {
            flow->bullets[i] = new_bullet(buffer, item);
            break;
        }
    }
}

void frog_shot(Flow* flow, Buffer* buffer, Item* item) {
    for (unsigned int i = 0; i < GRANADE_MAX_NUM; i++) {
        if (flow->granades[i].id != 0) { return; }
    }
    flow->granades[0] = new_granade(buffer, item, RIGHT);
    flow->granades[1] = new_granade(buffer, item, LEFT);
}

/**
 * Manages the game.
 * @note Defined in handler.h
 */
int manche(WINDOW* win, WINDOW* timer_win, Item* den) {
    srand(time(NULL));
    Buffer buffer = newBuffer();
    Flow* flows = new_flows();
    
    for (int i = 0; i < DEN_NUM; i++) { if (den[i].id != 0) displayItem(win, &den[i], &den[i]); }
    Item frog_item = new_frog(&buffer);
    new_timer(&buffer); displayTimer(timer_win);

    int exit_status = FROG;
    while(1) {
        Item receveid;
        readItem(&buffer, &receveid, MAIN_PIPE);
        Flow* flow = &flows[receveid.line - DEN_HEIGHT];

        // Creates new random crocodiles if there is space on each flow
        for (unsigned int i = 0; i < NUM_FLOWS; i++) { fill_flow(&buffer, &flows[i]); }
        
        switch (receveid.type) {
            case FROG: {
                if (receveid.line == 0) { 
                    Item* reached = is_above_any(&receveid, den);
                    if (reached == NULL) { exit_status = LOSE; break; }
                    else { reached->id = 0; exit_status = WIN; displayItem(win, &frog_item, &receveid); break; }
                }
                if (receveid.line >= DEN_HEIGHT + NUM_FLOWS ||receveid.line <= DEN_HEIGHT) { /* do nothing */ }
                else if (is_above_any(&receveid, flow->crocodiles) == NULL) { exit_status = LOSE; break; }

                displayItem(win, &frog_item, &receveid);
                frog_item = receveid;
            } break;

            case CROCODILE: {
                Item* stored = get_item(flow, &receveid);
                displayItem(win, stored, &receveid);

                *stored = receveid;
                crocodile_shot(flow, &buffer, stored);

                if (is_above(&frog_item, stored)) { moveFrog(&buffer, frog_item, flow->direction); }
                if (out_of_bounds(&receveid)) { killTask(stored); } 
            } break;     

            case BULLET: {
                Item* stored = get_item(flow, &receveid);
                displayItem(win, stored, &receveid);

                if (is_above(&receveid, &frog_item)) { exit_status = LOSE; break; }

                *stored = receveid;
                if (out_of_bounds(&receveid)) { killTask(stored); }
            } break;

            case SHOT: {
                if (frog_item.line < DEN_HEIGHT || frog_item.line >= DEN_HEIGHT + NUM_FLOWS) { break; }
                frog_shot(flow, &buffer, &frog_item);
            } break;

            case GRANADE: {
                Item* stored = get_item(flow, &receveid);
                Item* collided = is_above_any(&receveid, flow->bullets);
                displayItem(win, stored, &receveid);

                *stored = receveid;
                if (collided != NULL) { killTask(collided); killTask(stored); break; }
                if (out_of_bounds(&receveid)) { killTask(stored); }
            } break;

            case TIMER: { displayItem(timer_win, &receveid, &receveid); } break;
            case LOSE: exit_status = LOSE; break;
            case EXIT: exit_status = EXIT; break;
        }

        if (exit_status == EXIT || exit_status == WIN || exit_status == LOSE) { break; }
    }

    free_flows(flows);
    killTask(&frog_item);
    return exit_status;
}

void game() {
    WINDOW* timer_win = newwin(WIN_TIMER_HEIGHT, WIN_TIMER_WIDTH, 1, 1);
    WINDOW* game_win = newwin(WIN_GAME_HEIGHT, WIN_GAME_WIDTH, WIN_TIMER_HEIGHT + 1, 1); 
    WINDOW* info_win = newwin(WIN_INFO_HEIGHT, WIN_INFO_WIDTH, WIN_GAME_HEIGHT + WIN_TIMER_HEIGHT + 1, 1);
    wrefresh(game_win); wbkgd(game_win, COLOR_PAIR(WATER_COLOR)); wrefresh(game_win);

    Item den[DEN_NUM] = { new_den(1), new_den(2), new_den(3), new_den(4), new_den(5) };
    Item hearts[LIFES] = { new_heart(1), new_heart(2), new_heart(3), new_heart(4), new_heart(5) };

    int lifes = LIFES;
    int reached = 0;
    int score = 0;

    while(lifes > 0 && reached < DEN_NUM) {
        wclear(game_win); wclear(info_win); wrefresh(game_win); wrefresh(info_win);
        for (int i = 0; i < lifes; i++) { displayItem(info_win, &hearts[i], &hearts[i]); }
        displayScore(info_win, score);

        time_t start_time = time(NULL); 
        int status = manche(game_win, timer_win, den);
        time_t end_time = time(NULL); 

        int time_spent = (int)difftime(end_time, start_time);
        
        switch (status) {
            case EXIT: return;
            case LOSE: lifes--; break;
            case WIN:  if (time_spent < BASE_SCORE) { score += ((BASE_SCORE - time_spent) * lifes); }; reached++; break;
        }
    }
}