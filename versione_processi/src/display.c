#include "display.h"
#include "utils.h"
#include <locale.h>
#include <string.h>

static const char *title[] = {
    "░▒▓████████▓▒░▒▓███████▓▒░ ░▒▓██████▓▒░ ░▒▓██████▓▒░ ░▒▓██████▓▒░░▒▓████████▓▒░▒▓███████▓▒░ ",         
    "░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░",       
    "░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░      ░▒▓█▓▒░      ░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░",         
    "░▒▓██████▓▒░ ░▒▓███████▓▒░░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒▒▓███▓▒░▒▓█▓▒▒▓███▓▒░▒▓██████▓▒░ ░▒▓███████▓▒░ ",        
    "░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░",         
    "░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░",       
    "░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░░▒▓██████▓▒░ ░▒▓██████▓▒░ ░▒▓██████▓▒░░▒▓████████▓▒░▒▓█▓▒░░▒▓█▓▒░",            
};

static const char *croc_right_sprite[4][24] = {
    { " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "▂", " ", " ", " ", "▂", "▄", "▂", " ", " ", " ", " ", " ", " " },
    { " ", " ", "▄", "▂", "▂", "▆", "▄", "▄", "█", "▆", "▆", "█", "█", "▆", "█", "█", "▒", "█", "█", "▄", "▄", "▄", "█", "▄" },
    { "▒", "▓", "█", "█", "█", "█", "█", "█", "█", "█", "█", "█", "█", "█", "█", "█", "█", "█", "▘", "▜", "▙", "▗", "█", "▀" },
    { "▒", "▓", "█", "█", "█", "█", "█", "█", "█", "█", "█", "█", "█", "█", "█", "█", "█", "█", "█", "█", "█", "█", "▌", " " }
};

static const char *croc_left_sprite[4][24] = {
    { " ", " ", " ", " ", " ", " ", "▂", "▄", "▂", " ", " ", " ", "▂", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " " },
    { "▄", "█", "▄", "▄", "▄", "█", "█", "▒", "█", "█", "▆", "█", "█", "▆", "▆", "█", "▄", "▄", "▆", "▂", "▂", "▄", " ", " " },
    { "▀", "█", "▖", "▟", "▛", "▝", "█", "█", "█", "█", "█", "█", "█", "█", "█", "█", "█", "█", "█", "█", "█", "█", "▓", "▒" },
    { " ", "▐", "█", "█", "█", "█", "█", "█", "█", "█", "█", "█", "█", "█", "█", "█", "█", "█", "█", "█", "█", "█", "▓", "▒" }
};


static const char *wave_left_sprite[8][24] = {
    {"█", "▄", "█", "█", "▄", "▄", " ", " ", " ", " ", "▄", "▄", "▄", "▄", " ", " ", " ", " ", "▄", "▄", "█", "█", "▄", "▄"},
    {"█", " ", "▄", "▄", "▄", "▄", " ", " ", " ", " ", "▄", "▄", "█", "█", "▄", "▄", " ", " ", " ", " ", "▄", "▄", "▄", "▄"},
    {"█", " ", " ", " ", "▄", "▄", "▄", "▄", " ", " ", " ", " ", "▄", "▄", "█", "█", "▄", "▄", " ", " ", " ", "▄", "▄", "▄"},
    {"█", " ", " ", " ", "▄", "▄", "█", "█", "▄", "▄", " ", " ", " ", " ", "▄", "▄", "▄", "▄", " ", " ", " ", "▄", "▄", "█"},
    {"█", " ", " ", " ", "▄", "▄", "█", "█", "▄", "▄", " ", " ", " ", " ", "▄", "▄", "▄", "▄", " ", " ", " ", " ", "▄", "▄"},
    {"█", "▄", "▄", " ", " ", " ", "▄", "▄", "▄", "▄", " ", " ", " ", " ", "▄", "▄", "█", "█", "▄", "▄", " ", " ", " ", " "},
    {"█", "█", "▄", "▄", " ", " ", " ", " ", "▄", "▄", "▄", "▄", " ", " ", " ", " ", "▄", "▄", "█", "█", "▄", "▄", " ", " "},
    {"█", "▄", "▄", "▄", " ", " ", " ", " ", "▄", "▄", "█", "█", "▄", "▄", " ", " ", " ", " ", "▄", "▄", "▄", "▄", " ", " "}
};

static const char *wave_right_sprite[8][24] = {
    {"▄", "▄", "▄", "▄", " ", " ", " ", " ", "▄", "▄", "█", "█", "▄", "▄", " ", " ", " ", " ", "▄", "▄", "▄", "▄", " ", "█"},
    {"█", "█", "▄", "▄", " ", " ", " ", " ", "▄", "▄", "▄", "▄", " ", " ", " ", " ", "▄", "▄", "█", "█", "▄", "▄", " ", "█"},
    {"█", "▄", "▄", " ", " ", " ", "▄", "▄", "▄", "▄", " ", " ", " ", " ", "▄", "▄", "█", "█", "▄", "▄", " ", " ", " ", "█"},
    {"▄", " ", " ", " ", "▄", "▄", "█", "█", "▄", "▄", " ", " ", " ", " ", "▄", "▄", "▄", "▄", " ", " ", " ", " ", "▄", "█"},
    {" ", " ", " ", " ", "▄", "▄", "█", "█", "▄", "▄", " ", " ", " ", " ", "▄", "▄", "▄", "▄", " ", " ", " ", "▄", "▄", "█"},
    {" ", " ", " ", " ", "▄", "▄", "▄", "▄", " ", " ", " ", " ", "▄", "▄", "█", "█", "▄", "▄", " ", " ", " ", "▄", "▄", "█"},
    {" ", " ", "▄", "▄", "▄", "▄", " ", " ", " ", " ", "▄", "▄", "█", "█", "▄", "▄", " ", " ", " ", " ", "▄", "▄", "▄", "█"},
    {"▄", "▄", "█", "█", "▄", "▄", " ", " ", " ", " ", "▄", "▄", "▄", "▄", " ", " ", " ", " ", "▄", "▄", "█", "█", "▄", "█"}
};

static const char *frog_sprite[4] = {
    "▄ ▄██▄ ▄",
    "▀██████▀",
    "▄█▀██▀█▄",
    "▀      ▀"
};

static const char *den_sprite[5] = {
    "  ▄▄▄▄  ",
    "▄███▀   ",
    "████▄▄██",
    "▀██████▀",
    "  ▀▀▀▀  "
};

static const char *numbers[10][3] = {
    {"█▀█", "█ █", "▀▀▀"}, // zero
    {"▀█ ", " █ ", "▀▀▀"}, // one
    {"▀▀█", "█▀▀", "▀▀▀"}, // two
    {"▀▀█", " ▀█", "▀▀▀"}, // three
    {"█ ▄", "█▄█", "  ▀"}, // four
    {"█▀▀", "▀▀█", "▀▀▀"}, // five
    {"█▀▀", "█▀█", "▀▀▀"}, // six
    {"▀▀█", " ▄▀", " ▀ "}, // seven
    {"█▀█", "█▀█", "▀▀▀"}, // eight
    {"█▀█", "▀▀█", "▀▀▀"}  // nine
};

static const char *heart[2] = { 
    "▄ ▄", 
    "▀█▀" 
};

static const char *you_win[2] = { 
    "█ █ █▀█ █ █   █ █ █ █▀█ █▀█", 
    " █  █▄█ █▄█   █▄█▄█ █▄█ █ █" 
};

static const char *game_over[2] = { 
    "█▀▀ █▀█ █▀▄▀█ ███   █▀█ █ █ ███ █▀█",
    "█▄█ █▀█ █ ▀ █ █▄▄   █▄█ ▀▄▀ █▄▄ █▀▄ "
};

static const char *button[3] = { 
    "▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁",
    "▓▌                ▐▓",
    "▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔"
};

void initDisplay() {
    setlocale(LC_ALL, "");

    initscr();
    cbreak(); // Don't wait ENTER in getch()
    noecho(); // Hide character typed
    curs_set(NO_CURSOR); // Remove cursor
    keypad(stdscr, TRUE); // Enable function keys listener
    nodelay(stdscr, TRUE);
    clear(); refresh();

    start_color();

    // init_color(REAL_WHITE, 800, 800, 800); 
   
    init_pair(FULL_BLACK_COLOR, COLOR_BLACK, COLOR_BLACK);
    init_pair(WATER_COLOR, COLOR_CYAN, COLOR_CYAN);
    init_pair(FROG_COLOR, COLOR_BLACK, COLOR_YELLOW);
    init_pair(DROWNED_COLOR, COLOR_BLACK, COLOR_CYAN);
    init_pair(CROCODILE_COLOR, COLOR_YELLOW, COLOR_CYAN);
    init_pair(WAVE_COLOR, COLOR_CYAN, COLOR_YELLOW);
    init_pair(BULLET_COLOR, COLOR_RED, COLOR_RED);
    init_pair(DEN_COLOR, COLOR_GREEN, COLOR_CYAN);
    init_pair(TEXT_COLOR, COLOR_WHITE, COLOR_BLACK);
    init_pair(SCORE_COLOR, COLOR_YELLOW, COLOR_BLACK);
    init_pair(HEART_COLOR, COLOR_RED, COLOR_BLACK);
    init_pair(TITLE_COLOR, COLOR_WHITE, COLOR_CYAN);
}

int detectScreenSize() {
    if(LINES < WIN_MAIN_HEIGHT || COLS < WIN_MAIN_WIDTH) {
        WINDOW* err_win = newwin(3, 20, (LINES - 3) / 2, (COLS - 20) / 2);
        box(err_win, 0, 0);
        mvwprintw(err_win, 1, 1, "%s", "Terminal too small");
        wrefresh(err_win);
        while(getch() == ERR) { /* do nothing */ }
        delwin(err_win);
        return 0;
    }
    return 1;
}

void fill(WINDOW *win, int line, int column, int height, int width, int color) {
    wattron(win, COLOR_PAIR(color));
    for (int i = 0; i < height; ++i) { 
        for (int j = 0; j < width; ++j) {
            mvwaddch(win, line + i, column + j, ' ' | A_REVERSE);
        }
    }
    wattroff(win, COLOR_PAIR(color));
}

void displayFrog(WINDOW* win, int line, int column, int color) {
    wattron(win, COLOR_PAIR(color));
    for (int i = 0; i < 4; i++) {
        mvwprintw(win, line + i, column, "%s", frog_sprite[i]);
    }
    wattroff(win, COLOR_PAIR(color));
}

void displayDen(WINDOW* win, int line, int column) {
    wattron(win, COLOR_PAIR(DEN_COLOR));
    for (int i = 0; i < 5; i++) {
        mvwprintw(win, line + i, column, "%s", den_sprite[i]);
    }
    wattroff(win, COLOR_PAIR(DEN_COLOR));
}

void displayCrocodile(WINDOW* win, int line, int column, int direction, int stage) {
    wattron(win, COLOR_PAIR(CROCODILE_COLOR));
    int start_index = (column > 0) ? 0 : -column;
    int end_index = (column < WIN_GAME_WIDTH) ? CROCODILE_DIM : (column + CROCODILE_DIM) - WIN_GAME_WIDTH;
    for (int i = 0; i < 4; i++) {
        for (int j = start_index; j < end_index; j++) {
            mvwprintw(win, line + i, column + j, "%s", (direction == RIGHT) ? croc_right_sprite[i][j] : croc_left_sprite[i][j]);
        }
    }
    wattroff(win, COLOR_PAIR(CROCODILE_COLOR));
    wattron(win, COLOR_PAIR(WAVE_COLOR));
    for (int j = start_index; j < end_index; j++) {
        mvwprintw(win, line + 4, column + j, "%s", (direction == RIGHT) ? wave_right_sprite[stage][j] : wave_left_sprite[stage][j]);
    }
    wattroff(win, COLOR_PAIR(WAVE_COLOR));
}

void displayScore(WINDOW* win, int score, int line, int column) {
    int thousands = score / 1000;
    int hundreds = (score % 1000) / 100;
    int tens = (score % 100) / 10;
    int units = score % 10;

    wattron(win, COLOR_PAIR(SCORE_COLOR));
    
    for (int j=0; j<3; j++) {
        mvwprintw(win, line+j, column, "%s", numbers[thousands][j]);
        mvwprintw(win, line+j, column + 4, "%s", numbers[hundreds][j]);
        mvwprintw(win, line+j, column + 8, "%s", numbers[tens][j]);
        mvwprintw(win, line+j, column + 12, "%s", numbers[units][j]);
    }   

    wattroff(win, COLOR_PAIR(SCORE_COLOR));
    wrefresh(win);
}

void displayHearts(WINDOW* win, int lifes) {
    for (int i = 0; i < LIFES; i++) {
        if (i < lifes) {
            for (int j = 0; j < 2; j++) {
                wattron(win, COLOR_PAIR(HEART_COLOR));
                mvwprintw(win, 1+j, HEART_INFO + i + i * HEART_DIM, "%s", heart[j]);
                wattroff(win, COLOR_PAIR(HEART_COLOR));
            }
        } else {
            fill(win, 1, HEART_INFO + i + i * HEART_DIM, 2, HEART_DIM, FULL_BLACK_COLOR);
        }
    }
}

void displayTimer(WINDOW* win) {
    fill(win, 1, 1, 2, WIN_GAME_WIDTH - 2, CROCODILE_COLOR);
}

void displayEnd(WINDOW* win, int status) {
    wattron(win, COLOR_PAIR(HEART_COLOR));
    for (int i = 0; i < 2; i++) { 
        switch (status) {
            case LOSE: {
                mvwprintw(win, 2 + i, (WIN_MENU_WIDTH - 35) / 2, "%s", game_over[i]);
            } break;
            case WIN: {
                mvwprintw(win, 2 + i, (WIN_MENU_WIDTH - 27) / 2, "%s", you_win[i]);
            } break;
        }
    } 
    wrefresh(win);
    wattroff(win, COLOR_PAIR(HEART_COLOR));
}

void displayDeath(WINDOW* win, Item* old, Item* new) {
    if (old->type != FROG) { return; }
    fill(win, old->line * WIN_HEIGHT_RATIO + 1, old->column, WIN_HEIGHT_RATIO - 1, FROG_DIM, CROCODILE_COLOR);
    for (int i = 0; i < 4; i++) {
        fill(win, new->line * WIN_HEIGHT_RATIO + 1, new->column, WIN_HEIGHT_RATIO - 1, FROG_DIM, WATER_COLOR);
        wrefresh(win); usleep(USLEEP * 10);
        displayFrog(win, new->line * WIN_HEIGHT_RATIO + 1, new->column, DROWNED_COLOR);
        wrefresh(win); usleep(USLEEP * 10);
    }
}

void displayButton(WINDOW* win, int line, int column, const char* text, int selected) {
    int text_length = strlen(text);
    int start_column = column + (20 - text_length) / 2;

    wattron(win, COLOR_PAIR(TEXT_COLOR));
    for (int i = 0; i < 3; i++) { mvwprintw(win, line + i, column, "%s", button[i]); }
    wattroff(win, COLOR_PAIR(TEXT_COLOR));

    if (selected) {wattron(win, A_REVERSE);} else {wattron(win, COLOR_PAIR(TEXT_COLOR));}
    mvwprintw(win, line + 1, column + 1, "%s", "                  ");
    mvwprintw(win, line + 1, start_column, "%s", text);
    if (selected) {wattroff(win, A_REVERSE);} else {wattroff(win, COLOR_PAIR(TEXT_COLOR));}

    wrefresh(win);
}

void displayItem(WINDOW* win, Item* old, Item* new) {
    if (old == NULL || new == NULL) { return; }
    
    switch (old->type) {
        case FROG: { // mvwprintw(win, new->line * WIN_HEIGHT_RATIO, 1, "Frog: %d  ", new->column);
            fill(win, old->line * WIN_HEIGHT_RATIO + 1, old->column, WIN_HEIGHT_RATIO - 1, FROG_DIM, CROCODILE_COLOR);
            displayFrog(win, new->line * WIN_HEIGHT_RATIO + 1, new->column, FROG_COLOR);
        } break;
        case CROCODILE: {
            fill(win, old->line * WIN_HEIGHT_RATIO, old->column, WIN_HEIGHT_RATIO, CROCODILE_DIM, WATER_COLOR);
            // fill(win, new->line * WIN_HEIGHT_RATIO + 1, new->column, WIN_HEIGHT_RATIO - 1, CROCODILE_DIM, CROCODILE_COLOR);
            displayCrocodile(win, new->line * WIN_HEIGHT_RATIO, new->column, new->direction, old->stage);
        } break;
        case BULLET: { 
            fill(win, old->line * WIN_HEIGHT_RATIO + 2, old->column, 1, BULLET_DIM, WATER_COLOR);
            fill(win, new->line * WIN_HEIGHT_RATIO + 2, new->column, 1, BULLET_DIM, BULLET_COLOR);
        } break;
        case GRANADE: {
            fill(win, old->line * WIN_HEIGHT_RATIO + 2, old->column, 1, GRANADE_DIM, WATER_COLOR);
            fill(win, new->line * WIN_HEIGHT_RATIO + 2, new->column, 1, GRANADE_DIM, FULL_BLACK_COLOR);
        } break;
        case DEN: {
            fill(win, old->line * WIN_HEIGHT_RATIO, old->column, WIN_HEIGHT_RATIO, FROG_DIM, WATER_COLOR);
            displayDen(win, new->line * WIN_HEIGHT_RATIO, new->column);
        } break;
        case TIMER: {
            fill(win, new->line * WIN_HEIGHT_RATIO, new->column, WIN_HEIGHT_RATIO, FROG_DIM, FULL_BLACK_COLOR);
        } break;
    }

    // Aggiorna la finestra
    wrefresh(win);
}

void displayTitle(WINDOW* win) {
    wattron(win, COLOR_PAIR(DROWNED_COLOR));
    for (int i = 0; i < 7; i++) { mvwprintw(win, i + 5, (WIN_MAIN_WIDTH - 92) / 2, "%s", title[i]); }
    wattroff(win, COLOR_PAIR(DROWNED_COLOR));
    wrefresh(win);
}