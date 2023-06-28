#include <engine/gameState.h>

// Sections and regions
#pragma section( gameStateData, 0 )
#pragma region( gameStateDataRegion, 0x0316, 0x03ff, , , { gameStateData } )
#pragma section( allCharactersData, 0 )
#pragma region( allCharactersDataRegion, 0x0200, 0x0313, , , { allCharactersData } )


#pragma bss ( gameStateData )
struct GameState GS;

#pragma bss ( allCharactersData )
struct Character allCharacters[CHARACTER_COUNT];

// Switching code generation back to shared section
#pragma bss ( data )

void initGame() {
    // https://www.prawo.pl/akty/m-p-1991-4-23,16823109.html
    // 1/12th of 1989 r. - 49.583
    GS.cash = 4582;
    GS.pension = 4582;
    GS.inflation = 20;
    GS.bills = 80;
    initCharacterList(); // data kept in allCharacters
    initCalendar(&GS.calendar);
    initKitchen(&GS.kitchen);
    initTaskList(); // data kept in arrays in TaskManager
    initFarmland(&GS.farm);
    GS.vMap.x = 110;
    GS.vMap.y = 180;
    GS.vMap.location = 0;
    GS.vMap.direction = WALK_NONE;
}

void loadGame() {

}

void saveGame(){

}
