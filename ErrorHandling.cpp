#include "ErrorHandling.h"

bool ErrorHandling::endErrorHandle(int argc,char* argv[]) const
{
    if (argc <4)
    {
        taskManager.extraDetail(TaskManager::end);
        return false;
    }
    if (!boundaryCheck(argv[2])){return false;}

    const std::string_view statusType{(argv[3])};

    if (statusType != "s" && statusType != "x")
    {
        taskManager.extraDetail(TaskManager::end);
        return false;
    }

    return true;
}

bool ErrorHandling::deleteErrorHandle(int argc,char* argv[]) const
{
    if (argc <3)
    {
        taskManager.extraDetail(TaskManager::deletion);
        return false;
    }
    const std::string_view deletionType{argv[2]};

    if (deletionType == "all"){return true;}

    if (!boundaryCheck(argv[2])){return false;}

    return true;
}

bool ErrorHandling::addErrorHandle(int argc) const
{
    if (argc <3)
    {
        taskManager.extraDetail(TaskManager::add);
        return false;
    }

    return true;
}

bool ErrorHandling::changeErrorHandle(int argc,char* argv[]) const
{
    if (argc <3)
    {
        taskManager.extraDetail(TaskManager::change);
        return false;
    }

    if (!boundaryCheck(argv[2])){return false;}

    return true;
}

bool ErrorHandling::duplicateCheck(char* argv[]) const
{

    bool isDuplicate{false};

    const std::string taskName{argv[2]};

    for (const auto& task : taskManager.getTasks()) {
        if (task.description == taskName){isDuplicate = true;}
    }

    if (isDuplicate)
    {
        char duplicateAnswer{};
        std::cout << "There seems to be a task with the name just entered.Would you like to another task with the same name?(Y/n)";
        std::cin >> duplicateAnswer;

        if (duplicateAnswer == 'n' || duplicateAnswer == 'N' ){return true;}
        else if (duplicateAnswer == 'Y' || duplicateAnswer == 'y'){return false;}
    }

    return false;
}

bool ErrorHandling::boundaryCheck(const char* argv) const
{
    try
    {
        const int taskID{std::stoi(argv)};

        if (taskID < 1 || taskID > taskManager.getTasks().size())
        {
            std::cout << "The task ID cannot be found! Introduce an ID for a task in the list!" << "\n";
            return false;
        }
    } catch (const std::exception&) {

        std::cout << "You must enter the ID of the task;anything else will lead to an error!" << "\n";
        return false;

    }

    return true;
}