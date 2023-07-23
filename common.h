#ifndef COMMON_H
#define COMMON_H

// stage:
//  1 - black to bright
//  2 - bright to black
//  3 - full
void splashScreen(bool turnOn, char stage);

#pragma compile("common.c")

#endif