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
BattleEngine battleEngine;
struct Player player;
Actor* playerT[1];
Actor* enemyT[1];

static void displaySortedActors(BattleEngine* battleEngine, int x, int y) {
  cwin_putat_string_raw(&cw, x, y, s"Sorted actors:", VCOL_LT_GREY);
  for (int i = 0; i < battleEngine->numActors; i++) {
      cwin_putat_string_raw(&cw, x, y + i + 1, battleEngine->sortedActors[i]->name, VCOL_MED_GREY);
  }
}

static void displayActorStats(int x, int y, Actor* selectedActor) {
    // Display the actor's name.
    cwin_putat_string_raw(&cw, x, y, selectedActor->name, VCOL_LT_GREY);

    // Display the actor's stats.
    char str[3];
    sprintf(str, "%3d", selectedActor->energy);
    cwin_putat_string_raw(&cw, x + 0, y + 2, "EN:", VCOL_LT_GREY);
    cwin_putat_string_raw(&cw, x + 3, y + 2, str, VCOL_LT_GREY);
    sprintf(str, "%2d", selectedActor->strength);
    cwin_putat_string_raw(&cw, x + 0, y + 3, "STR:", VCOL_LT_GREY);
    cwin_putat_string_raw(&cw, x + 4, y + 3, str, VCOL_LT_GREY);
    sprintf(str, "%2d", selectedActor->dexterity);
    cwin_putat_string_raw(&cw, x + 0, y + 4, "DEX:", VCOL_LT_GREY);
    cwin_putat_string_raw(&cw, x + 4, y + 4, str, VCOL_LT_GREY);
    sprintf(str, "%2d", selectedActor->regenerationPoints);
    cwin_putat_string_raw(&cw, x + 0, y + 5, "REG:", VCOL_LT_GREY);
    cwin_putat_string_raw(&cw, x + 4, y + 5, str, VCOL_LT_GREY);
}

static void battleTurn() {
    // Call the battle engine main loop
    BattleStatus battleStatus = BattleEngine_mainLoop(&battleEngine);

    // Check the battle status and take appropriate action
    switch (battleStatus) {
        // Player's turn
        case BATTLE_STATUS_WAITING_FOR_INPUT:
            cwin_putat_string_raw(&cw, 0, 8, "PLAYER TURN", VCOL_LT_GREY);

            // update screen
            break;
        // NPC turn
        case BATTLE_STATUS_WAITING_FOR_SCREEN_UPDATE:
            cwin_putat_string_raw(&cw, 0, 8, "NPC TURN", VCOL_LT_GREY);

            // update screen
            break;

        // Battle over
        case BATTLE_STATUS_TEAM_A_WIN:
            cwin_putat_string_raw(&cw, 0, 8, "PLAYER WON ", VCOL_LT_GREY);

            break;

        // Battle over
        case BATTLE_STATUS_TEAM_B_WIN:
            cwin_putat_string_raw(&cw, 0, 8, "PLAYER LOST", VCOL_LT_GREY);
            break;

        default:
        // Unknown battle status
        break;
    }
    displaySortedActors(&battleEngine, 15, 5);
    // displayActiveActorStats(&battleEngine);
}

// Function declarations for battleMenu options
static void _battleMenuAttack() {
    player.selectedAction = ACTION_ATTACK;
    player.selectedTarget = enemyT[0];
    cwin_putat_string_raw(&cw, 0, 7, "ATTACK", VCOL_LT_GREY);
    battleTurn();
};

static void _battleMenuDefend() {
    player.selectedAction = ACTION_DEFEND;
    player.selectedTarget = nullptr;
    cwin_putat_string_raw(&cw, 0, 7, "DEFEND", VCOL_LT_GREY);
    battleTurn();
};

const struct MenuOption BATTLE_MENU[] = {
    { TXT_IDX_BATTLE_MENU_ATTACK, '1', SCREEN_SPLIT_MC_TXT, UI_SELECT, &_battleMenuAttack, 0, 16, 1 },
    { TXT_IDX_BATTLE_MENU_DEFEND, '2', SCREEN_SPLIT_MC_TXT, UI_SELECT, &_battleMenuDefend, 0, 16, 2 },
    { TXT_IDX_MENU_EXIT, KEY_ARROW_LEFT, SCREEN_TRANSITION, UI_LF, &showMenu, MENU_BANK_MAIN_MENU, 16, 3},
    END_MENU_CHOICES
};

static void _menuHandler() {
    mnu_isGfxLoaded = false;
    loadMenuGfx();
    // loadMenuSprites();

    // Prepare output window
    cwin_init(&cw, GFX_1_SCR, SCREEN_X_START, SCREEN_Y_START, SCREEN_WIDTH, SCREEN_HEIGHT);
    cwin_clear(&cw);

    // Initialize 2 actors with basic but different stats
    player.name = s"Player";
    player.strength = 10;
    player.dexterity = 10;
    player.energy = 100;
    player.regenerationPoints = 10;
    player.playerControlled = true;
    player.selectedAction = ACTION_NONE;
    player.Attack = &Player_Attack;
    player.Defend = &Player_Defend;
    player.experiencePoints = 0;

    struct Actor enemy1 = {
        .name = s"Dog",
        .strength = 5,
        .dexterity = 15,
        .energy = 100,
        .regenerationPoints = 5,
        .playerControlled = false,
        .selectedAction = ACTION_NONE,
        .Attack = &Actor_Attack,
        .Defend = &Actor_Defend,
    };

    // struct Actor enemy2 = {
    // .name = s"Fat Dog",
    // .strength = 7,
    // .dexterity = 12,
    // .energy = 100,
    // .regenerationPoints = 6,
    // .Attack = &Actor_Attack,
    // .Defend = &Actor_Defend
    // };

    // Add the player actor to teamA.
    playerT[0] = &player;

    // Add the enemy actors to teamB.
    enemyT[0] = &enemy1;
    // teamB[1] = &enemy2;

    // Calculate the number of actors in each team.
    char numActorsA = 1;
    char numActorsB = 1;

    // Initialize the battle engine.
    BattleEngine_init(&battleEngine, playerT, enemyT, numActorsA, numActorsB);

    displayMenu(BATTLE_MENU);
    switchScreenTo(SCREEN_SPLIT_MC_TXT);

    displaySortedActors(&battleEngine, 15, 5);
    // displayAllActorStats();
    // start battle
    battleTurn();
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
