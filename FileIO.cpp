#include <FileIO.h>
#include <fstream>
#include <Constants.h>
#include <filesystem>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/table.hpp>


#include "Date.h"
#include "Interface.h"
#include "TaskManager.h"

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

        if (!myFile) {
            throw std::runtime_error("Error : The file has failed to open!");
        }

        int count{0};
        std::string myTask{};

        while (std::getline(myFile, myTask)) {
            std::size_t openBrace{myTask.find_first_of('{')};
            std::size_t closeBrace{myTask.find_first_of('}')};
            std::size_t openBracket{myTask.find_first_of('[')};

            ++count;

            tasks.emplace_back(count,
                                myTask.substr(openBrace+1,closeBrace-3),
                                myTask[closeBrace+1],
                                myTask.substr(openBracket+1));
        }
        myFile.close();
    }

    void deleteTaskIO(std::vector<TaskManager::Task> &tasks,
                      const char *taskID) {

        std::filesystem::path tempPath{"temp.txt"};
        std::ofstream tempFile(tempPath);

        int chosenTask{std::stoi(taskID)};
        --chosenTask;
        // Adjusting by one so that the index starts at one,not at zero

        tasks.erase(tasks.begin() + chosenTask);

        for (std::size_t x{0}; x < tasks.size(); ++x) {
            tempFile  << x + 1 << "{" << tasks[x].description << "}" <<
                    tasks[x].completion << "[" << tasks[x].date <<'\n';
        }

        std::filesystem::rename(tempPath,Constants::fileName);
    }

    void deleteAllTasksIO() {
        std::filesystem::remove(Constants::fileName.c_str());
    }

    void endTaskIO(const std::vector<TaskManager::Task> &tasks,
                   const char *taskID, const char *status) {

        std::filesystem::path tempPath{"temp.txt"};
        std::ofstream tempFile(tempPath);

        int chosenTask{std::stoi(taskID)};

        const std::string_view endedTask{tasks[chosenTask - 1].description};

        for (const auto &task: tasks) {
            if (task.id != chosenTask) {
                tempFile  << task.id << "{" << task.description << "}" <<
                        task.completion << "[" << task.date << '\n';
            } else {
                tempFile << task.id << "{" << endedTask << "}" << status
                         << "[" << task.date << '\n';
            }
        }

        std::filesystem::rename(tempPath,Constants::fileName);
    }

    void addTaskIO(const std::vector<TaskManager::Task> &tasks, char *argv[],
                   int argc) {

        //std::filesystem::path tempPath{"temp.txt"};
        std::ofstream tempFile(Constants::fileName, std::ios::app);

        std::string_view presentDate{dateDropdown()};

        tempFile << tasks.size() + 1 << "{";
        for (int x{2}; x < argc; ++x) {
            tempFile << argv[x] << " ";
        }
        tempFile << "}" << Constants::ongoingMark << "[" << presentDate << '\n';

        //std::filesystem::rename(tempPath,Constants::fileName);
    }


    void changeTaskIO(std::vector<TaskManager::Task> &tasks,
                      const char *taskID) {

        std::filesystem::path tempPath{"temp.txt"};
        std::ofstream tempFile(tempPath);

        int chosenTask{std::stoi(taskID)};
        --chosenTask;
        // Adjusting by one so that the index starts at one,not at zero
        std::string descriptionToChange{tasks[chosenTask].description};

        Interface::changeTaskInput(descriptionToChange);

        tasks[chosenTask].description = descriptionToChange;

        // //id{description}status
        for (auto &task: tasks) {
            tempFile << task.id << "{" << task.description << "}" << task
                    .completion << "[" << task.date << '\n';
        }

        std::filesystem::rename(tempPath,Constants::fileName);
    }

}

