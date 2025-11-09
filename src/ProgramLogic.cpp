#include <ProgramLogic.h>
#include <ErrorHandling.h>
#include <Interface.h>

#include "Constants.h"

//The separation of concerns is not the greatest;yet,I don't feel like restructuring

ProgramResult programLogic(const int argc,char* argv[])
{
    using enum TaskManager::DetailType;
    using enum ProgramResult;
    using namespace Interface;

    TaskManager tasker{};

    if (!minArgs(argc,ErrorHandling::ArgCheck::min)) {
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

    switch (*choice)
    {
        case list:
            listLogic(tasker,argc,argv);
            break;
        case add:
            addLogic(tasker,argc,argv);
            break;
        case deletion:
            deleteLogic(tasker,argc,taskNumber,argv);
            break;
        case end:
            endLogic(tasker,argc,taskNumber,argv);
            break;
        case change:
            changeLogic(tasker,argc,taskNumber,argv);
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

//maybe use an unordered map
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

ProgramResult listLogic(TaskManager& tasker,const int argc,char* argv[])
{
    if (!minArgs(argc,ErrorHandling::ArgCheck::general)) {
        tasker.listSimple();
        return ProgramResult::success;
    }
    if (!stringArgCheck(argv[2],ErrorHandling::CommandType::list)){return ProgramResult::failure;}

    tasker.listDetailed();
    return ProgramResult::success;
}


ProgramResult addLogic(TaskManager& tasker,const int argc,char* argv[])
{
    if (!minArgs(argc,ErrorHandling::ArgCheck::general)) {
        Interface::errorResponse(TaskManager::DetailType::add);
        return ProgramResult::failure;
    }

    if (!tasker.duplicateCheck(argv[2])){return ProgramResult::failure;}

    tasker.addTask(argv,argc);
    Interface::successMessage(TaskManager::DetailType::add);
    return ProgramResult::success;

}

ProgramResult endLogic(TaskManager& tasker,const int argc,const int taskNumber,char* argv[])
{
    if (!minArgs(argc,ErrorHandling::ArgCheck::max)) {
        Interface::errorResponse(TaskManager::DetailType::end);
        return ProgramResult::failure;
    }
    const auto endParseResult = ErrorHandling::argBreakdown(argv[2],taskNumber);

    if (endParseResult.errorType)
    {
        Interface::exceptionErrorMessage(*endParseResult.errorType,0);
        return ProgramResult::failure;
    }

    if (!stringArgCheck(std::string_view(argv[3]),
        ErrorHandling::CommandType::end)){return ProgramResult::failure;}

    if (*argv[3] != Constants::successMark && *argv[3] != Constants::failureMark) {
        Interface::errorResponse(TaskManager::DetailType::end);
        return ProgramResult::failure;
    }

    if (endParseResult.ids.size() == 1) {
        const std::string startDate{tasker.getStartDate(endParseResult.ids[0] - 1)};
        const std::string dueDate{tasker.getDueDate(endParseResult.ids[0] - 1)};

        tasker.endTask(endParseResult.ids[0],argv[3],startDate,dueDate);
        Interface::successMessage(TaskManager::DetailType::end);
    }
    else
    {
        return ProgramResult::failure;
    }

    return ProgramResult::success;
}

ProgramResult deleteLogic(TaskManager& tasker,const int argc,const int taskNumber,char* argv[])
{
    if (!minArgs(argc,ErrorHandling::ArgCheck::general)) {
        Interface::errorResponse(TaskManager::DetailType::deletion);
        return ProgramResult::failure;
    }
    if (stringArgCheck(argv[2],ErrorHandling::CommandType::deletion)) {
        tasker.deleteAllTasks();
        Interface::deletionMessage(TaskManager::DeletionType::all);
        return ProgramResult::success;
    }

    const auto deleteParseResult = ErrorHandling::argBreakdown(argv[2],taskNumber);

    if (deleteParseResult.errorType)
    {
        Interface::exceptionErrorMessage(*deleteParseResult.errorType,0);
        return ProgramResult::failure;
    }

    tasker.deleteTasks(deleteParseResult.ids);
    if (deleteParseResult.ids.size() == 1){Interface::deletionMessage(TaskManager::DeletionType::single);}
    else {
        Interface::deletionMessage(TaskManager::DeletionType::multiple);
    }

    return ProgramResult::success;
}

ProgramResult changeLogic(TaskManager& tasker,const int argc,const int taskNumber,char* argv[])
{
    if (!minArgs(argc,ErrorHandling::ArgCheck::general)) {
        Interface::errorResponse(TaskManager::DetailType::change);
        return ProgramResult::failure;
    }
    const auto changeParseResult = ErrorHandling::argBreakdown(argv[2],taskNumber);

    if (changeParseResult.errorType)
    {
        Interface::exceptionErrorMessage(*changeParseResult.errorType,0);
        return ProgramResult::failure;
    }

    //this form is temporary,since I don't yet have multi-task change functionality
    if (changeParseResult.ids.size() == 1) {
        tasker.changeTask(changeParseResult.ids[0]);
        Interface::successMessage(TaskManager::DetailType::change);
    }
    else {
        Interface::errorResponse(TaskManager::DetailType::change);
        return ProgramResult::failure;
    }

    return ProgramResult::success;
}