#include "entities.h"
#include "utils.h"
#include "struct.h"
#include "display.h"

void frog (int pipe_fd, int *args) {

    while (TRUE) {
        int c = getch();
        if (c == 'q') { break;}

        switch (c) {
            case KEY_UP: if (y > 1) y -= 1; break;
            case KEY_DOWN: if (y < w_lines - 2) y += 1; break;
            case KEY_RIGHT: if (x < w_cols - 3) x += 1; break;
            case KEY_LEFT: if (x > 2) x -= 1; break;
        }

        pos.x = x;
        pos.y = y;

        
    }
}