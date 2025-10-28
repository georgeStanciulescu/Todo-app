#include <FileIO.h>
#include <fstream>
#include <Constants.h>
#include <filesystem>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/table.hpp>

#include "Interface.h"
#include "TaskManager.h"

void fileIO(std::vector<TaskManager::Task>& tasks)
{
    if (!std::filesystem::exists(Constants::fileName))
    {
        std::ofstream fileCreation(Constants::fileName);
        if (!fileCreation) { throw std::runtime_error("Error : The file has failed to be created!");}
    }

    std::ifstream myFile(Constants::fileName);

    if (!myFile) {throw std::runtime_error("Error : The file has failed to open!");}

    int count{0};
    std::string myTask{};

    while (std::getline(myFile,myTask))
    {
        std::size_t tStart{myTask.find_first_of(' ')};
        std::size_t tEnd{myTask.find_last_of('[')};

        if (tEnd == std::string::npos || tStart == std::string::npos ||
            tEnd + 1>= myTask.size()  || tStart + 1 >= myTask.size()) {
            std::cerr << "Incorrect file format for line: " << myTask << '\n';
            continue;
            }

        ++count;

        tasks.emplace_back(count,
                    myTask.substr((tStart + 1),(tEnd -1 ) - tStart),
                     myTask[tEnd + 1]);
    }
    myFile.close();
}

void deleteTaskIO(std::vector<TaskManager::Task>& tasks,const char* taskID)
{
    std::ofstream myFile(Constants::fileName,std::ios::out | std::ofstream::trunc);

    int chosenTask{std::stoi(taskID)};
    --chosenTask; // Adjusting by one so that the index starts at one,not at zero

    tasks.erase(tasks.begin() + chosenTask);

    for (std::size_t x{0}; x < tasks.size();++x)
    {
        myFile <<"[" << x + 1 << "] " << tasks[x].description << " [" << tasks[x].completion << "]" << '\n';
    }

    myFile.close();
}

void deleteAllTasksIO()
{
    std::filesystem::remove(Constants::fileName.c_str());
}

void endTaskIO(const std::vector<TaskManager::Task>& tasks,const char* taskID,const char* status)
{
    std::ofstream myFile(Constants::fileName,std::ios::out | std::ofstream::trunc);

    int chosenTask{std::stoi(taskID)};

    const std::string_view endedTask{tasks[chosenTask - 1].description};

    for (const auto& task : tasks)
    {
        if (task.id != chosenTask)
        {
            myFile <<"[" << task.id  << "] " << task.description << " [" << task.completion << "]" << '\n';

        }
        else{myFile <<"[" << task.id << "] " << endedTask << " [" << status << "]" << '\n';}
    }

    myFile.close();
}

void addTaskIO(const std::vector<TaskManager::Task>& tasks,char* argv[],int argc)
{
    std::ofstream myFile(Constants::fileName,std::ios::app);

    myFile <<"[" << tasks.size() + 1 << "] ";
    for (int x{2}; x<argc ;++x)
    {
        myFile << argv[x] << " ";
    }
    myFile << "[" << Constants::ongoingMark << "]" << '\n';

    myFile.close();
}

void changeTaskIO(std::vector<TaskManager::Task>& tasks,const char* taskID)
{
    std::ofstream myFile(Constants::fileName,std::ios::out | std::ofstream::trunc);

    int chosenTask{std::stoi(taskID)};
    --chosenTask; // Adjusting by one so that the index starts at one,not at zero
    std::string descriptionToChange{tasks[chosenTask].description};

    changeTaskInput(descriptionToChange);

    tasks[chosenTask].description = descriptionToChange;

    for (auto& task : tasks)
    {
        myFile <<"[" << task.id  << "] " << task.description << " [" << task.completion << "]" << '\n';
    }

    myFile.close();
}
