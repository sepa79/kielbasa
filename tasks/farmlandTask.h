#ifndef FARMLAND_TASK_H
#define FARMLAND_TASK_H
// Switching code generation to shared section
#pragma code ( mainCode )
#pragma data ( mainData )

void sowFieldTask(byte taskId);

#pragma compile("farmlandTask.c")
#endif