#ifndef UNTITLED_FILEIO_H
#define UNTITLED_FILEIO_H

#include <vector>
#include <TaskManager.h>

namespace IO
{
    void fileIO(std::vector<TaskManager::Task>& tasks);
    void deleteTaskIO(std::vector<TaskManager::Task>& tasks,const char* taskID);
    void deleteAllTasksIO();
    void endTaskIO(const std::vector<TaskManager::Task>& tasks,const char* taskID,const char* status);
    void addTaskIO(const std::vector<TaskManager::Task>& tasks,char* argv[],int argc);
    void changeTaskIO(std::vector<TaskManager::Task>& tasks,const char* taskID);

}


#endif //UNTITLED_FILEIO_H