#include "TaskManager.h"
#include "Calculations.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/table.hpp>

TaskManager::TaskManager()
{
    std::ifstream myFile("tasks.txt");

    int count{0};
    std::string myTask{};

    while (std::getline(myFile,myTask))
    {
        std::size_t taskName{myTask.find_first_of(' ') + 1};
        std::size_t tEnd{myTask.find_last_of('[') - 1};

        std::size_t statusPosition{myTask.find_last_of('[')};

        ++count;
        Task task{count,myTask.substr(taskName,tEnd - taskName),myTask[statusPosition + 1]};

        tasks.push_back(task);
    }
}

void TaskManager::deleteTask(const char* taskID)
{
    if (static_cast<std::string>(taskID) == "all")
    {
        std::remove("tasks.txt");
        std::cout << "All the tasks have been successfully deleted!" << "\n";
        return;
    }

    std::ofstream myFile("tasks.txt",std::ios::out | std::ofstream::trunc);

    int chosenTask{std::stoi(taskID)};
    --chosenTask; // Adjusting by one so that the index starts at one,not at zero

    tasks.erase(tasks.begin() + chosenTask);

    for (std::size_t x{0}; x < tasks.size();++x)
    {
        myFile <<"[" << x + 1 << "] " << tasks[x].description << " [" << tasks[x].completion << "]" << '\n';
    }
    std::cout << "The task has been successfully deleted!" << "\n";

    myFile.close();
}

void TaskManager::endTask(const char* taskID,const char* status) const
{
    std::ofstream myFile("tasks.txt",std::ios::out | std::ofstream::trunc);

    int chosenTask{std::stoi(taskID)};
    --chosenTask; // Adjusting by one so that the index starts at one,not at zero

    const std::string_view endedTask{tasks[chosenTask].description};
    ++chosenTask;

    for (const auto& task : tasks)
    {
        if (task.id != chosenTask)
        {
            myFile <<"[" << task.id  << "] " << task.description << " [" << task.completion << "]" << '\n';

        }
        else{myFile <<"[" << task.id << "] " << endedTask << " [" << status << "]" << '\n';}
    }
    std::cout << "The status of the task has been successfully updated!" << "\n";

    myFile.close();
}

void TaskManager::addTask(char* argv[],int argc) const
{
    std::ofstream myFile("tasks.txt",std::ios::app);

    myFile <<"[" << tasks.size() + 1 << "] ";
    for (int x{2}; x<argc ;++x)
    {
        myFile << argv[x] << " ";
    }
    myFile << "[-]" << '\n';

    std::cout << "Task successfully added!" << "\n";
}
void TaskManager::changeTask(const char* taskID)
{
    std::ofstream myFile("tasks.txt",std::ios::out | std::ofstream::trunc);

    int chosenTask{std::stoi(taskID)};
    --chosenTask; // Adjusting by one so that the index starts at one,not at zero
    std::string descriptionToChange{tasks[chosenTask].description};

    auto input = ftxui::Input(&descriptionToChange, "Edit task description");
    auto screen = ftxui::ScreenInteractive::TerminalOutput();

    input |= ftxui::CatchEvent([&](const ftxui::Event& event) {
    if (event == ftxui::Event::Return) {
        screen.Exit();
        return true;  // Consume the event and exit
    }
    return false;  // Let other events pass through
});
    screen.Loop(input);

    tasks[chosenTask].description = descriptionToChange;

    for (auto& task : tasks)
    {
       myFile <<"[" << task.id  << "] " << task.description << " [" << task.completion << "]" << '\n';
    }

    std::cout << "The task's description has been successfully updated!" << '\n';

    myFile.close();
}

