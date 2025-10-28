#include <TaskManager.h>
#include <Calculations.h>
#include <Constants.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/table.hpp>
#include <chrono>


TaskManager::TaskManager()
{
    std::ifstream myFile(Constants::fileName);

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
        std::remove(Constants::fileName.c_str());
        std::cout << "All the tasks have been successfully deleted!" << "\n";
        return;
    }

    std::ofstream myFile(Constants::fileName,std::ios::out | std::ofstream::trunc);

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

void TaskManager::endTask(const char* taskID,const char* status)
{
    std::ofstream myFile(Constants::fileName,std::ios::out | std::ofstream::trunc);

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

void TaskManager::addTask(char* argv[],int argc)
{
    std::ofstream myFile(Constants::fileName,std::ios::app);

    myFile <<"[" << tasks.size() + 1 << "] ";
    for (int x{2}; x<argc ;++x)
    {
        myFile << argv[x] << " ";
    }
    myFile << "[" << Constants::ongoingMark << "]" << '\n';

    std::cout << "Task successfully added!" << "\n";
}

void TaskManager::changeTask(const char* taskID)
{
    std::ofstream myFile(Constants::fileName,std::ios::out | std::ofstream::trunc);

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
    std::ifstream myFile(Constants::fileName);

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

        if (task.completion == Constants::failureMark)
        {
            values.push_back(text(std::format("{}",task.description)) | bold | strikethrough);
            values.push_back(text(std::format("{}",task.completion)) | bold | color(Color::Red));
            ++cancelledTasks;
        }
        else if (task.completion == Constants::ongoingMark)
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
        paragraph( Constants::listASCII) | bold | italic,
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

void TaskManager::extraDetail(const DetailType type) const {
    using namespace ftxui;

    Element document{};
    switch (type)
    {
        case basic: {
            document = vbox(
                text(""),
                paragraph(Constants::optionsASCII) ,
                text(""),
                text("[list] -- Presents the list of tasks")  ,
                text("[add] -- Add a task")  ,
                text("[end] -- Add a marker indicating completion,either success or failure")  ,
                text("[change] -- Change a task")  | bold,
                text("[info] -- Information regarding the to do app") ,
                text("[delete] -- Delete the task completely")
            );
            break;
        }
        case add:
        {
            document = vbox(text(""),
            text("Task addition form: task add [description] ")) ;
            break;
        }
        case deletion:
        {
            document = vbox(text(""),
            text("Task deletion form: task delete [ID] ")) ;
            break;
        }
        case change:
        {
            document = vbox(text(""),
            text("Task deletion form: task change [ID] ")) ;
            break;
        }
        case end:
        {
            document = vbox(text(""),
            text("Task end form: task end [ID] [status] "),
            text( " Status can be either x for failure,or s for success")) ;
            break;
        }
        case info:
            document = vbox(text(""),
            paragraph("The purpose of the to do app is to log whatever task you would like to do.\n"
                      "The app also tracks your progress;it allows you to change the tasks' description,to delete them if necessary,etc.\n"
                      "Soon,I shall add further functionality,via date-keeping and -updating!")) ;
            break;
        case list:
            break;
    }

    document |= bold;
    auto screen = Screen::Create(Dimension::Fit(document ), Dimension::Fit(document));
    Render(screen, document);
    screen.Print();
    std::cout << "\n\n";
}