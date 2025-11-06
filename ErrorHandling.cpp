#include <ErrorHandling.h>
#include <Constants.h>
#include <iostream>

#include "Interface.h"


bool ErrorHandling::endErrorHandle(const int totalArgs,char* endArgs[]) const
{
    if (totalArgs < Constants::minEndArgs) {return Interface::errorResponse(TaskManager::DetailType::end);}

    if (!boundaryCheck(endArgs[Constants::thirdArg])) {return false;}

    if (std::string_view(endArgs[Constants::fourthArg]).size() != 1) {return Interface::errorResponse(TaskManager::DetailType::end);}

    char statusType(endArgs[Constants::fourthArg][0]);

    if (statusType != Constants::successMark && statusType != Constants::failureMark) {return Interface::errorResponse(TaskManager::DetailType::end);}

    return true;
}

bool ErrorHandling::deleteErrorHandle(const int totalArgs,char* deletionArg[]) const
{
    if (totalArgs < Constants::minGeneralArgs) {return Interface::errorResponse(TaskManager::DetailType::deletion);}

    if (std::string_view(deletionArg[Constants::thirdArg]) == "all"){return true;}

    if (!boundaryCheck(deletionArg[Constants::thirdArg])){return false;}

    return true;
}

bool ErrorHandling::addErrorHandle(const int totalArgs) const
{
    if (totalArgs < Constants::minGeneralArgs) {return Interface::errorResponse(TaskManager::DetailType::add);}

    return true;
}

bool ErrorHandling::changeErrorHandle(const int totalArgs,char* changeArg[]) const
{
    if (totalArgs < Constants::minGeneralArgs) {return Interface::errorResponse(TaskManager::DetailType::change);}

    if (!boundaryCheck(changeArg[Constants::thirdArg])){return false;}

    return true;
}

bool ErrorHandling::listErrorHandle(const int totalArgs,char* listArgs[])
{
    if (totalArgs > 2) {
        if (std::string(listArgs[2]) != "-d"){return Interface::errorResponse(TaskManager::DetailType::list);}
    }

    return true;
}

bool ErrorHandling::boundaryCheck(const char* boundaryArg) const
{
    try
    {
        const int taskID{std::stoi(boundaryArg)};
        if (taskID < 1 || static_cast<std::size_t>(taskID) > taskManager.getTasks().size())
        {
            Interface::exceptionErrorMessage(outOfRange,0);
            return false;
        }
    } catch (const std::invalid_argument&) {
        Interface::exceptionErrorMessage(outOfRange,0);
         return false;
    } catch (const std::out_of_range&) {
        Interface::exceptionErrorMessage(outOfRange,0);
        return false;
    }

    return true;
}

bool ErrorHandling::programStartHandle(const int totalArgs) const
{
    if (totalArgs < Constants::minStartArgs) {return Interface::errorResponse(TaskManager::DetailType::basic);}
    return true;
}



