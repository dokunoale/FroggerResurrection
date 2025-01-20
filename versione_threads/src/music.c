#include "music.h"
#include <stdlib.h>
#include <stdio.h>

#include <unistd.h>

#define MAX_VOLUME 32768
#define LIM_STR_BUF 256

#define VOL_EFCT_SET 8

static const str debug[2] = {">/dev/null 2>&1 &", ""};

static const str sound[] = {"select", "jump", "hit", "frogger" };
static const str music[] = {"alongtheriver", "pixelriver"}; 
static const int loop[]  = { YES, YES };

int sounds_volume = MAX_VOLUME;
int music_volume = MAX_VOLUME / 2;

void set_sounds_volume(int volume) {
    if (volume < 0 || volume > 10) return;
    sounds_volume = volume * MAX_VOLUME / 10;
}

void set_music_volume(int volume) {
    if (volume < 0 || volume > 10) return;
    music_volume = volume * MAX_VOLUME / 20;
}

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
    sprintf(cmd, "mpg123 -f%d ./src/audio/%s.mp3 %s", sounds_volume, sound[sound_id], debug[NON]);
    if (system(cmd) == -1) { perror("Error: "); exit(1); }
}

// Play a music
void play_music(int music_id) {
    if (VOL_EFCT_SET == 0) return; 
    stop_music();
    char cmd[LIM_STR_BUF];
    switch (loop[music_id]) {
        case NON: sprintf(cmd, "mpg123 -f%d -q ./src/audio/%s.mp3 %s", music_volume, music[music_id], debug[NON]); break;
        case YES: sprintf(cmd, "mpg123 -f%d -q --loop %d ./src/audio/%s.mp3 %s", music_volume, loop[music_id], music[music_id], debug[NON]); break;
        default: return;
    }
    
    if (system(cmd) == -1) { perror("Error: "); exit(1); }
}