void TaskManager::listTasks() {

    using namespace ftxui;
    std::ifstream myFile("tasks.txt");

    std::vector<Elements> tableContent{{text("ID") | bold | italic,
                                           text("Task") | bold | italic ,
                                           text("Status") | bold | italic }};

    float cancelledTasks{};
    float runningTasks{};
    float successTasks{};

    for (auto& task : tasks)
    {
        Elements values{};
        values.push_back(text(std::format("{}",task.id)) | bold );

        if (task.completion == 'x')
        {
            values.push_back(text(std::format("{}",task.description)) | bold | strikethrough);
            values.push_back(text(std::format("{}",task.completion)) | bold | color(Color::Red));
            ++cancelledTasks;
        }
        else if (task.completion == '-')
        {
            values.push_back(text(std::format("{}",task.description)) | bold);
            values.push_back(text(std::format("{}",task.completion)) | bold | strikethrough );
            ++runningTasks;
        }
        else
        {
            values.push_back(text(std::format("{}",task.description)) | bold );
            values.push_back(text(std::format("{}",task.completion)) | bold | color(Color::Green));
            ++successTasks;
        }

        tableContent.push_back(values);
    }
    float totalTasks{successTasks + runningTasks + cancelledTasks};

    auto table = Table(tableContent);
    table.SelectAll().Border(LIGHT);

    table.SelectColumn(0).Border(LIGHT);
    table.SelectColumn(1).Border(LIGHT);

    table.SelectRow(0).Border(LIGHT);
    table.SelectColumn(2).DecorateCells(center);

    auto renderedTable{table.Render()};

    auto completed = hbox(
        window(text("Completed") | bold | italic,text(std::format("{}%",dividingTasks(successTasks,totalTasks ) * 100)) | bold),
        gauge(successTasks/totalTasks) | bold | color(Color::DarkGreen )| border
        );

    auto uncompleted = hbox(
        window(text("Unfinished") | bold | italic,text(std::format("{}%",dividingTasks(runningTasks,totalTasks ) * 100)) | bold ),
        gauge(runningTasks/totalTasks) | bold | color(Color::NavajoWhite1) | border
    );

    auto failed = hbox(
        window(text("Failed") | bold | italic,text(std::format("{}%",dividingTasks(cancelledTasks,totalTasks ) * 100)) | bold),
        gauge(cancelledTasks/totalTasks) | bold | color(Color::Red) | border

    );

    auto document = vbox(
        text(""),
        paragraph(            "X      X  XXXXX XXXXX\n"
                                       "X      X  X       X\n"
                                       "X      X  XXXXX   X\n"
                                       "X      X      X   X\n"
                                       "XXXXX  X  XXXXX   X\n"   ) | bold | italic | color(Color::Gold1),
        text(""),
        renderedTable,
        completed,
        uncompleted,
        failed
        );
    auto screen = ftxui::Screen::Create(ftxui::Dimension::Fit(document,/*extend_beyond_screen=*/true),
                                       ftxui::Dimension::Fit(document,/*extend_beyond_screen=*/true));
    Render(screen, document);
    screen.Print();
    //std::cout << std::chrono::system_clock::now();
    std::cout << "\n\n";
}

void TaskManager::extraDetail(const DetailType type) const
{
    using namespace ftxui;

    switch (type)
    {
        case basic: {
            auto document = vbox(
                text(""),
                paragraph(                            "XXXXX XXXXX XXXXX  X  XXXXX X   X XXXXX\n"
                                                                "X   X X   X   X    X  X   X XX  X X\n"
                                                                "X   X XXXXX   X    X  X   X X X X XXXXX\n"
                                                                "X   X X       X    X  X   X X  XX     X\n"
                                                                "XXXXX X       X    X  XXXXX X   X XXXXX\n") | bold,
                text(""),
                text("[list] -- Presents the list of tasks")  | bold,
                text("[add] -- Add a task")  | bold,
                text("[end] -- Add a marker indicating completion,either success or failure")  | bold | strikethrough,
                text("[change] -- Change a task")  | bold,
                text("[delete] -- Delete the task completely")  | bold
            );

            auto screen = Screen::Create(Dimension::Fit(document), Dimension::Fit(document));
            Render(screen, document);
            screen.Print();

            std::cout << "\n\n";

            break;
        }
        case add:
            std::cout << '\n' << "Task addition form: task add [description] " << "\n\n";
            break;
        case deletion:
            std::cout << '\n' << "Task deletion form: task delete [ID] " << "\n\n";
            break;
        case change:
            std::cout << '\n' << "Task change form: task change [ID] " << "\n\n";
            break;
        case end:
            std::cout << '\n' << "Task end form: task end [ID] [status] " << '\n'
                      << "Status can be either x for failure,or s for success" << "\n\n";
            break;
    }
}