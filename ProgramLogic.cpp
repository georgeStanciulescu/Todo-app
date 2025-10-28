#include <ProgramLogic.h>
#include <TaskManager.h>
#include <ErrorHandling.h>

#include "Interface.h"

ProgramResult programLogic(const int argc,char* argv[])
{
    using enum TaskManager::DetailType;
    using enum ProgramResult;

    TaskManager tasker{};
    ErrorHandling validator{tasker};

    if (!validator.programStartHandle(argc)){return failure;};

    std::string_view input{argv[1]};

    auto choice = commandChoice(input);
    if (!choice)
    {
        extraDetail(basic);
        return failure;
    }

    TaskManager::DetailType cmd{*choice};

    switch (cmd) {
        case list:
            tasker.listTasks();
            break;
        case add:
            if (!validator.addErrorHandle(argc)){return failure;}
            if (!validator.duplicateCheck(argv[2])){return failure;}

            tasker.addTask(argv,argc);
            successMessage(add);
            break;
        case deletion:
            if (!validator.deleteErrorHandle(argc,argv)){return failure;}

            tasker.deleteTask(argv[2]) == TaskManager::all ? successMessage(deletion,true) :successMessage(deletion);

            break;
        case end:
            if (!validator.endErrorHandle(argc,argv)){return failure;}

            tasker.endTask(argv[2],argv[3]);
            successMessage(end);
            break;
        case change:
            if (!validator.changeErrorHandle(argc,argv)){return failure;}

            tasker.changeTask(argv[2]);
            successMessage(change);
            break;
        case info:
            extraDetail(info);
            break;
        case basic:
            extraDetail(basic);
            break;
    }

    return success;
}

std::optional<TaskManager::DetailType> commandChoice(std::string_view userInput)
{
    using enum TaskManager::DetailType;
    if (userInput == "list"){return list;}
    if (userInput == "add"){return add;}
    if (userInput == "delete"){return deletion;}
    if (userInput == "end"){return end;}
    if (userInput == "change"){return change;}
    if (userInput == "info"){return info;}
    if (userInput == "options"){return basic;}

    return {};
}