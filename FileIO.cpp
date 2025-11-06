#include <FileIO.h>
#include <Constants.h>
#include <Calculations.h>
#include <Interface.h>
#include <filesystem>
#include <fstream>
#include <TaskManager.h>


namespace IO
{
    void fileIO(std::vector<TaskManager::Task> &tasks)
    {

        if (!std::filesystem::exists(Constants::fileName)) {
            std::ofstream fileCreation(static_cast<std::string>(Constants::fileName));
            if (!fileCreation) {
                throw std::runtime_error(
                    "Error : The file has failed to be created!");
            }
        }

        std::ifstream myFile(static_cast<std::string>(Constants::fileName));

        if (!myFile) {throw std::runtime_error("Error : The file has failed to open!");}

        int count{0};
        std::string myTask{};

        while (std::getline(myFile, myTask))
        {
            ++count;
            TaskManager::Task task{taskReader(myTask,count)};
            if (task.completion == Constants::ongoingMark){recalculateDaysLeft(task);}
            tasks.push_back(task);
            // tasks.push_back(taskReader(myTask,count));
        }
    }

    TaskManager::Task taskReader(std::string& myTask,int count)
    {
        std::istringstream stream{myTask};
        std::string garbage{};

        int id{};
        if (!(stream >> id)){Interface::exceptionErrorMessage(ErrorHandling::id,count);}

        if (!std::getline(stream,garbage,Constants::pipeDelimiter))
         {
            Interface::exceptionErrorMessage(ErrorHandling::status,count);
        }
        char status{};
        if (!(stream >> status)){Interface::exceptionErrorMessage(ErrorHandling::status,count);}

        std::string startDate{};
        if (!std::getline(stream,garbage,Constants::pipeDelimiter) || !std::getline(stream,startDate,Constants::pipeDelimiter))
        {
            Interface::exceptionErrorMessage(ErrorHandling::startDate,count);
        }
        std::string dueDate{};

        if (!std::getline(stream,dueDate,Constants::pipeDelimiter))
        {
            Interface::exceptionErrorMessage(ErrorHandling::dueDate,count);
        }
        std::string presentToDueDate{};
        if (!std::getline(stream,presentToDueDate,Constants::pipeDelimiter))
        {
            Interface::exceptionErrorMessage(ErrorHandling::daysLeft,count);
        }
        std::string endDate{};
        if (!std::getline(stream,endDate,Constants::pipeDelimiter))
        {
            Interface::exceptionErrorMessage(ErrorHandling::endDate,count);
        }
        std::string startDateEndDateDifference{};
        if (!std::getline(stream,startDateEndDateDifference,Constants::pipeDelimiter))
        {
            Interface::exceptionErrorMessage(ErrorHandling::startDateEndDateDifference,count);
        }
        std::string dueDateEndDateDifference{};
        if (!std::getline(stream,dueDateEndDateDifference,Constants::pipeDelimiter))
        {
            Interface::exceptionErrorMessage(ErrorHandling::dueDateEndDateDifference,count);
        }
        std::string description{};
        if (!std::getline(stream,description))
        {
            Interface::exceptionErrorMessage(ErrorHandling::description,count);
        }

        return {id,description,status,startDate,dueDate,
      presentToDueDate,endDate,startDateEndDateDifference,dueDateEndDateDifference};
    }

    void deleteTaskIO(std::vector<TaskManager::Task> &tasks,const char *taskID)
    {
        using namespace Constants;

        std::filesystem::path tempPath{"temp.txt"};
        std::ofstream tempFile(tempPath);

        const int chosenTask{std::stoi(taskID) - 1};

        tasks.erase(tasks.begin() + chosenTask);

        for (std::size_t x{0}; x < tasks.size(); ++x)
        {
            tempFile  << x + 1 << pipeDelimiter <<  tasks[x].completion << pipeDelimiter
                      << tasks[x].date << pipeDelimiter << tasks[x].dueDate << pipeDelimiter
                      << tasks[x].daysLeft << pipeDelimiter << tasks[x].endDate
                      << pipeDelimiter <<tasks[x].startDateEndDateDifference << pipeDelimiter
                      << tasks[x].dueDateEndDateDifference << pipeDelimiter << tasks[x].description << '\n';
        }

        std::filesystem::rename(tempPath,fileName);
    }

    void deleteAllTasksIO() {
        std::filesystem::remove(static_cast<std::string>(Constants::fileName).c_str());
    }

