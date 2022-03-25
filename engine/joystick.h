#ifndef JOYSTICK_H
#define JOYSTICK_H

enum JOY_DIR {
    JOY_UP=1,
    JOY_DOWN=2,
    JOY_LEFT=4,
    JOY_RIGHT=8,
    JOY_FIRE=16
};

extern char _joy1Status;
extern char _joy2Status;

#endif