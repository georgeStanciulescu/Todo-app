#include <TaskManager.h>
#include <Calculations.h>
#include <Constants.h>
#include <FileIO.h>
#include <Interface.h>
#include <filesystem>

TaskManager::TaskManager()
{
    fileIO(tasks);
}

TaskManager::DeletionType TaskManager::deleteTask(const char* taskID)
{
    if (std::string_view(taskID) == "all")
    {
        deleteAllTasksIO();
        return all;
    }

    deleteTaskIO(tasks,taskID);
    return single;
}

void TaskManager::endTask(const char* taskID,const char* status)
{
    endTaskIO(tasks,taskID,status);
}

void TaskManager::addTask(char* argv[],int argc)
{
    addTaskIO(tasks,argv,argc);
}

void TaskManager::changeTask(const char* taskID)
{
    changeTaskIO(tasks,taskID);
}

void TaskManager::listTasks()
{
    displayTotalList(tasks);
}








