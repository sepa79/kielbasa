#ifndef MUSIC_H
#define MUSIC_H

struct Song {
    /* Text to display */
    const char* textIdx;
    /* SID bank */
    char bank;
    /* Song index in SID */
    char songIdx;
    /* SID index in the bank */
    char sidIdx;
    /* how much to copy */
    unsigned int size;
};

#define TITLE_ONLY 0xff
#define RADIO_PLAYLIST_SIZE 9
#define PLAYLIST_SIZE 12 + RADIO_PLAYLIST_SIZE

extern const struct Song PLAYLIST[];

void loadMusic(const struct Song * song);

#pragma compile("music.c")

#endif