#ifndef KITCHEN_TASK_H
#define KITCHEN_TASK_H

void execBakeBreadTask(char taskId);
void addBakeBreadTask();
bool addKitchenItem(FOOD_ITEMS item);

#pragma compile("kitchenTask.c")
#endif