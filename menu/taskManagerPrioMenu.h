#ifndef TASK_MANAGER_PRIO_MENU_H
#define TASK_MANAGER_PRIO_MENU_H
// Switching code generation to shared section
#pragma code ( mainCode )
#pragma data ( mainData )

extern const struct MenuOption TASK_MANAGER_PRIO_MENU[];
void showTaskManagerPrioMenu();

#pragma compile("taskManagerPrioMenu.c")
#endif
