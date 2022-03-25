// **************************************************************************
// main menu pic, base version
// **************************************************************************
#pragma data_seg(bank_10_0_gfx)
#define GFX_BANK_MAIN_MENU_DAY 10
#define GFX_CHIP_MAIN_MENU_DAY 0
#define GFX_BANK_MAIN_MENU_NIGHT 16
#define GFX_CHIP_MAIN_MENU_NIGHT 0
__export char PIC_MM[] = kickasm {{
    .var mmPicA = LoadBinary("assets/multicolorGfx/main.kla", HALF_KOALA)
    *=$8000; .fill mmPicA.getBitmapSize(), mmPicA.getBitmap(i)
    *=$8f00; .fill mmPicA.getColorRamSize(), mmPicA.getColorRam(i)
    *=$90e0; .fill mmPicA.getScreenRamSize(), mmPicA.getScreenRam(i)
}};

#define SPR_MAIN_MENU_PIG $2f00

__export char SPR_MAIN_MENU_PIG_[64*1] = kickasm {{
    .byte $30, $38, $00
    .byte $38, $E0, $00
    .byte $6D, $C0, $04
    .byte $0F, $E0, $06
    .byte $1F, $EF, $83
    .byte $15, $FF, $F3
    .byte $15, $FF, $FE
    .byte $15, $FF, $FC
    .byte $3F, $FF, $9C
    .byte $6B, $FF, $7E
    .byte $6B, $FE, $FE
    .byte $3E, $FE, $FE
    .byte $01, $FE, $FE
    .byte $0F, $FE, $FE
    .byte $00, $7E, $7C
    .byte $01, $8F, $78
    .byte $01, $B8, $78
    .byte $01, $B8, $70
    .byte $03, $38, $70
    .byte $03, $18, $B0
    .byte $05, $28, $D0, 10
}};

// main menu pic, night version
#pragma data_seg(bank_16_0_gfx)
__export char PIC_MM1[] = kickasm {{
    .var mmPicA1 = LoadBinary("assets/multicolorGfx/main_night.kla", HALF_KOALA)
    *=$8000; .fill mmPicA1.getBitmapSize(), mmPicA1.getBitmap(i)
    *=$8f00; .fill mmPicA1.getColorRamSize(), mmPicA1.getColorRam(i)
    *=$90e0; .fill mmPicA1.getScreenRamSize(), mmPicA1.getScreenRam(i)
}};

// copy of sprites
__export char SPR_MAIN_MENU_PIG_COPY[64*1] = kickasm {{
    .byte $30, $38, $00
    .byte $38, $E0, $00
    .byte $6D, $C0, $04
    .byte $0F, $E0, $06
    .byte $1F, $EF, $83
    .byte $15, $FF, $F3
    .byte $15, $FF, $FE
    .byte $15, $FF, $FC
    .byte $3F, $FF, $9C
    .byte $6B, $FF, $7E
    .byte $6B, $FE, $FE
    .byte $3E, $FE, $FE
    .byte $01, $FE, $FE
    .byte $0F, $FE, $FE
    .byte $00, $7E, $7C
    .byte $01, $8F, $78
    .byte $01, $B8, $78
    .byte $01, $B8, $70
    .byte $03, $38, $70
    .byte $03, $18, $B0
    .byte $05, $28, $D0, 10
}};

// **************************************************************************
// farmland
// **************************************************************************
#pragma data_seg(bank_11_0_gfx)
#define GFX_BANK_FARMLAND_DAY 11
#define GFX_CHIP_FARMLAND_DAY 0
#define GFX_BANK_FARMLAND_NIGHT 17
#define GFX_CHIP_FARMLAND_NIGHT 0
__export char PIC_FARMLAND[] = kickasm {{
    .var farmland = LoadBinary("assets/multicolorGfx/farmland_bottomBar.kla", HALF_KOALA)
    *=$8000; .fill farmland.getBitmapSize(), farmland.getBitmap(i)
    *=$8f00; .fill farmland.getColorRamSize(), farmland.getColorRam(i)
    *=$90e0; .fill farmland.getScreenRamSize(), farmland.getScreenRam(i)
}};

#define SPR_THERMO_BAR_1 $2f00
#define SPR_THERMO_BAR_2 $2f40
#define SPR_THERMO_BAR_3 $2f80
// need to set right memory address
__export char * const SPR_THERMO_BAR_1_BANK = (char *)$ef00;
__export char * const SPR_THERMO_BAR_2_BANK = (char *)$ef40;
__export char * const SPR_THERMO_BAR_3_BANK = (char *)$ef80;

__export char SPR_THERMO_BAR_12[64*2] = kickasm {{
    .fill 128, 0
}};

