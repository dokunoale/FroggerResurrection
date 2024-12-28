#include "entities.h"
#include "utils.h"
#include "struct.h"
#include "display.h"

// TODO: Implement the frog function
void frog (Buffer *buffer, Item *item) {

    while (TRUE) {
        int c = getch();
        if (c == 'q') { break;}

        switch (c) {
            case KEY_UP: if (item->line > 1) line -= 1; break;
            case KEY_DOWN: if (item->line < w_lines - 2) line += 1; break;
            case KEY_RIGHT: if (column < w_cols - 3) column += 1; break;
            case KEY_LEFT: if (x > 2) x -= 1; break;
        }

        pos.x = x;
        pos.y = y;

        
    }
}