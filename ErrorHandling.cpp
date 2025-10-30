#include <ErrorHandling.h>
#include <Constants.h>

#include "Interface.h"


bool ErrorHandling::endErrorHandle(const int totalArgs,char* endArgs[]) const
{
    if (totalArgs < Constants::minEndArgs) {return Interface::errorResponse(TaskManager::end);}

    if (!boundaryCheck(endArgs[Constants::thirdArg])) {return false;}

    if (std::string_view(endArgs[Constants::fourthArg]).size() != 1) {return Interface::errorResponse(TaskManager::end);}

    char statusType(endArgs[Constants::fourthArg][0]);

    if (statusType != Constants::successMark && statusType != Constants::failureMark) {return Interface::errorResponse(TaskManager::end);}

    return true;
}

bool ErrorHandling::deleteErrorHandle(const int totalArgs,char* deletionArg[]) const
{
    if (totalArgs < Constants::minGeneralArgs) {return Interface::errorResponse(TaskManager::deletion);}

    if (std::string_view(deletionArg[Constants::thirdArg]) == "all"){return true;}

    if (!boundaryCheck(deletionArg[Constants::thirdArg])){return false;}

    return true;
}

bool ErrorHandling::addErrorHandle(const int totalArgs) const
{
    if (totalArgs < Constants::minGeneralArgs) {return Interface::errorResponse(TaskManager::add);}

    return true;
}

bool ErrorHandling::changeErrorHandle(const int totalArgs,char* changeArg[]) const
{
    if (totalArgs < Constants::minGeneralArgs) {return Interface::errorResponse(TaskManager::change);}

    if (!boundaryCheck(changeArg[Constants::thirdArg])){return false;}

    return true;
}

bool ErrorHandling::boundaryCheck(const char* boundaryArg) const
{
    try
    {
        const int taskID{std::stoi(boundaryArg)};
        if (taskID < 1 || static_cast<std::size_t>(taskID) > taskManager.getTasks().size())
        {
            Interface::exceptionErrorMessage(outOfRange);
            return false;
        }
    } catch (const std::invalid_argument&) {
        Interface::exceptionErrorMessage(outOfRange);
         return false;
    } catch (const std::out_of_range&) {
        Interface::exceptionErrorMessage(outOfRange);
        return false;
    }

    return true;
}

bool ErrorHandling::programStartHandle(const int totalArgs) const
{
    if (totalArgs < Constants::minStartArgs) {return Interface::errorResponse(TaskManager::basic);}
    return true;
}