__export char SPR_THERMO_BAR_[64*1] = kickasm {{
// Sprite Thermometer bar, lowest
// Single color mode, BG color: 0, Sprite color: 14
    .byte $00, $00, $00
    .byte $00, $00, $00
    .byte $00, $00, $00
    .byte $00, $00, $00
    .byte $00, $00, $00
    .byte $00, $00, $00
    .byte $00, $00, $00
    .byte $00, $00, $00
    .byte $00, $00, $00
    .byte $00, $00, $00
    .byte $00, $00, $00
    .byte $00, $00, $00
    .byte $00, $00, $00
    .byte $00, $00, $00
    .byte $00, $00, $00
    .byte $00, $00, $00
    .byte $00, $00, $00
    .byte $00, $00, $00
    .byte $00, $00, $00
    .byte $F0, $00, $00
    .byte $F0, $00, $00,0
}};
__export char * const SPR_POTATO_UI = (char *)$efc0;
__export char * const SPR_LUPINE_UI = (char *)$f000;
__export char * const SPR_WHEAT_UI  = (char *)$f040;
__export char * const SPR_CORN_UI  = (char *)$f080;

__export char SPR_PLANTS_[64*4] = kickasm {{
    .fill $ff, 0
}};

#pragma data_seg(bank_17_0_gfx)
__export char PIC_FARMLAND1[] = kickasm {{
    .var farmland1 = LoadBinary("assets/multicolorGfx/farmland_bottomBar_night.kla", HALF_KOALA)
    *=$8000; .fill farmland1.getBitmapSize(), farmland1.getBitmap(i)
    *=$8f00; .fill farmland1.getColorRamSize(), farmland1.getColorRam(i)
    *=$90e0; .fill farmland1.getScreenRamSize(), farmland1.getScreenRam(i)
}};

__export char SPR_THERMO_BAR_122[64*2] = kickasm {{
    .fill 128, 0
}};

__export char SPR_THERMO_BAR_N[64*1] = kickasm {{
// Sprite Thermometer bar, lowest
// Single color mode, BG color: 0, Sprite color: 14
    .byte $00, $00, $00
    .byte $00, $00, $00
    .byte $00, $00, $00
    .byte $00, $00, $00
    .byte $00, $00, $00
    .byte $00, $00, $00
    .byte $00, $00, $00
    .byte $00, $00, $00
    .byte $00, $00, $00
    .byte $00, $00, $00
    .byte $00, $00, $00
    .byte $00, $00, $00
    .byte $00, $00, $00
    .byte $00, $00, $00
    .byte $00, $00, $00
    .byte $00, $00, $00
    .byte $00, $00, $00
    .byte $00, $00, $00
    .byte $00, $00, $00
    .byte $F0, $00, $00
    .byte $F0, $00, $00,0
}};
__export char SPR_PLANTS_2[64*4] = kickasm {{
    .fill $ff, 0
}};
// **************************************************************************
// shop outside
// **************************************************************************
#pragma data_seg(bank_14_0_gfx)
#define GFX_BANK_SHOP_OUTSIDE 14
#define GFX_CHIP_SHOP_OUTSIDE 0
__export char PIC_SO[] = kickasm {{
    .var shopO = LoadBinary("assets/multicolorGfx/shopOutsideCropped.kla", HALF_KOALA)
    *=$8000; .fill shopO.getBitmapSize(), shopO.getBitmap(i)
    *=$8f00; .fill shopO.getColorRamSize(), shopO.getColorRam(i)
    *=$90e0; .fill shopO.getScreenRamSize(), shopO.getScreenRam(i)
}};
// **************************************************************************
// shop inside
// **************************************************************************
#pragma data_seg(bank_15_0_gfx)
#define GFX_BANK_SHOP_INSIDE 15
#define GFX_CHIP_SHOP_INSIDE 0
__export char PIC_SI[] = kickasm {{
    .var shopI = LoadBinary("assets/multicolorGfx/shopInsideCropped.kla", HALF_KOALA)
    *=$8000; .fill shopI.getBitmapSize(), shopI.getBitmap(i)
    *=$8f00; .fill shopI.getColorRamSize(), shopI.getColorRam(i)
    *=$90e0; .fill shopI.getScreenRamSize(), shopI.getScreenRam(i)
}};
// pickled gherkin anim
#define SHOP_INSIDE_ANIM_1_FRAMES 6
#define SHOP_INSIDE_ANIM_1_POINTER $2f00
// address here is just after the bitmap - that's where loader is going to copy animated sprites
__export char SHOP_INSIDE_ANIM_1[64*6] = kickasm {{
    // .var pickledGherkin = LoadPicture("assets/sprites/pickledGherkin.png", List().add($000000, $8e5029,$56ac4d,$553800))
    .var pickledGherkin = LoadPicture("assets/sprites/pickledGherkin.png", List().add($000000, $553800,$8e5029,$56ac4d))
    .for (var i=0; i<6; i++)
        :getSprite(pickledGherkin, i)
}};

#pragma data_seg(Data)
