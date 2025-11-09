#ifndef UNTITLED_PROGRAMLOGIC_H
#define UNTITLED_PROGRAMLOGIC_H

#include <optional>
#include <TaskManager.h>

enum class ProgramResult
{
    success = 0,
    failure = 1,
};

std::optional<TaskManager::DetailType> commandChoice(std::string_view userInput);
ProgramResult programLogic(int argc,char* argv[]);
ProgramResult listLogic(TaskManager& tasker,int argc,char* argv[]);
ProgramResult addLogic(TaskManager& tasker,int argc,char* argv[]);
ProgramResult endLogic(TaskManager& tasker,int argc,int taskNumber,char* argv[]);
ProgramResult deleteLogic(TaskManager& tasker,int argc,int taskNumber,char* argv[]);
ProgramResult changeLogic(TaskManager& tasker,int argc,int taskNumber,char* argv[]);

#endif //UNTITLED_PROGRAMLOGIC_H