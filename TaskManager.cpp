#include <TaskManager.h>
#include <FileIO.h>
#include <Interface.h>

#include "InterfaceComposition.h"


TaskManager::TaskManager()
{
    IO::fileIO(tasks);
}

void TaskManager::deleteTasks(const std::vector<int>& taskIDs)
{
    IO::deleteTasksIO(tasks,taskIDs);
}

void TaskManager::deleteAllTasks()
{
    IO::deleteAllTasksIO();
}

void TaskManager::endTask(const char* taskID,const char* status,const std::string& startDate,const std::string& dueDate)
{
    IO::endTaskIO(tasks,taskID,status,startDate,dueDate);
}

void TaskManager::addTask(char* argv[],int argc)
{
    IO::addTaskIO(tasks,argv,argc);
}

void TaskManager::changeTask(const char* taskID)
{
    IO::changeTaskIO(tasks,taskID);
}

void TaskManager::listSimple()
{
    Interface::displayTotalList(tasks);
}

void TaskManager::listDetailed()
{
    InterfaceComposition::tabStatsCreation(tasks);
}

//make better;easy way to do so
bool TaskManager::duplicateCheck(const char* duplicateArg)

{
    for (const auto& task : tasks) {

        if (task.description == std::string_view(duplicateArg))
        {
            return Interface::userWantsDuplicate();
        }
    }

    return true;
}







