#ifndef OPTIONS_MENU_H
#define OPTIONS_MENU_H

extern const struct MenuOption OPTIONS_MENU[];
extern const struct MenuOption MUSIC_MENU[];
void showOptionsMenu();
// used by Shop and other places with Radio
void playNextRadioSong();
// Plays given song from main PLAYLIST
void playSong(char song);

#pragma compile("optionsMenu.c")
#endif
