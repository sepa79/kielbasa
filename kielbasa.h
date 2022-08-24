#ifndef KIELBASA_H
#define KIELBASA_H
// Main code & data region
#pragma section( mainCode, 0 )
#pragma section( mainData, 0 )

void mainLoop();

#pragma compile("kielbasa.c")

#endif