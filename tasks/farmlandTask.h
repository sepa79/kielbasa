#ifndef FARMLAND_TASK_H
#define FARMLAND_TASK_H

void sowFieldTask(char taskId);
void reapFieldTask(char taskId);
void plowFieldTask(char taskId);

#pragma compile("farmlandTask.c")
#endif