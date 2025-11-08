#include <ProgramLogic.h>
#include <ErrorHandling.h>
#include <Interface.h>
#include <span>

#include "Constants.h"

ProgramResult programLogic(const int argc,char* argv[])
{
    using enum TaskManager::DetailType;
    using enum ProgramResult;
    using namespace Interface;

    TaskManager tasker{};
    ErrorHandling validator{tasker};

    if (!validator.programStartHandle(argc)){return failure;};

    const std::string_view input{argv[1]};

    const auto choice = commandChoice(input);
    if (!choice)
    {
        extraDetail(basic);
        return failure;
    }

    switch (*choice) {
        case list:
            if (!validator.listErrorHandle(argc,argv)){return failure;}

            argc > 2 ?  tasker.listDetailed() : tasker.listSimple();
            break;
        case add:
            if (!validator.addErrorHandle(argc)){return failure;}
            if (!tasker.duplicateCheck(argv[2])){return failure;}

            tasker.addTask(argv,argc);
            successMessage(add);
            break;
        case deletion: {
            if (!validator.deleteArgNumber(argc)){return failure;}
            if (validator.deleteAllCheck(argv[2])) {
                tasker.deleteAllTasks();
                successMessage(deletion,TaskManager::DeletionType::all);
                return success;
            }

            const auto ids = validator.deleteArgBreakdown(argv[2]);

            if (ids.empty()){return failure;}

            tasker.deleteTasks(ids);
            if (ids.size() == 1){successMessage(deletion,TaskManager::DeletionType::single);}
            else{successMessage(deletion,TaskManager::DeletionType::multiple);}
            break;
        }
        case end: {
            if (!validator.endErrorHandle(argc,argv)){return failure;}

            const int taskID{std::stoi(argv[2]) - 1};
            const std::string startDate{tasker.getStartDate(taskID)};
            const std::string dueDate{tasker.getDueDate(taskID)};

            tasker.endTask(argv[2],argv[3],startDate,dueDate);
            successMessage(end);
            break;
        }
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

std::optional<TaskManager::DetailType> commandChoice(const std::string_view userInput)
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