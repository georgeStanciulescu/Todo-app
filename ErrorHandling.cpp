#include <ErrorHandling.h>
#include <Constants.h>
#include <format>
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


bool ErrorHandling::deleteArgNumber(const int totalArgs) const
{
    if (totalArgs < Constants::minGeneralArgs) {return Interface::errorResponse(TaskManager::DetailType::deletion);}

    return true;
}

bool ErrorHandling::deleteAllCheck(char* deletionArg) const
{
    if (std::string_view(deletionArg) == "all"){return true;}

    return false;
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
//might delete,and use the other function at the bottom
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
    } catch (const std::exception&) {
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

std::vector<int> ErrorHandling::deleteArgBreakdown(const char* deleteArg) const
{
    const std::size_t count {taskManager.getTasks().size() };

    std::istringstream stream{deleteArg};
    std::string id{};
    std::vector<int> ids{};
    while (std::getline(stream,id,','))
    {
        try {
            const int val{std::stoi(id)};

            if (val < 1 || static_cast<std::size_t>(val) > count) {
                Interface::exceptionErrorMessage(outOfRange,0);
                return {};
            }

            ids.push_back(val);
        } catch (const std::exception&) {
            Interface::exceptionErrorMessage(ErrorHandling::incorrectArg,0);
            return {};
        }

    }
    std::cout << ids.size() << '\n';
    return ids;
}





