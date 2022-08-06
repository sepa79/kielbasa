#include <c64/vic.h>
#include <assets/assetsSettings.h>
#include <c64/joystick.h>
#include <c64/sprites.h>

#define IRQ1RAS 50
#define IRQ2RAS IRQ1RAS + 152

// Raster IRQ Routine playing music
__interrupt void msxIrq1(){
    vic.color_border++;
	// Poll joystick
	joy_poll(0);

	// Move crosshair coordinates
	CrossX += 2 * joyx[0]; CrossY += 2 * joyy[0];

	// Stop at edges of screen
	if (CrossX < 8)
		CrossX = 8;
	else if (CrossX > 312)
		CrossX = 312;
	if (CrossY < 20)
		CrossY = 20;
	else if (CrossY > 172)
		CrossY = 172;

	// Move crosshair sprite
	// spr_move(0, CrossX + 14, CrossY + 40);	

	// Check button
	if (joyb[0])
	{
		// Avoid quickfire and bouncing
		if (CrossDelay == 0)
		{
			// Request fire from non interrupt code
			CrossP = true;
			CrossDelay = 4;
		}
	}
	else if (CrossDelay > 0)
		CrossDelay--;
    
    // play music
    __asm {
        jsr MSX_PLAY
        dec $d020
    }
}

__interrupt void msxIrq2(){
    vic.color_border++;

    __asm {
        jsr MSX_PLAY
        dec $d020
    }
}
