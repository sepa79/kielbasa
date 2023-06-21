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
    initCharacterList(); // data kept in allCharacters
    initCalendar(&GS.calendar);
    initKitchen(&GS.kitchen);
    initTaskList(); // data kept in arrays in TaskManager
    initFarmland(&GS.farm);
    GS.vMap.x = 112;
    GS.vMap.y = 128;
    GS.vMap.location = 0;
    GS.vMap.direction = WALK_DOWN;
}

void loadGame() {

}

void saveGame(){

}
