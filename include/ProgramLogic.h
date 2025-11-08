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

#endif //UNTITLED_PROGRAMLOGIC_H