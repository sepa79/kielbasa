#ifndef TASK_MANAGER_MENU_H
#define TASK_MANAGER_MENU_H
// Switching code generation to shared section
#pragma code ( mainCode )
#pragma data ( mainData )

extern const struct MenuOption TASK_MANAGER_MENU[];
void showTaskManagerMenu();

#pragma compile("taskManagerMenu.c")
#endif
