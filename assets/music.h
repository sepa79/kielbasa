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
    /* speed, in times per frame */
    unsigned int speed;
};

#define SPEED_1X 1
#define SPEED_2X 2

// don't forget to add space for album titles - radio playlist MUST NOT have these, so +1 to main playlist
#define TITLE_ONLY 0xff
#define RADIO_PLAYLIST_SIZE 4
#define PLAYLIST_SIZE 8 + RADIO_PLAYLIST_SIZE

extern const struct Song PLAYLIST[];

#pragma compile("music.c")

#endif