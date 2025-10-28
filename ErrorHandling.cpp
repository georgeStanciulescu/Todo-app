#include <ErrorHandling.h>
#include <Constants.h>

#include "Interface.h"


bool ErrorHandling::endErrorHandle(const int totalArgs,char* endArgs[]) const
{
    if (totalArgs < Constants::minEndArgs) {return errorResponse(TaskManager::end);}

    if (!boundaryCheck(endArgs[Constants::thirdArg])) {return false;}

    if (std::string_view(endArgs[Constants::fourthArg]).size() != 1) {return errorResponse(TaskManager::end);}

    char statusType(endArgs[Constants::fourthArg][0]);

    if (statusType != Constants::successMark && statusType != Constants::failureMark) {return errorResponse(TaskManager::end);}

    return true;
}

bool ErrorHandling::deleteErrorHandle(const int totalArgs,char* deletionArg[]) const
{
    if (totalArgs < Constants::minGeneralArgs) {return errorResponse(TaskManager::deletion);}

    if (std::string_view(deletionArg[Constants::thirdArg]) == "all"){return true;}

    if (!boundaryCheck(deletionArg[Constants::thirdArg])){return false;}

    return true;
}

bool ErrorHandling::addErrorHandle(const int totalArgs) const
{
    if (totalArgs < Constants::minGeneralArgs) {return errorResponse(TaskManager::add);}

    return true;
}

bool ErrorHandling::changeErrorHandle(const int totalArgs,char* changeArg[]) const
{
    if (totalArgs < Constants::minGeneralArgs) {return errorResponse(TaskManager::change);}

    if (!boundaryCheck(changeArg[Constants::thirdArg])){return false;}

    return true;
}

bool ErrorHandling::duplicateCheck(const char* duplicateArg)
{
    bool isDuplicate{false};

    for (const auto& task : taskManager.getTasks()) {
        if (task.description == duplicateArg){isDuplicate = true;}
    }

    return userWantsDuplicate(isDuplicate);
}

bool ErrorHandling::userWantsDuplicate(const bool isDuplicate)
{
    if (isDuplicate)
    {
        char duplicateAnswer{};
        std::cout << "There seems to be a task with the name just entered.Would you like to another task with the same name?(Y/n)";
        std::cin >> duplicateAnswer;

        if (duplicateAnswer == 'n' || duplicateAnswer == 'N' ){return false;}
        if (duplicateAnswer == 'Y' || duplicateAnswer == 'y'){return true;}
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
            std::cout << "The task ID cannot be found! Introduce an ID for a task in the list!" << '\n';
            return false;
        }
    } catch (const std::invalid_argument&) {
         std::cout << "You must enter the ID of the task;anything else will lead to an error!" << '\n';
         return false;
    } catch (const std::out_of_range&) {
        std::cout << "The entered ID is out of range!" << '\n';
        return false;
    }

    return true;
}

bool ErrorHandling::programStartHandle(const int totalArgs) const
{
    if (totalArgs < Constants::minStartArgs) {return errorResponse(TaskManager::basic);}
    return true;
}

bool ErrorHandling::errorResponse(const TaskManager::DetailType type) const
{
    extraDetail(type);
    return false;
}

