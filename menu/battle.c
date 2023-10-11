#include <c64/vic.h>
#include <c64/charwin.h>
#include <c64/keyboard.h>

#include <menu/menuSystem.h>
#include <translation/common.h>
#include <engine/easyFlashBanks.h>
#include <assets/assetsSettings.h>
#include <engine/uiHandler.h>

#include <miniGame/actor.h>
#include <miniGame/player.h>
#include <miniGame/battleEngine.h>

// Sections and regions
#pragma section(battleMenuLoaderData, 0)
#pragma section(battleMenuCode, 0)
#pragma section(battleMenuGfxDay, 0)
#pragma section(battleMenuSprites, 0)
#pragma region(bankBattleMenuC, 0x8000, 0x9000, , MENU_BANK_BATTLE_MENU, { battleMenuLoaderData, battleMenuCode })
#pragma region(bankBattleMenuG1, DAY_GFX_BMP, DAY_GFX_BMP + 0x1400, , MENU_BANK_BATTLE_MENU, { battleMenuGfxDay })
#pragma region(bankBattleMenuG3, MENU_SPRITE_SRC, MENU_SPRITE_SRC + 0x0400, , MENU_BANK_BATTLE_MENU, { battleMenuSprites })

#pragma data(battleMenuGfxDay)
__export const char battleMenuGfx1[] = {
    #embed 0x0f00 0x0002 "assets/multicolorGfx/church_300823.kla"
    #embed 0x01e0 0x1f42 "assets/multicolorGfx/church_300823.kla"
    #embed 0x01e0 0x232a "assets/multicolorGfx/church_300823.kla"
};

// menu code is in ROM - data in RAM
#pragma code(battleMenuCode)
#pragma data(data)

// Function declarations for battleMenu options
// static void _bmMenuAttack();
// static void _bmMenuDefend();
// static void _bmMenuItems();
// static void _bmMenuFlee();

const struct MenuOption BATTLE_MENU[] = {
    // { TXT_IDX_BATTLE_MENU_ATTACK, KEY_ARROW_UP, SCREEN_SPLIT_MC_TXT, UI_SELECT, &_bmMenuAttack, 0, 1, 1 },
    // { TXT_IDX_BATTLE_MENU_DEFEND, KEY_ARROW_DOWN, SCREEN_SPLIT_MC_TXT, UI_SELECT, &_bmMenuDefend, 0, 1, 2 },
    // { TXT_IDX_BATTLE_MENU_ITEMS, KEY_ARROW_LEFT, SCREEN_SPLIT_MC_TXT, UI_SELECT, &_bmMenuItems, 0, 1, 3 },
    { TXT_IDX_MENU_EXIT, KEY_ARROW_LEFT, SCREEN_TRANSITION, UI_LF, &showMenu, MENU_BANK_MAIN_MENU, 2, 11},
    END_MENU_CHOICES
};

static void displaySortedActors(BattleEngine* battleEngine, CharWin* cw, int x, int y) {
  cwin_putat_string_raw(cw, x, y, s"Sorted actors:", VCOL_LT_GREY);
  for (int i = 0; i < battleEngine->numActors; i++) {
    cwin_putat_string_raw(cw, x, y + i + 1, battleEngine->sortedActors[i]->name, VCOL_MED_GREY);
  }
}

static void _menuHandler() {
    mnu_isGfxLoaded = false;
    loadMenuGfx();
    // loadMenuSprites();

    // Prepare output window
    cwin_init(&cw, GFX_1_SCR, SCREEN_X_START, SCREEN_Y_START, SCREEN_WIDTH, SCREEN_HEIGHT);
    cwin_clear(&cw);

    // Initialize 2 actors with basic but different stats

    struct Player player = {
    .name = s"Player",
    .strength = 10,
    .dexterity = 10,
    .energy = 100,
    .regenerationPoints = 10,
    .experiencePoints = 0,
    .Attack = &Player_Attack,
    .Defend = &Player_Defend
    };

    struct Actor enemy1 = {
    .name = s"Dog",
    .strength = 5,
    .dexterity = 15,
    .energy = 100,
    .regenerationPoints = 5,
    .Attack = &Actor_Attack,
    .Defend = &Actor_Defend
    };

    struct Actor enemy2 = {
    .name = s"Fat Dog",
    .strength = 7,
    .dexterity = 12,
    .energy = 100,
    .regenerationPoints = 6,
    .Attack = &Actor_Attack,
    .Defend = &Actor_Defend
    };

    Actor* teamA[1];
    Actor* teamB[2];

    // Add the player actor to teamA.
    teamA[0] = &player;

    // Add the enemy actors to teamB.
    teamB[0] = &enemy1;
    teamB[1] = &enemy2;

    // Calculate the number of actors in each team.
    char numActorsA = 1;
    char numActorsB = 2;

    // Initialize the battle engine.
    BattleEngine battleEngine;
    BattleEngine_init(&battleEngine, teamA, teamB, numActorsA, numActorsB);

    displayMenu(BATTLE_MENU);
    switchScreenTo(SCREEN_SPLIT_MC_TXT);

    displaySortedActors(&battleEngine, &cw, 0, 0);
}

#pragma data(battleMenuLoaderData)

__export static const Loaders menuLoaders = {
    .loadMenuCode    = &menuNoop,
    .loadMenuGfx     = &menuGfxLoaderSingleBitmap,
    .loadMenuSprites = &menuSpriteLoader,
    .showMenu        = &_menuHandler,
    .showSprites     = &spriteNoop,
    .updateMenu      = &menuNoop,
    .runMenuLoop     = &menuNoop,
};

// Switching code generation back to shared section
#pragma code(code)
#pragma data(data)
