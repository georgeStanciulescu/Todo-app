#ifndef UNTITLED_FILEIO_H
#define UNTITLED_FILEIO_H

#include <vector>
#include <TaskManager.h>

#include "Date.h"

namespace IO
{
    void fileIO(std::vector<TaskManager::Task>& tasks);
    TaskManager::Task taskReader(std::string& myTask,int count);
    void deleteTaskIO(std::vector<TaskManager::Task>& tasks,const char* taskID);
    void deleteAllTasksIO();
    void endTaskIO(const std::vector<TaskManager::Task>& tasks,const char* taskID,const char* status,const std::string& startDateString,const std::string& dueDateString);
    void addTaskIO(const std::vector<TaskManager::Task>& tasks,char* argv[],int argc);
    void changeTaskIO(std::vector<TaskManager::Task>& tasks,const char* taskID);
    std::vector<std::string> returnDateSubstrings(const std::string& date);
    DateInformation::DayMonthYear returnNumericDate(const std::string& date);
    void recalculateDaysLeft(std::vector<TaskManager::Task>& tasks);
    void recalculateDaysLeft(TaskManager::Task& task);

}


#endif //UNTITLED_FILEIO_H