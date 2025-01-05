#include "handler.h"

int edge_empty(Flow *flow) {
    if (flow->how_many_crocodiles == 0) { return 1; }
    switch (flow->direction) {
        case RIGHT: return flow->crocodiles[flow->how_many_crocodiles - 1].column > 0 ;
        case LEFT:  return flow->crocodiles[flow->how_many_crocodiles - 1].column + CROCODILE_DIM < GAME_WITDH;
        default:    return 0;
    }
}

Item* get_crocodile(Flow* flow, Item* item) {
    for (unsigned int i = 0; i < flow->how_many_crocodiles; i++) {
        if (flow->crocodiles[i].id == item->id) { return &flow->crocodiles[i]; }
    }
    return NULL;
}

void update_crocodile(Flow* flow, Item* item) {
    Item* crocodile = get_crocodile(flow, item);
    if (crocodile != NULL) { *crocodile = *item; }
}

Flow* new_flows() {
    Flow* flows = (Flow*)malloc(sizeof(Flow) * NUM_FLOWS);
    for (int i = 0; i < NUM_FLOWS; i++) {
        Item* crocodiles = (Item*)malloc(sizeof(Item) * CROCODILE_MAX_NUM);
        Flow flow = (Flow){
            .line = i,
            .direction = i % 2,
            .speed = MIN_SPEED + rand() % (MAX_SPEED - MIN_SPEED + 1),
            .how_many_crocodiles = 0,
            .crocodiles = crocodiles
        };
        flows[i] = flow;
    }
    return flows;
}

Item new_frog() {
    Item frog = (Item){
        .line = DEN_HEIGHT + NUM_FLOWS,
        .column = GAME_WITDH / 2,
        .type = FROG,
        .dimension = FROG_DIM,
        .speed = 0,
        .direction = 0,
        .id = 0
    };
    return frog;
}

Game new_game() {
    Game game = (Game){
        .flows = new_flows(),
        .frog = new_frog(),
    };
    return game;
}

void end_game(Buffer *buffer, Game* game) {
    for (int i = 0; i < NUM_FLOWS; i++) {
        for (unsigned int j = 0; j < game->flows[i].how_many_crocodiles; j++) {
            if (game->flows[i].crocodiles[j].id != 0) { killTask(buffer, &game->flows[i].crocodiles[j]); }
        }
        free(game->flows[i].crocodiles);
    }
    free(game->flows);
}

void new_crocodile(Buffer* buffer, Flow* flow) {
    if (flow->how_many_crocodiles < CROCODILE_MAX_NUM && edge_empty(flow) && rand() % PROBABILITY == 0) {
        Item new_crocodile = (Item){
            .line = flow->line + DEN_HEIGHT,
            .column = flow->direction == RIGHT ? 0 : GAME_WITDH - CROCODILE_DIM - 1,
            .type = CROCODILE,
            .dimension = CROCODILE_DIM,
            .speed = flow->speed,
            .direction = flow->direction,
            .id = flow->how_many_crocodiles
        };
        newTask(buffer, &crocodile, &new_crocodile);
        flow->crocodiles[flow->how_many_crocodiles] = new_crocodile;
        flow->how_many_crocodiles++;
    }
}

void new_crocodiles(Buffer* buffer, Game* game) {
    for (int i = 0; i < NUM_FLOWS; i++) {
        new_crocodile(buffer, &game->flows[i]);
    }
}

void manche(WINDOW* win) {
    Buffer buffer = newBuffer();
    Game game = new_game();
    newTask(&buffer, &frog, &game.frog);

    while(1) {
        Item receveid;
        readItem(&buffer, &receveid, MAIN_PIPE);
        new_crocodiles(&buffer, &game);

        switch (receveid.type) {
            case FROG:
                displayItem(win, &game.frog, &receveid);
                game.frog = receveid;
                break;
            case CROCODILE:
                mvwprintw(win, 0, 0, "flow %d -> %d", receveid.line - DEN_HEIGHT, game.flows[receveid.line - DEN_HEIGHT].how_many_crocodiles);
                displayItem(win, get_crocodile(&game.flows[receveid.line - DEN_HEIGHT], &receveid), &receveid);
                update_crocodile(&game.flows[receveid.line - DEN_HEIGHT], &receveid);
                break;
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

    end_game(&buffer, &game);
    killTask(&buffer, &game.frog);
}