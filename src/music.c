#include "music.h"
#include <stdlib.h>
#include <stdio.h>

#include <unistd.h>

#define MAX_VOLUME 32768
#define LIM_STR_BUF 256

#define VOL_EFCT_SET 8


// Stop all sounds
void stop_music(void) {
    system(SYSTEM_KILL " mpg123");
}

// Pause all sounds
void pause_music(void) {
    system(SYSTEM_KILL " -STOP mpg123");
}

// Resume all paused sounds
void resume_music(void) {
    system(SYSTEM_KILL " -CONT mpg123");
}

// Play a sound
void play_sound(int sound_id) {
    if (VOL_EFCT_SET == 0) return;
    char cmd[LIM_STR_BUF];
    sprintf(cmd, "mpg123 -f%d ./src/audio/%s.mp3 %s", VOL_EFCT_SET * MAX_VOLUME/10, sound[sound_id], debug[NON]);
    if (system(cmd) == -1) { perror("Error: "); exit(1); }
}

// Play a music
void play_music(int music_id) {
    if (VOL_EFCT_SET == 0) return; 
    stop_music();
    char cmd[LIM_STR_BUF];
    switch (loop[music_id]) {
        case NON: sprintf(cmd, "mpg123 -f%d -q ./src/audio/%s.mp3 %s", VOL_EFCT_SET * MAX_VOLUME/10, music[music_id], debug[NON]); break;
        case YES: sprintf(cmd, "mpg123 -f%d -q --loop %d ./src/audio/%s.mp3 %s", VOL_EFCT_SET * MAX_VOLUME/10, loop[music_id], music[music_id], debug[NON]); break;
        default: return;
    }
    
    if (system(cmd) == -1) { perror("Error: "); exit(1); }
}