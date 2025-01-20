#include "utils.h"
#include "display.h"
#include "entities.h"
#include "handler.h"
#include "music.h"
#include <string.h>

void read_record(int score[10], char* player[10]) {
    FILE* file = fopen("./src/.record", "r");
    if (file == NULL) { return; }

    char line[256];
    int index = 0;
    while (fgets(line, sizeof(line), file) && index < 10) {
        char* token = strtok(line, " ");
        if (token != NULL) {
            score[index] = atoi(token);
            token = strtok(NULL, " ");
            if (token != NULL) {
                token[strcspn(token, "\n")] = '\0';
                player[index] = strdup(token);
                index++;
            }
        }
    }

    fclose(file);
}

void write_record(int score, char* player) {
    int scores[10]; char* players[10];
    read_record(scores, players);

    // Replace spaces with underscores in the player's name
    for (char* p = player; *p; p++) {
        if (*p == ' ') {
            *p = '_';
        }
    }

    // Check if the new score is higher than the lowest score in the top 10
    if (score <= scores[9]) { return; }

    // Add the new score and player to the arrays
    scores[9] = score;
    players[9] = strdup(player);

    // Sort the scores and players in descending order
    for (int i = 0; i < 10; i++) {
        for (int j = i + 1; j < 10; j++) {
            if (scores[i] < scores[j]) {
                int temp_score = scores[i];
                scores[i] = scores[j];
                scores[j] = temp_score;

                char* temp_player = players[i];
                players[i] = players[j];
                players[j] = temp_player;
            }
        }
    }

    // Write the top 10 scores and players to the file
    FILE* file = fopen("./src/.record", "w");
    if (file == NULL) { return; }

    // Clear the file content before writing
    freopen(NULL, "w", file);

    for (int i = 0; i < 10; i++) {
        if (players[i] != NULL) {
            fprintf(file, "%d %s\n", scores[i], players[i]);
            free(players[i]);
        }
    }

    fclose(file);
}

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

// Checks if any item is above another one.
int is_above(Item* item, Item* other) {
    return (item->line == other->line) && (item->column >= other->column) && (item->column + (int)item->dimension <= other->column + (int)other->dimension);
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
        if (stream->type == DEN && stream[i].stage != 0 && is_above(item, &stream[i]) ) { return &stream[i];; }
        if (stream[i].id != 0 && is_above(item, &stream[i])) { return &stream[i]; }
    }
    return NULL;
}

// Check if any item collides with another one. TODO: modificare
int is_colliding(Item* item, Item* other) {
    return (item->line == other->line) && (item->column >= other->column) && (item->column <= other->column + (int)other->dimension);
}

// Check if any item collides with another stream of Item (array) and returns the pointer to the item below.
Item* is_colliding_any(Item* item, Item* stream) {
    unsigned int max;
    switch (stream->type) {
        case CROCODILE: max = CROCODILE_MAX_NUM; break;
        case BULLET: max = BULLET_MAX_NUM; break;
        case DEN: max = 5; break;
        default: return NULL;
    }
    for (unsigned int i = 0; i < max; i++) {
        if (stream->type == DEN && stream[i].stage != 0 && is_colliding(item, &stream[i]) ) { return &stream[i];; }
        if (stream[i].id != 0 && is_colliding(item, &stream[i])) { return &stream[i]; }
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
            .speed = choose(MAX_SPEED, MIN_SPEED),
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
        .stage = 0,
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
        .stage = 0,
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
        .stage = 0,
        .id = 0
    };
    newTask(buffer, &bullet, &item);
    return item;
}

