#ifndef UNTITLED_FILEIO_H
#define UNTITLED_FILEIO_H

#include <vector>
#include <TaskManager.h>
#include "Date.h"

namespace IO
{
    void fileIO(std::vector<TaskManager::Task>& tasks);
    TaskManager::Task taskReader(const std::string& myTask,int count);
    void deleteTaskIO(std::vector<TaskManager::Task>& tasks,const char* taskID);
    void deleteAllTasksIO();
    void deleteTasksIO(std::vector<TaskManager::Task> &tasks,const std::vector<int>& tasksIDs);
    void endTaskIO(std::vector<TaskManager::Task>& tasks,int taskID,const char* status,const std::string& startDateString,const std::string& dueDateString);
    void addTaskIO(std::vector<TaskManager::Task>& tasks,char* argv[],int argc);
    void changeTaskIO(std::vector<TaskManager::Task>& tasks,int taskID);
    std::vector<std::string> returnDateSubstrings(const std::string& date);
    DateInformation::DayMonthYear returnNumericDate(const std::string& date);
    void writeToFile(const std::vector<TaskManager::Task>& tasks);

}


#endif //UNTITLED_FILEIO_H