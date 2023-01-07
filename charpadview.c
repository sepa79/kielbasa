#include <c64/vic.h>
#include <c64/memmap.h>
#include <c64/charwin.h>
#include <c64/keyboard.h>
#include <string.h>

static const char ro_attribs[] = {
    #embed "assets/charGfx/kielbasaMapS2 - CharAttribs_L1.bin"	
};

static const char ro_chars[] = {
    #embed "assets/charGfx/kielbasaMapS2 - Chars.bin"	
};

#define MAP_SIZE_X 32
#define MAP_SIZE_Y 24
static const char ro_map[] = {
    #embed "assets/charGfx/kielbasaMapS2 - (8bpc, 32x24) Map.bin"	
};

static const char ro_tiles[] = {
    #embed "assets/charGfx/kielbasaMapS2 - Tiles.bin"	
};

static const char ro_tileAttribs[] = {
    #embed "assets/charGfx/kielbasaMapS2 - TileAttribs_L1.bin"	
};

static char ro_xmap[MAP_SIZE_X * MAP_SIZE_Y];

char * const Font = (char *)0xd000;
char * const Screen = (char *)0xc000;
char * const Color = (char *)0xd800;

void tiles_remap(void)
{
    for(char y=0; y<MAP_SIZE_Y; y++)
    {
        for(char x=0; x<MAP_SIZE_X; x++)
        {
            ro_xmap[MAP_SIZE_X * y + x] = ro_map[MAP_SIZE_X * y + x];
        }
    }
}

void tiles_put4x4row0(char * dp, char * cp, const char * mp, const char * tp)
{
    for(char tx=0; tx<10; tx++)
    {
        const char  * ti = tp + mp[tx] * 16;

#assign cx 0
#repeat
        {
            char ci = ti[cx];
            dp[cx] = ci;
            cp[cx] = ro_tileAttribs[mp[tx]];
        }
#assign cx cx + 1
#until cx == 4

        dp += 4;
        cp += 4;
    }
}

void tiles_put4x4row1(char * dp, char * cp, const char * mp, const char * tp)
{
    const char  * ti = tp + mp[0] * 16 + 1;

#assign cx 0
#repeat
    {
        char ci = ti[cx];
        dp[cx] = ci;
        cp[cx] = ro_tileAttribs[mp[0]];
    }
#assign cx cx + 1
#until cx == 3

    dp += 3;
    cp += 3;

    for(char tx=1; tx<10; tx++)
    {
        ti = tp + mp[tx] * 16;

#assign cx 0
#repeat
        {
            char ci = ti[cx];
            dp[cx] = ci;
            cp[cx] = ro_tileAttribs[mp[tx]];
        }
#assign cx cx + 1
#until cx == 4

        dp += 4;
        cp += 4;
    }

    ti = tp + mp[10] * 16;

#assign cx 0
#repeat
    {
        char ci = ti[cx];
        dp[cx] = ci;
        cp[cx] = ro_tileAttribs[mp[10]];
    }
#assign cx cx + 1
#until cx == 1
}

void tiles_put4x4row2(char * dp, char * cp, const char * mp, const char * tp)
{
    const char  * ti = tp + mp[0] * 16 + 2;

#assign cx 0
#repeat
    {
        char ci = ti[cx];
        dp[cx] = ci;
        cp[cx] = ro_tileAttribs[mp[0]];
    }
#assign cx cx + 1
#until cx == 2

    dp += 2;
    cp += 2;

    for(char tx=1; tx<10; tx++)
    {
        ti = tp + mp[tx] * 16;

#assign cx 0
#repeat
        {
            char ci = ti[cx];
            dp[cx] = ci;
            cp[cx] = ro_tileAttribs[mp[tx]];
        }
#assign cx cx + 1
#until cx == 4

        dp += 4;
        cp += 4;
    }

    ti = tp + mp[10] * 16;

#assign cx 0
#repeat
    {
        char ci = ti[cx];
        dp[cx] = ci;
        cp[cx] = ro_tileAttribs[mp[10]];
    }
#assign cx cx + 1
#until cx == 2
}

void tiles_put4x4row3(char * dp, char * cp, const char * mp, const char * tp)
{
    const char  * ti = tp + mp[0] * 16 + 3;

#assign cx 0
#repeat
    {
        char ci = ti[cx];
        dp[cx] = ci;
        cp[cx] = ro_tileAttribs[mp[0]];
    }
#assign cx cx + 1
#until cx == 1

    dp += 1;
    cp += 1;

    for(char tx=1; tx<10; tx++)
    {
        ti = tp + mp[tx] * 16;

#assign cx 0
#repeat
        {
            char ci = ti[cx];
            dp[cx] = ci;
            cp[cx] = ro_tileAttribs[mp[tx]];
        }
#assign cx cx + 1
#until cx == 4

        dp += 4;
        cp += 4;
    }

    ti = tp + mp[10] * 16;

#assign cx 0
#repeat
    {
        char ci = ti[cx];
        dp[cx] = ci;
        cp[cx] = ro_tileAttribs[mp[10]];
    }
#assign cx cx + 1
#until cx == 3
}

void tiles_put4x4(const char * mp, char ox, char oy)
{
    char * dp = Screen, * cp = Color;

    mp += MAP_SIZE_X * (oy >> 2) + (ox >> 2);
    oy &= 3;
    ox &= 3;

    for(char ty=0; ty<25; ty++)
    {
        switch (ox)
        {
            case 0:
                tiles_put4x4row0(dp, cp, mp, ro_tiles + 4 * oy);
                break;
            case 1:
                tiles_put4x4row1(dp, cp, mp, ro_tiles + 4 * oy);
                break;
            case 2:
                tiles_put4x4row2(dp, cp, mp, ro_tiles + 4 * oy);
                break;
            case 3:
                tiles_put4x4row3(dp, cp, mp, ro_tiles + 4 * oy);
                break;
        }
        dp += 40;
        cp += 40;

        oy ++;
        if (oy == 4)
        {
            mp += MAP_SIZE_X;
            oy = 0;
        }
    }
}

int main(void)
{
    mmap_trampoline();

    mmap_set(MMAP_RAM);

    memcpy(Font, ro_chars, sizeof(ro_chars));

    mmap_set(MMAP_NO_BASIC);

    vic_setmode(VICM_TEXT_MC, Screen, Font);

    vic.color_back = VCOL_BROWN;
    vic.color_border = VCOL_BLACK;
    vic.color_back1 = VCOL_BLACK;
    vic.color_back2 = VCOL_WHITE;

    tiles_remap();

    char	ox = 0, oy =0;
    while (true)
    {
        tiles_put4x4(ro_xmap, ox, oy);

        char ch = cwin_getch();
        switch (ch)
        {
            case KEY_CSR_DOWN:
                if (oy < MAP_SIZE_Y*3-1)
                    oy++;
                break;
            case KEY_CSR_UP:
                if (oy > 0)
                    oy--;
                break;
            case KEY_CSR_LEFT:
                if (ox > 0)
                    ox--;
                break;
            case KEY_CSR_RIGHT:
                if (ox < MAP_SIZE_X*3-2*4)
                    ox++;
                break;
        }
    }

    while (true) ;

    return 0;
}



