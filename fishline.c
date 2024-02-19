#include <c64/vic.h>
#include <c64/memmap.h>
#include <string.h>
#include <math.h>


__export const char fishingMenuGfx1[] = {
    #embed 0xffff 2 "assets/multicolorGfx/fishing_060224a.kla"
};

char * const GFX_1_SCR = (char *)0xc000;
char * const GFX_1_BMP = (char *)0xe000;
char * const GFX_1_COLOR = (char *)0xd800;

__striped const char * const ySrcT[200] = {
#for(i,200) fishingMenuGfx1 + 40 * (i & ~7)  + (i & 7),
};

__striped char * const yDstT[200] = {
#for(i,200) GFX_1_BMP + 40 * (i & ~7)  + (i & 7),
};

__striped char const xan7[256] = {
#for(i,256) i & ~7,
};

const char setT[] =  {0b11000000,0b11000000, 0b00110000,0b00110000, 0b00001100,0b00001100, 0b00000011,0b00000011};


inline void drawPointAnd(char x, char y) 
{
    char m = x & 7;
    char xo = x & ~7;

    yDstT[y][xo] = ySrcT[y][xo] | setT[m];
}

void drawPointTab(char x, char y) 
{
    char m = x & 7;
    char xo = xan7[x];

    yDstT[y][xo] = ySrcT[y][xo] | setT[m];
}

__export const char tantab[64] =
{
#for(i, 64) (char)(tan(i * PI / 256) * 128),
};

__export const char costab[64] =
{
#for(i, 64) (char)(cos(i * PI / 256) * 120),
};

void draw(char n, char d)
{
	unsigned	x = 160;

	const char * sp = fishingMenuGfx1 + 320 * 3 + (x & ~7);
	char * dp = GFX_1_BMP + 320 * 3 + (x & ~7);

	char i = 0;
	char m = setT[x & 7];
	char xl = 0;

	char y = 0;
	for(; i+8<n; i+=8)
	{
		#pragma unroll(full)
		for(char j=0; j<8; j++)
		{
			dp[y] = sp[y] | m;
			y++;

			unsigned s = xl + d;
			xl = s;

			if (s & 0xff00)
			{
				m >>= 2;
				if (!m)
				{
					m = 0xc0;
					y += 8;
				}
			}
		}
		sp += 320; dp += 320;
		y -= 8;
	}

	for(;i<n; i++)
	{	    		
		dp[y] = sp[y] | m;
		y++;

		unsigned s = xl + d;
		xl = s;

		if (s & 0xff00)
		{
			m >>= 2;
			if (!m)
			{
				m = 0xc0;
				y += 8;
			}
		}
	}
}

void clear(char n, char d)
{
	unsigned	x = 160;

	const char * sp = fishingMenuGfx1 + 320 * 3 + (x & ~7);
	char * dp = GFX_1_BMP + 320 * 3 + (x & ~7);

	char i = 0;
	char m = setT[x & 7];
	char xl = 0;

	char y = 0;
	for(; i+8<n; i+=8)
	{
		#pragma unroll(full)
		for(char j=0; j<8; j++)
		{
			dp[y] = sp[y];

			unsigned s = xl + d;
			xl = s;

			if (s & 0xff00)
			{
				m >>= 2;
				if (!m)
				{
					m = 0xc0;
					y += 8;
				}
			}

			y++;
		}
		y -= 8;
		sp += 320; dp += 320;
	}

	for(;i<n; i++)
	{	    		
		dp[y] = sp[y];

		unsigned s = xl + d;
		xl = s;

		if (s & 0xff00)
		{
			m >>= 2;
			if (!m)
			{
				m = 0xc0;
				y += 8;
			}
		}

		y++;
	}
}

int main(void)
{
	mmap_trampoline();

	memcpy(GFX_1_BMP, fishingMenuGfx1, 8000);
	memcpy(GFX_1_SCR, fishingMenuGfx1 + 8000, 1000);
	memcpy(GFX_1_COLOR, fishingMenuGfx1 + 9000, 1000);

	vic_setmode(VICM_HIRES_MC, GFX_1_SCR, GFX_1_BMP);


	char n = costab[0], d = tantab[0];

	while (true)
	{
		for(char i=0; i<64; i++)
		{
			unsigned	x = 160;

			vic_waitBottom();
			vic.color_border++;

			clear(n, d);
			n = costab[i];
			d = tantab[i];
			draw(n, d);

			vic.color_border--;
			vic_waitTop();
		}
		for(char i=63; i>0; i--)
		{
			unsigned	x = 160;

			vic_waitBottom();
			vic.color_border++;

			clear(n, d);
			n = costab[i];
			d = tantab[i];
			draw(n, d);

			vic.color_border--;
			vic_waitTop();
		}
	}

	return 0;
}