Item new_granade(Buffer* buffer, Item* source, int direction) {
    Item item = (Item){
        .line = source->line,
        .column = direction == RIGHT ? source->column + FROG_DIM : source->column - BULLET_DIM,
        .type = GRANADE,
        .dimension = GRANADE_DIM,
        .speed = GRANADE_SPEED,
        .direction = direction,
        .stage = 0,
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
        .stage = 1,
        .id = 0
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
        .stage = 0,
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
    
    for (int i = 0; i < DEN_NUM; i++) { if (den[i].stage != 0) displayItem(win, &den[i], &den[i]); }
    Item frog_item = new_frog(&buffer);
    new_timer(&buffer); displayTimer(timer_win);

    int exit_status = FROG;
    while(1) {
        Item receveid;
        readItem(&buffer, &receveid, MAIN_BUF);
        Flow* flow = &flows[receveid.line - DEN_HEIGHT];

        // Creates new random crocodiles if there is space on each flow
        for (unsigned int i = 0; i < NUM_FLOWS; i++) { fill_flow(&buffer, &flows[i]); }
        
        switch (receveid.type) {
            case FROG: {
                if (receveid.line != frog_item.line) { play_sound(JUMP); }
                if (receveid.line == 0) { 
                    Item* reached = is_above_any(&receveid, den);
                    if (reached == NULL) { exit_status = LOSE; play_sound(HIT); displayDeath(win, &frog_item, &receveid); break; }
                    else { reached->stage = 0; exit_status = WIN; displayItem(win, &frog_item, &receveid); break; }
                }
                if (receveid.line >= DEN_HEIGHT + NUM_FLOWS ||receveid.line <= DEN_HEIGHT) { /* do nothing */ }
                else if (is_above_any(&receveid, flow->crocodiles) == NULL) { exit_status = LOSE; play_sound(HIT); displayDeath(win, &frog_item, &receveid); break; }

                displayItem(win, &frog_item, &receveid);
                frog_item = receveid;
            } break;

            case CROCODILE: {
                Item* stored = get_item(flow, &receveid);
                if (stored == NULL) { break; }
                displayItem(win, stored, &receveid);

                *stored = receveid;
                crocodile_shot(flow, &buffer, stored);

                if (is_above(&frog_item, stored)) { moveFrog(&buffer, frog_item, flow->direction); }
                if (out_of_bounds(&receveid)) { killTask(stored); } 
            } break;     

            case BULLET: {
                Item* stored = get_item(flow, &receveid);
                if (stored == NULL) { break; }
                displayItem(win, stored, &receveid);

                if (is_above(&receveid, &frog_item)) { exit_status = LOSE; play_sound(HIT); displayDeath(win, &frog_item, &frog_item); break; }
                Item* collided = is_above_any(&receveid, flow->granades);

                *stored = receveid;
                if (collided != NULL) { killTask(collided); killTask(stored); break; }
                if (out_of_bounds(&receveid)) { killTask(stored); }
            } break;

            case SHOT: {
                if (frog_item.line < DEN_HEIGHT || frog_item.line >= DEN_HEIGHT + NUM_FLOWS) { break; }
                frog_shot(flow, &buffer, &frog_item);
            } break;

            case GRANADE: {
                Item* stored = get_item(flow, &receveid);
                if (stored == NULL) { break; }
                Item* collided = is_colliding_any(&receveid, flow->bullets);
                displayItem(win, stored, &receveid);

                *stored = receveid;
                if (collided != NULL) { killTask(collided); killTask(stored); break; }
                if (out_of_bounds(&receveid)) { killTask(stored); }
            } break;

            case TIMER: { displayItem(timer_win, &receveid, &receveid); } break;
            case LOSE: exit_status = LOSE; break;
            case EXIT: exit_status = EXIT; break;
        }
        if ( exit_status == EXIT || exit_status == WIN || exit_status == LOSE ) { break; }
    }

    free_flows(flows);
    killTask(&frog_item);
    freeBuffer(&buffer);
    return exit_status;
}

int game() {
    WINDOW* timer_win = newwin(WIN_TIMER_HEIGHT - 1, WIN_TIMER_WIDTH, 1, 1);
    WINDOW* game_win = newwin(WIN_GAME_HEIGHT, WIN_GAME_WIDTH, WIN_TIMER_HEIGHT + 1, 1); 
    WINDOW* info_win = newwin(WIN_INFO_HEIGHT, WIN_INFO_WIDTH, WIN_GAME_HEIGHT + WIN_TIMER_HEIGHT + 1, 1);
    wrefresh(game_win); wbkgd(game_win, COLOR_PAIR(WATER_COLOR)); wrefresh(game_win);

    Item den[DEN_NUM] = { new_den(1), new_den(2), new_den(3), new_den(4), new_den(5) };

    int lifes = LIFES;
    int reached = 0;
    int score = 0;

    play_music(CHILL);
    
    while(lifes >= 0 && reached < DEN_NUM) {
        wclear(game_win); wclear(info_win); wrefresh(game_win); wrefresh(info_win);
        
        fill(game_win, WIN_HEIGHT_RATIO + 1, 0, WIN_HEIGHT_RATIO - 1, WIN_GAME_WIDTH, CROCODILE_COLOR);
        fill(game_win, (NUM_FLOWS + DEN_HEIGHT) *  WIN_HEIGHT_RATIO + 1, 0, WIN_HEIGHT_RATIO - 1, WIN_GAME_WIDTH, CROCODILE_COLOR);
        wrefresh(game_win);
        displayHearts(info_win, lifes);
        displayScore(info_win, score, 1, SCORE_INFO);

        time_t start_time = time(NULL); 
        int status = manche(game_win, timer_win, den);
        time_t end_time = time(NULL); 

        int time_spent = (int)difftime(end_time, start_time);
        
        switch (status) {
            case LOSE: lifes--; break;
            case WIN:  if (time_spent < BASE_SCORE) { score += ((BASE_SCORE - time_spent) * (lifes + 1)); }; reached++; break;
        }
        if (status == EXIT) { break; }
    }

    displayScore(info_win, score, 1, SCORE_INFO);
    stop_music();

    WINDOW* end_win = newwin(WIN_MENU_HEIGHT, WIN_MENU_WIDTH, (WIN_GAME_HEIGHT - WIN_MENU_HEIGHT) / 2, (WIN_GAME_WIDTH - WIN_MENU_WIDTH) / 2);
    box(end_win, 0, 0); 
    int selected = LEFT;

    if (reached == DEN_NUM) { displayEnd(end_win, WIN); 
        wattron(end_win, TEXT_COLOR); 
        mvwprintw(end_win, 6, (WIN_MENU_WIDTH - 40) / 2, "Write your name > "); 
        mvwprintw(end_win, 7, (WIN_MENU_WIDTH - 40) / 2, "▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔"); 
        wrefresh(end_win);

        char name[16];
        wrefresh(end_win);
        int c;
        int index = 0;
        while (index < 15) {
            if (input(&c) == 0) { continue; }
            if (c == '\n' || c == '\r') {
                break;
            } else if (c == KEY_BACKSPACE || c == 127) {
                if (index > 0) {
                    index--;
                    name[index] = '\0';
                    mvwprintw(end_win, 6, (WIN_MENU_WIDTH - 40) / 2 + 18 + index, " ");
                    wmove(end_win, 6, (WIN_MENU_WIDTH - 40) / 2 + 18 + index);
                    wrefresh(end_win);
                }
            } else if (c >= ' ' && c <= '~') {
                name[index++] = c;
                mvwprintw(end_win, 6, (WIN_MENU_WIDTH - 40) / 2 + 18 + index - 1, "%c", c);
                wrefresh(end_win);
            }
        }
        name[index] = '\0';
        wattroff(end_win, TEXT_COLOR);

        write_record(score, name);
        selected = LEFT;

    } else { displayEnd(end_win, LOSE); 
        int c = KEY_LEFT; 
        while(1) { 
            switch (c) {
                case KEY_LEFT: selected = LEFT; displayButton(end_win, 6, 3, "Back to menu", TRUE); displayButton(end_win, 6, 25, "Play again", FALSE); break;
                case KEY_RIGHT: selected = RIGHT; displayButton(end_win, 6, 3, "Back to menu", FALSE); displayButton(end_win, 6, 25, "Play again", TRUE); break;
            }
            loop_input(&c)
            if (c == '\n' || c == ' ') { play_sound(SELECT); usleep(USLEEP * 100); break; }
            if (c == 'q') { delwin(end_win); return 0; }
            usleep(USLEEP); 
        }
    }
    
    
    
    delwin(end_win);
    return selected == LEFT ? 0 : 1;
}

void showRecord() {
    WINDOW* record_win = newwin(WIN_MENU_HEIGHT, WIN_MENU_WIDTH, (WIN_GAME_HEIGHT - WIN_MENU_HEIGHT) / 2, (WIN_GAME_WIDTH - WIN_MENU_WIDTH) / 2);
    box(record_win, 0, 0); wrefresh(record_win);

    int scores[10]; char* players[10]; read_record(scores, players);

    wattron(record_win, TEXT_COLOR); 

    for (int i = 0; i < 3; i++) {
        if (players[i] != NULL) {
            mvwprintw(record_win, (i*3) + 2, 1, "%2d. %s", i + 1, players[i]);
            mvwprintw(record_win, (i*3) + 3, 1, "    %d", scores[i]);
            free(players[i]);
        }
    }

    for (int i = 3; i < 10; i++) {
        if (players[i] != NULL) {
            mvwprintw(record_win, i - 1, 22, "%2d. %-15s %d", i + 1, players[i], scores[i]);
            free(players[i]);
        }
    }
    wattroff(record_win, TEXT_COLOR); 
    wrefresh(record_win);
    int c = 0;
    loop_input(&c)
    delwin(record_win);
}

int menu() {
    WINDOW* win = newwin(WIN_MENU_HEIGHT, WIN_MENU_WIDTH, (WIN_GAME_HEIGHT - WIN_MENU_HEIGHT) / 2, (WIN_GAME_WIDTH - WIN_MENU_WIDTH) / 2);
    box(win, 0, 0); wrefresh(win);
    play_music(CHILL); 

    // TODO: stampa il titolo
    static const int settings_line = 1;
    static const int settings_height = 3;

    displayButton(win, setline(0), (WIN_MENU_WIDTH - 20) / 2, "PLAY GAME", TRUE);
    displayButton(win, setline(1), (WIN_MENU_WIDTH - 20) / 2, "RECORD", FALSE);
    displayButton(win, setline(2), (WIN_MENU_WIDTH - 20) / 2, "QUIT", FALSE);

    int c = KEY_UP; unsigned int selected = 0;
    while (1) {
        loop_input(&c)
        switch (c) {
            case KEY_UP: selected = (selected - 1) % 3; break;
            case KEY_DOWN: selected = (selected + 1) % 3; break;
        }

        displayButton(win, setline(0), (WIN_MENU_WIDTH - 20) / 2, "PLAY GAME", FALSE);
        displayButton(win, setline(1), (WIN_MENU_WIDTH - 20) / 2, "RECORD", FALSE);
        displayButton(win, setline(2), (WIN_MENU_WIDTH - 20) / 2, "QUIT", FALSE);

        switch (selected) {
            case PLAY:      displayButton(win, setline(0), (WIN_MENU_WIDTH - 20) / 2, "PLAY GAME", TRUE); break;
            case RECORD:    displayButton(win, setline(1), (WIN_MENU_WIDTH - 20) / 2, "RECORD", TRUE); break;
            case QUIT:      displayButton(win, setline(2), (WIN_MENU_WIDTH - 20) / 2, "QUIT", TRUE); break;
        }

        if (c == '\n' || c == ' ') { play_sound(SELECT); usleep(USLEEP * 100); break; }
        if (c == 'q') { stop_music(); delwin(win); return QUIT; }
    }

    delwin(win);
    stop_music(); return (int)selected;
}

