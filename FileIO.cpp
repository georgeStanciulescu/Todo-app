#include <FileIO.h>
#include <Constants.h>
#include <Calculations.h>
#include <Interface.h>
#include <filesystem>
#include <fstream>
#include <TaskManager.h>
#include <unordered_set>


namespace IO
{
    void fileIO(std::vector<TaskManager::Task> &tasks)
    {

        if (!std::filesystem::exists(Constants::fileName)) {
            std::ofstream fileCreation(static_cast<std::string>(Constants::fileName));
            if (!fileCreation) {
                Interface::exceptionErrorMessage(ErrorHandling::failedCreation,0);
            }
        }

        std::ifstream myFile(static_cast<std::string>(Constants::fileName));

        if (!myFile) {
            Interface::exceptionErrorMessage(ErrorHandling::failedOpening,0);;
        }

        int count{0};
        std::string myTask{};

        while (std::getline(myFile, myTask))
        {
            ++count;
            TaskManager::Task task{taskReader(myTask,count)};

            if (task.completion == Constants::ongoingMark) {
                const auto dueDateInt{returnNumericDate(task.dueDate)};
                task.daysLeft = std::format("{}",presentToDueDate(dueDateInt));
            }

            tasks.push_back(task);
        }
    }

    TaskManager::Task taskReader(const std::string& myTask,int count)
    {
        std::istringstream stream{myTask};

        std::string status{};
        if ( !std::getline(stream,status,Constants::pipeDelimiter)) {
            Interface::exceptionErrorMessage(ErrorHandling::status,count);
        }

        std::string startDate{};
        if (!std::getline(stream,startDate,Constants::pipeDelimiter))
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

        return {count,description,status[0],startDate,dueDate,
      presentToDueDate,endDate,startDateEndDateDifference,dueDateEndDateDifference};
    }


    void deleteAllTasksIO() {
        std::ofstream{static_cast<std::string>(Constants::fileName), std::ios::trunc};
    }

    void deleteTasksIO(std::vector<TaskManager::Task> &tasks,const std::vector<int>& tasksIDs)
    {
        auto ids{tasksIDs};
        std::ranges::sort(ids);

        int deletionCounter{-1};
        for (const auto& id :ids) {
            tasks.erase(tasks.begin() + (id+deletionCounter));
            --deletionCounter;
        }

        writeToFile(tasks);
    }

    void endTaskIO(std::vector<TaskManager::Task> &tasks,const char *taskID, const char *status,
                   const std::string& startDateString,const std::string& dueDateString)
    {
        const auto taskEndDate{endDateDropdown(startDateString)};
        const auto taskEndDateString{std::format("{}/{}/{}",taskEndDate.day,taskEndDate.month,taskEndDate.year)};

        const auto taskStartDate = returnNumericDate(startDateString);
        const auto taskDueDate = returnNumericDate(dueDateString);

        const auto startEndDifference{returnDateDifference(taskStartDate,taskEndDate)};
        const auto dueEndDifference{std::abs(returnDateDifference(taskDueDate,taskEndDate))};

        const auto chosenTask{std::stoi(taskID) - 1};

        tasks[chosenTask].completion = *status;
        tasks[chosenTask].endDate = taskEndDateString;
        tasks[chosenTask].startDateEndDateDifference = std::to_string(startEndDifference);
        tasks[chosenTask].dueDateEndDateDifference = std::to_string(dueEndDifference);

        writeToFile(tasks);
    }

    void addTaskIO(std::vector<TaskManager::Task> &tasks, char *argv[],int argc)
    {
        using namespace Constants;

        const auto dates = dateDropdown();

        const auto startDate = std::format("{}/{}/{}",dates[0].day ,dates[0].month ,dates[0].year);
        const auto endDate = std::format("{}/{}/{}",dates[1].day,dates[1].month ,dates[1].year);

        std::string description{};
        for (int x{2}; x < argc; ++x) {
            description += argv[x];
            description += " ";
        }

        tasks.emplace_back(tasks.size() + 1,description,ongoingMark,startDate,endDate,
                    "","","","");

        writeToFile(tasks);
    }

    void changeTaskIO(std::vector<TaskManager::Task> &tasks,const char *taskID)
    {
        const auto chosenTask{std::stoi(taskID) - 1};

        std::string descriptionToChange{tasks[chosenTask].description};

        Interface::changeTaskInput(descriptionToChange);

        tasks[chosenTask].description = descriptionToChange;

        writeToFile(tasks);
    }

    std::vector<std::string> returnDateSubstrings(const std::string& date)
    {
        const auto dayEnd{date.find_first_of('/')};
        const auto monthEnd{date.find_last_of('/')};

        return { date.substr(0,dayEnd),
                    date.substr(dayEnd + 1,monthEnd - dayEnd - 1),
                    date.substr(monthEnd + 1)};
    }

    DateInformation::DayMonthYear returnNumericDate(const std::string& date)
    {
        const auto  dateSubstrings= returnDateSubstrings(date);
        return { std::stoi(dateSubstrings[0]),std::stoi(dateSubstrings[1]),std::stoi(dateSubstrings[2])};
    }

    void writeToFile(const std::vector<TaskManager::Task>& tasks)
    {
        using namespace Constants;

        std::filesystem::path tempPath{"temp.txt"};
        std::ofstream tempFile(tempPath);

        for (const auto &task: tasks)
        {
            tempFile << task.completion << pipeDelimiter << task.date
                     << pipeDelimiter << task.dueDate << pipeDelimiter  << task.daysLeft
                     << pipeDelimiter << task.endDate << pipeDelimiter << task.startDateEndDateDifference
                     << pipeDelimiter << task.dueDateEndDateDifference << pipeDelimiter << task.description << '\n';
        }

        std::filesystem::remove(fileName);
        std::filesystem::rename(tempPath,fileName);

    }

}

