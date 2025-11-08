#include <ProgramLogic.h>
#include <ErrorHandling.h>
#include <Interface.h>

#include "Constants.h"

ProgramResult programLogic(const int argc,char* argv[])
{
    using enum TaskManager::DetailType;
    using enum ProgramResult;
    using namespace Interface;

    TaskManager tasker{};
    ErrorHandling validator{};

    if (!validator.minArgs(argc,ErrorHandling::ArgCheck::min)) {
        errorResponse(basic);
        return failure;
    }

    const auto taskNumber = static_cast<int>(tasker.getTasks().size());
    const std::string_view input{argv[1]};

    const auto choice = commandChoice(input);
    if (!choice)
    {
        extraDetail(basic);
        return failure;
    }

    switch (*choice) {
        case list:

            if (!validator.minArgs(argc,ErrorHandling::ArgCheck::general)) {
                tasker.listSimple();
                return success;
            }
            if (!validator.stringArgCheck(argv[2],ErrorHandling::CommandType::list)){return failure;}

            tasker.listDetailed();
            break;
        case add:
            //if (!validator.addErrorHandle(argc)){return failure;}
            if (!validator.minArgs(argc,ErrorHandling::ArgCheck::general)) {
                errorResponse(add);
                return failure;
            }

            if (!tasker.duplicateCheck(argv[2])){return failure;}

            tasker.addTask(argv,argc);
            successMessage(add);
            break;
        case deletion: {
            //if (!validator.deleteArgNumber(argc)){return failure;}
            if (!validator.minArgs(argc,ErrorHandling::ArgCheck::general)) {
                errorResponse(deletion);
                return failure;
            }
            if (validator.stringArgCheck(argv[2],ErrorHandling::CommandType::deletion)) {
                tasker.deleteAllTasks();
                successMessage(deletion,TaskManager::DeletionType::all);
                return success;
            }

            const auto ids = validator.argBreakdown(argv[2],taskNumber);

            tasker.deleteTasks(ids);
            if (ids.size() == 1){successMessage(deletion,TaskManager::DeletionType::single);}
            else{successMessage(deletion,TaskManager::DeletionType::multiple);}
            break;
        }
        case end: {
            if (!validator.minArgs(argc,ErrorHandling::ArgCheck::max)) {
                errorResponse(end);
                return failure;
            }
            const auto endIDs = validator.argBreakdown(argv[2],taskNumber);
            if (!validator.stringArgCheck(std::string_view(argv[3]),
                ErrorHandling::CommandType::end)){return failure;}
            if (*argv[3] != Constants::successMark && *argv[3] != Constants::failureMark) {
                errorResponse(end);
                return failure;
            }

            if (endIDs.size() == 1) {
                const std::string startDate{tasker.getStartDate(endIDs[0] - 1)};
                const std::string dueDate{tasker.getDueDate(endIDs[0] - 1)};

                tasker.endTask(endIDs[0],argv[3],startDate,dueDate);
                successMessage(end);
            }
            else{ return failure;}

            break;
        }
        case change: {
            if (!validator.minArgs(argc,ErrorHandling::ArgCheck::general)) {
                errorResponse(change);
                return failure;
            }
            const auto changeIDs = validator.argBreakdown(argv[2],taskNumber);

            //this form is temporary,since I don't yet have multi-task change functionality
            if (changeIDs.size() == 1) {
                tasker.changeTask(changeIDs[0]);
                successMessage(change);
            }
            else {
                errorResponse(change);
                return failure;
            }
            break;
        }
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