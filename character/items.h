#ifndef ITEMS_H
#define ITEMS_H

#include <translation/common.h>
#include <engine/gameState.h>

// #define FOOD_ITEMS_COUNT 6

// enum FOOD_ITEMS {
//     FOOD_SHOP_BREAD,
//     FOOD_HOME_BREAD,
//     FOOD_CANNED_MEAT,
//     FOOD_SOUSAGE,
//     FOOD_POTATOES,
//     FOOD_CORN,
// };

// struct FoodItem {
//     // char * nameIdx;
//     unsigned int regenAmount;
//     char regenTime;
//     bool isHealing;
//     char weight;
//     unsigned int price;
// };

// extern __striped const struct FoodItem foodItems[FOOD_ITEMS_COUNT];

#pragma compile("items.c")
#endif