#ifndef MUSIC_H
#define MUSIC_H

#if defined(__APPLE__)
#define SYSTEM_KILL "pkill"
#else
#define SYSTEM_KILL "skill"
#endif

#define str char*
#define NON 0
#define YES 1

static const str debug[2] = {">/dev/null 2>&1 &", ""};

static const str sound[4] = {"select", "jump", "hit", "frogger" };
typedef enum sounds { SELECT = 0, JUMP, HIT, INTRO } Sounds;
static const str music[1] = {"alongtheriver" };
typedef enum musics { EASY = 0 } Music;
static const int loop[1]  = { YES };

// Function prototypes
void play_sound(int sound_id);
void play_music(int music_id);
void stop_music(void);
void pause_music(void);
void resume_music(void);

#endif // MUSIC_H