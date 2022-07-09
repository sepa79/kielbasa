#include <c64/sprites.h>
#include <c64/keyboard.h>
#include <audio/sidfx.h>
#include <string.h>
#include <stdio.h>

// Sound effect for a player shot
SIDFX	SIDFXFire[1] = {{
	8000, 1000, 
	SID_CTRL_GATE | SID_CTRL_SAW,
	SID_ATK_16 | SID_DKY_114,
	0x40  | SID_DKY_750,
	-80, 0,
	4, 30
}};

// Sound effect for enemy explosion
SIDFX	SIDFXExplosion[1] = {{
	1000, 1000, 
	SID_CTRL_GATE | SID_CTRL_NOISE,
	SID_ATK_2 | SID_DKY_6,
	0xf0  | SID_DKY_1500,
	-20, 0,
	8, 40
}};

// Sound effect for player explosion
SIDFX	SIDFXBigExplosion[3] = {
	{
	1000, 1000, 
	SID_CTRL_GATE | SID_CTRL_SAW,
	SID_ATK_2 | SID_DKY_6,
	0xf0  | SID_DKY_168,
	-20, 0,
	4, 0
	},
	{
	1000, 1000, 
	SID_CTRL_GATE | SID_CTRL_NOISE,
	SID_ATK_2 | SID_DKY_6,
	0xf0  | SID_DKY_168,
	-20, 0,
	10, 0
	},
	{
	1000, 1000, 
	SID_CTRL_GATE | SID_CTRL_NOISE,
	SID_ATK_2 | SID_DKY_6,
	0xf0  | SID_DKY_1500,
	-10, 0,
	8, 80
	},	
};

int main(void)
{

	// Init sound effects state machine
	sidfx_init();

	// Full volume
	sid.fmodevol = 15;

  byte _key = 0;
  /* putchar(14); */

	for(;;)
	{
    // get the key
    keyb_poll();
    _key = keyb_codes[keyb_key & 0x7f];

    if (_key == 'r'){

        // SID FX - Fire button
        sidfx_play(0, SIDFXFire, 1);

        printf(p"r key\n");
    } else if (_key == 't'){

        // SID FX - Fire button
        sidfx_play(1, SIDFXExplosion, 1);

        printf(p"t key\n");
    } else if (_key == 'y'){

        // SID FX - Fire button
        sidfx_play(0, SIDFXBigExplosion, 3);

        printf(p"y key\n");
    } else if(_key != 20) {
        printf("key :  %d\n", _key);
    }
    // Update sound effects
    sidfx_loop();
		// Wait for one frame iteration
		vic_waitFrame();
	}

	return 0;
}

