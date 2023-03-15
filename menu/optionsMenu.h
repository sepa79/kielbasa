#ifndef OPTIONS_MENU_H
#define OPTIONS_MENU_H

struct Song {
    /* Text to display */
    const char* textIdx;
    /* SID bank */
    byte bank;
    /* Song index in SID */
    byte songIdx;
    /* SID index in the bank */
    byte sidIdx;
};

extern const struct Song PLAYLIST[];
extern const struct MenuOption OPTIONS_MENU[];
extern const struct MenuOption MUSIC_MENU[];
void showOptionsMenu();
// used by Shop and other places with Radio
void playNextRadioSong();
// Plays given song from main PLAYLIST
void playSong(char song);

#pragma compile("optionsMenu.c")
#endif
