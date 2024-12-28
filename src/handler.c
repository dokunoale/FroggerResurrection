#include "utils.h"
#include "struct.h"
#include "handler.h"

void manche() {
    Buffer buffer = newBuffer();
    newTask(buffer.pipe_fd, buffer.pid_list, 0, NULL, NULL);

    Game game = initGame();

}

// Item frog = {line : 0, column : 0, type : FROG, dimension : FROG_DIM};



//Funzione per inizializzare la struct del gioco
Game initGame()
{
    Game game;
    game.Frog = {10; 7; FROG; FROG_DIM}; //posizione di partenza della rana
    game.flows[0] = rand_num(LEFT, RIGHT) //primo flusso deciso random tra destra e sinistra

    for(int i=1; i < NUM_FLOWS; i++)
    {
        game.flows[i].id = i; //numero del flusso
        int previous_flow = game.flows[i-1].direction; //direzione del precedente

        //direzione contraria al flusso precedente
        switch (previous_flow)
        {
        case RIGHT: game.flows[i].direction = LEFT;
            break;  
        case LEFT: game.flows[i].direction = RIGHT;
            break;
        }

        
        game.flows[i].speed = 0; // TO DO = decisione velocità flussi

        game.flows[i].num_croc_in_flow = 0; //all'inizio 0 coccodrilli nel flusso

    }

    return game;
}