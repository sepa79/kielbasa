#ifndef OPTIONS_MENU_H
#define OPTIONS_MENU_H

extern const struct MenuOption OPTIONS_MENU[];
extern const struct MenuOption MUSIC_MENU[];
void showOptionsMenu();
// used by Shop and other places with Radio
void playNextRadioSong();
// Plays given song from main PLAYLIST, does not restart it if already playing
void playSong(char song);
// Starts given song from the beginning, regardles if it is playing already or not
void startSong(char song);

#pragma compile("optionsMenu.c")
#endif
