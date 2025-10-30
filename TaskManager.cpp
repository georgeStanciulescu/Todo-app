#include <TaskManager.h>
#include <Calculations.h>
#include <Constants.h>
#include <FileIO.h>
#include <Interface.h>
#include <filesystem>

#include "Date.h"

TaskManager::TaskManager()
{
    IO::fileIO(tasks);
}

TaskManager::DeletionType TaskManager::deleteTask(const char* taskID)
{
    if (std::string_view(taskID) == "all")
    {
        IO::deleteAllTasksIO();
        return all;
    }

    IO::deleteTaskIO(tasks,taskID);
    return single;
}

void TaskManager::endTask(const char* taskID,const char* status)
{
    IO::endTaskIO(tasks,taskID,status);
}

void TaskManager::addTask(char* argv[],int argc)
{
    IO::addTaskIO(tasks,argv,argc);
    //dateDropdown();
}

void TaskManager::changeTask(const char* taskID)
{
    IO::changeTaskIO(tasks,taskID);
}

void TaskManager::listTasks()
{
    Interface::displayTotalList(tasks);
}

bool TaskManager::duplicateCheck(const char* duplicateArg)

{
    bool isDuplicate{false};

    for (const auto& task : tasks) {

        if (task.description == std::string_view(duplicateArg)){isDuplicate = true;}
    }

    return Interface::userWantsDuplicate(isDuplicate);
}







