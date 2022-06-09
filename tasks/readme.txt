Tasks code - should be in mem all the time, called by TaskManager.

Task must handle task_status NEW and REMOVE, and should set the DONE one and call removeTask(taskId) when done.