    void endTaskIO(const std::vector<TaskManager::Task> &tasks,const char *taskID, const char *status,
                   const std::string& startDateString,const std::string& dueDateString)
    {
        using namespace Constants;

        std::filesystem::path tempPath{"temp.txt"};
        std::ofstream tempFile(tempPath);

        DateInformation::DayMonthYear taskEndDate{endDateDropdown(startDateString)};
        const auto taskEndDateString{std::format("{}/{}/{}",taskEndDate.day,taskEndDate.month,taskEndDate.year)};

        const auto taskStartDate = returnNumericDate(startDateString);
        const auto taskDueDate = returnNumericDate(dueDateString);

        int startEndDifference{returnDateDifference(taskStartDate,taskEndDate)};
        int dueEndDifference{std::abs(returnDateDifference(taskDueDate,taskEndDate))};

        const int chosenTask{std::stoi(taskID)};

        const std::string_view endedTask{tasks[chosenTask - 1].description};

        for (const auto &task: tasks)
        {
            if (task.id != chosenTask) {
                tempFile  << task.id << pipeDelimiter << task.completion << pipeDelimiter
                          << task.date << pipeDelimiter << task.dueDate << pipeDelimiter
                          << task.daysLeft << pipeDelimiter << task.endDate<< pipeDelimiter
                          << task.startDateEndDateDifference << pipeDelimiter
                          << task.dueDateEndDateDifference << pipeDelimiter << task.description << '\n';
            }
            else
            {
                tempFile << task.id << pipeDelimiter << status << pipeDelimiter
                         << task.date << pipeDelimiter << task.dueDate
                         << pipeDelimiter << task.daysLeft << pipeDelimiter
                         << taskEndDateString << pipeDelimiter << startEndDifference
                         << pipeDelimiter << dueEndDifference << pipeDelimiter << endedTask << '\n';
            }
        }

        std::filesystem::rename(tempPath,fileName);
    }

    void addTaskIO(const std::vector<TaskManager::Task> &tasks, char *argv[],int argc)
    {
        using namespace Constants;
        //std::filesystem::path tempPath{"temp.txt"};
        std::ofstream tempFile(static_cast<std::string>(fileName), std::ios::app);

        auto dates = dateDropdown();

        auto startDate = std::format("{}/{}/{}",dates[0].day ,dates[0].month ,dates[0].year);
        auto endDate = std::format("{}/{}/{}",dates[1].day,dates[1].month ,dates[1].year);

        //int daysLeft {presentToDueDate(dates[1])};

        tempFile << tasks.size() + 1 << pipeDelimiter << ongoingMark <<  pipeDelimiter << startDate
                 <<  pipeDelimiter << endDate <<  pipeDelimiter << ' ' <<  pipeDelimiter
                 << ' ' <<  pipeDelimiter << ' ' <<  pipeDelimiter << ' ' << pipeDelimiter;

        for (int x{2}; x < argc; ++x) {
            tempFile << argv[x] << " ";
        }
        tempFile << '\n';

        //std::filesystem::rename(tempPath,Constants::fileName);
    }


    void changeTaskIO(std::vector<TaskManager::Task> &tasks,const char *taskID)
    {

        using namespace Constants;
        std::filesystem::path tempPath{"temp.txt"};
        std::ofstream tempFile(tempPath);

        int chosenTask{std::stoi(taskID) - 1};
        std::string descriptionToChange{tasks[chosenTask].description};

        Interface::changeTaskInput(descriptionToChange);

        tasks[chosenTask].description = descriptionToChange;

        for (const auto &task: tasks)
        {
            tempFile << task.id << pipeDelimiter << task.completion << pipeDelimiter << task.date
                     << pipeDelimiter << task.dueDate << pipeDelimiter  << task.daysLeft
                     << pipeDelimiter << task.endDate << pipeDelimiter << task.startDateEndDateDifference
                     << pipeDelimiter << task.dueDateEndDateDifference << pipeDelimiter << task.description << '\n';
        }

        std::filesystem::rename(tempPath,fileName);
    }

    std::vector<std::string> returnDateSubstrings(const std::string& date)
    {
        const std::size_t dayEnd{date.find_first_of('/')};
        const std::size_t monthEnd{date.find_last_of('/')};

        return { date.substr(0,dayEnd),
                    date.substr(dayEnd + 1,monthEnd - dayEnd - 1),
                    date.substr(monthEnd + 1)};
    }

    DateInformation::DayMonthYear returnNumericDate(const std::string& date)
    {
        const auto  dateSubstrings= returnDateSubstrings(date);
        return { std::stoi(dateSubstrings[0]),std::stoi(dateSubstrings[1]),std::stoi(dateSubstrings[2])};
    }

    void recalculateDaysLeft(std::vector<TaskManager::Task>& tasks)
    {
        for (auto& task : tasks)
        {
            std::vector dueDateSubstrings{returnDateSubstrings(task.dueDate)};
            DateInformation::DayMonthYear dueDate{std::stoi(dueDateSubstrings[0]),std::stoi(dueDateSubstrings[1]),std::stoi(dueDateSubstrings[2])};
            task.daysLeft = std::format("{}",presentToDueDate(dueDate));
        }

    }
    void recalculateDaysLeft(TaskManager::Task& task)
    {

        std::vector dueDateSubstrings{returnDateSubstrings(task.dueDate)};
        DateInformation::DayMonthYear dueDate{std::stoi(dueDateSubstrings[0]),std::stoi(dueDateSubstrings[1]),std::stoi(dueDateSubstrings[2])};
        task.daysLeft = std::format("{}",presentToDueDate(dueDate));

    }

}

