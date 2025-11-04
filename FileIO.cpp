#include <FileIO.h>
#include <Constants.h>
#include <Calculations.h>
#include <Interface.h>
#include <filesystem>
#include <fstream>
#include <TaskManager.h>


namespace IO {
    void fileIO(std::vector<TaskManager::Task> &tasks) {

        if (!std::filesystem::exists(Constants::fileName)) {
            std::ofstream fileCreation(Constants::fileName);
            if (!fileCreation) {
                throw std::runtime_error(
                    "Error : The file has failed to be created!");
            }
        }

        std::ifstream myFile(Constants::fileName);

        if (!myFile) {throw std::runtime_error("Error : The file has failed to open!");}

        int count{0};
        std::string myTask{};

        while (std::getline(myFile, myTask)) {
            std::size_t openBrace{myTask.find_first_of('{')};
            std::size_t closeBrace{myTask.find_first_of('}')};
            std::size_t openBracket{myTask.find_first_of('[')};
            std::size_t closeBracket{myTask.find_first_of(']')};
            std::size_t openBraceLast{myTask.find_last_of('{')};
            std::size_t closeBraceLast{myTask.find_last_of('}')};
            std::size_t openBracketLast{myTask.find_last_of('[')};

            ++count;

            tasks.emplace_back(  count,
                          myTask.substr(openBrace     + 1,closeBrace    - openBrace    - 1),
                           myTask[closeBrace+1],
                                myTask.substr(openBracket   + 1,closeBracket  - openBracket  - 1),
                             myTask.substr(closeBracket  + 1,openBraceLast - closeBracket - 1),
                            myTask.substr(openBraceLast + 1,closeBraceLast - openBraceLast - 1),
                            myTask.substr(closeBraceLast + 1,openBracketLast - closeBraceLast - 1),
                            myTask.substr(openBracketLast + 1));
        }
        myFile.close();
    }

    void deleteTaskIO(std::vector<TaskManager::Task> &tasks,
                      const char *taskID) {

        std::filesystem::path tempPath{"temp.txt"};
        std::ofstream tempFile(tempPath);

        int chosenTask{std::stoi(taskID) - 1};

        tasks.erase(tasks.begin() + chosenTask);

        for (std::size_t x{0}; x < tasks.size(); ++x) {
            tempFile  << x + 1 << '{' << tasks[x].description << '}'
                      << tasks[x].completion << '[' << tasks[x].date << ']' <<tasks[x].dueDate << '{'
                      << tasks[x].daysLeft << '}' << tasks[x].endDate << '[' << tasks[x].startDateEndDateDifference <<  '\n';
        }

        std::filesystem::rename(tempPath,Constants::fileName);
    }

    void deleteAllTasksIO() {
        std::filesystem::remove(Constants::fileName.c_str());
    }

    void endTaskIO(const std::vector<TaskManager::Task> &tasks,
                   const char *taskID, const char *status,const std::string& startDateString) {

        std::filesystem::path tempPath{"temp.txt"};
        std::ofstream tempFile(tempPath);
        std::cout << "IT IS,as I say: " << startDateString << '\n';

        DateInformation::DayMonthYear taskEndDate{endDateDropdown(startDateString)};

        std::size_t dayEnd{startDateString.find_first_of("/")};
        std::size_t monthEnd{startDateString.find_last_of("/")};

        std::string day {startDateString.substr(0,dayEnd)};
        std::string month {startDateString.substr(dayEnd + 1,monthEnd - dayEnd - 1)};
        std::string year {startDateString.substr(monthEnd + 1)};

        const DateInformation::DayMonthYear taskStartDate{std::stoi(day),std::stoi(month),std::stoi(year)};

        const auto taskEndDateString{std::format("{}/{}/{}",taskEndDate.day,taskEndDate.month,taskEndDate.year)};

        int dayDifference{returnDateDifference(taskStartDate,taskEndDate)};

        int chosenTask{std::stoi(taskID)};

        const std::string_view endedTask{tasks[chosenTask - 1].description};

        for (const auto &task: tasks) {
            if (task.id != chosenTask) {
                tempFile  << task.id << '{' << task.description << '}' <<
                        task.completion << '[' << task.date << ']' << task.dueDate << '{' << task.daysLeft << '}' << task.endDate << '[' << task.startDateEndDateDifference << '\n';
            } else {
                tempFile << task.id << '{' << endedTask << '}' << status
                         << '[' << task.date << ']' << task.dueDate << '{' << task.daysLeft << '}' << taskEndDateString << '[' << dayDifference <<'\n';
            }
        }

        std::filesystem::rename(tempPath,Constants::fileName);
    }

    void addTaskIO(const std::vector<TaskManager::Task> &tasks, char *argv[],
                   int argc) {

        //std::filesystem::path tempPath{"temp.txt"};
        std::ofstream tempFile(Constants::fileName, std::ios::app);

        auto dates = dateDropdown();

        auto startDate = std::format("{}/{}/{}",dates[0].day ,dates[0].month ,dates[0].year);
        auto endDate = std::format("{}/{}/{}",dates[1].day,dates[1].month ,dates[1].year);

        int daysLeft {presentToDueDate(dates[1])};

        tempFile << tasks.size() + 1 << '{';
        for (int x{2}; x < argc; ++x) {
            tempFile << argv[x] << " ";
        }
        tempFile << "}" << Constants::ongoingMark << '[' << startDate << ']' << endDate << '{' << daysLeft << '}' << ' ' << '[' <<  '\n';

        //std::filesystem::rename(tempPath,Constants::fileName);
    }


    void changeTaskIO(std::vector<TaskManager::Task> &tasks,
                      const char *taskID) {

        std::filesystem::path tempPath{"temp.txt"};
        std::ofstream tempFile(tempPath);

        int chosenTask{std::stoi(taskID) - 1};
        std::string descriptionToChange{tasks[chosenTask].description};

        Interface::changeTaskInput(descriptionToChange);

        tasks[chosenTask].description = descriptionToChange;

        for (const auto &task: tasks) {
            tempFile << task.id << '{' << task.description << '}' << task
                    .completion << '[' << task.date << ']' << task.dueDate << '{' << task.daysLeft << '\n';
        }

        std::filesystem::rename(tempPath,Constants::fileName);
    }

}

