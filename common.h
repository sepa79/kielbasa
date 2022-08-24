#ifndef COMMON_H
#define COMMON_H
// Switching code generation to shared section
#pragma code ( mainCode )
#pragma data ( mainData )

void splashScreen(bool turnOn, char stage);
inline signed char babs(signed char val);
void cwin_write_string_raw(CharWin * win, const char * buffer);

#pragma compile("common.c")

#endif