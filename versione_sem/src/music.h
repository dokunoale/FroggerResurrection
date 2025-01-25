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

typedef enum sounds { SELECT = 0, JUMP, HIT, INTRO } Sounds;
typedef enum musics { CHILL = 0, PIXEL } Music;


// Function prototypes
void set_sounds_volume(int volume);
void set_music_volume(int volume);
void play_sound(int sound_id);
void play_music(int music_id);
void stop_music(void);
void pause_music(void);
void resume_music(void);

#endif // MUSIC_H