#include <iostream>
#include <vector>
#include <fstream>
#include <optional>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <chrono>

#include "ftxui/dom/table.hpp"

struct Task
{
    int id;
    std::string description;
    char completion;
};

enum DetailType
{
    basic,
    add,
    deletion,
    end,
    change,
};

std::vector<Task> taskOrder();
void deleteTask(std::vector<Task>& orderedTasks,const char* taskID);
void endTask(const std::vector<Task>& orderedTasks,const char* taskID,const char* status,int argc);
void addTask(char* argv[],int argc,const std::vector<Task>& orderedTasks);
void changeTask(std::vector<Task>& orderedTasks,const char* taskID);
void listTasks(std::vector<Task>& orderedTasks);
void extraDetail(DetailType type);

bool endErrorHandle(int argc,char* argv[],const std::vector<Task>& orderedTasks);
bool deleteErrorHandle(int argc,char* argv[],const std::vector<Task>& orderedTasks);
bool addErrorHandle(int argc);
bool changeErrorHandle(int argc,char* argv[],const std::vector<Task>& orderedTasks);

bool duplicateCheck(char* argv[],const std::vector<Task>& orderedTasks);
bool boundaryCheck(const char* argv,const std::vector<Task>& orderedTasks);
float dividingTasks(float x,float y);




int main(const int argc,char* argv[])
{

    if (argc < 2)
    {
        extraDetail(basic);
        return 1;
    }

    const std::string_view command{argv[1]}; // leave it here;if you put it at the start of main it will cause a segmentation fault

    std::vector orderedTasks{taskOrder()};

    if (command == "list"){listTasks(orderedTasks);}

    if (command == "add")
    {
        if (!addErrorHandle(argc)){return 1;}
        if (duplicateCheck(argv,orderedTasks)){return 1;}

        addTask(argv,argc,orderedTasks);
    }

    if (command == "delete")
    {
        if (!deleteErrorHandle(argc,argv,orderedTasks)){return 1;}

        deleteTask(orderedTasks,argv[2]);
    }

    if (command == "end")
    {
        if (!endErrorHandle(argc,argv,orderedTasks)){return 1;}

        endTask(orderedTasks,argv[2],argv[3],argc);
    }

    if (command == "change")
    {
        if (!changeErrorHandle(argc,argv,orderedTasks)){return 1;}

        changeTask(orderedTasks,argv[2]);
    }

    return 0;
}

std::vector<Task> taskOrder()
{
    std::ifstream myFile("tasks.txt");

    int count{0};
    std::string myTask{};

    std::vector<Task> orderedTasks;

    while (std::getline(myFile,myTask))
    {
        std::size_t taskName{myTask.find_first_of(' ') + 1};
        std::size_t tEnd{myTask.find_last_of('[') - 1};

        std::size_t statusPosition{myTask.find_last_of('[')};

        ++count;
        Task task{count,myTask.substr(taskName,tEnd - taskName),myTask[statusPosition + 1]};

        orderedTasks.push_back(task);
    }

    return orderedTasks;
}

void deleteTask(std::vector<Task>& orderedTasks,const char* taskID)
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

    orderedTasks.erase(orderedTasks.begin() + chosenTask);

    for (std::size_t x{0}; x < orderedTasks.size();++x)
    {
        myFile <<"[" << x + 1 << "] " << orderedTasks[x].description << " [" << orderedTasks[x].completion << "]" << '\n';
    }
    std::cout << "The task has been successfully deleted!" << "\n";

    myFile.close();
}

void endTask(const std::vector<Task>& orderedTasks,const char* taskID,const char* status,int argc)
{
    std::ofstream myFile("tasks.txt",std::ios::out | std::ofstream::trunc);

    int chosenTask{std::stoi(taskID)};
    --chosenTask; // Adjusting by one so that the index starts at one,not at zero

    const std::string_view endedTask{orderedTasks[chosenTask].description};
    ++chosenTask;

    for (const auto& task : orderedTasks)
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

void addTask(char* argv[],int argc,const std::vector<Task>& orderedTasks)
{
    std::ofstream myFile("tasks.txt",std::ios::app);

    myFile <<"[" << orderedTasks.size() + 1 << "] ";
    for (int x{2}; x<argc ;++x)
    {
        myFile << argv[x] << " ";
    }
    myFile << "[-]" << '\n';

    std::cout << "Task successfully added!" << "\n";
}
void changeTask( std::vector<Task>& orderedTasks,const char* taskID)
{
    std::ofstream myFile("tasks.txt",std::ios::out | std::ofstream::trunc);

    int chosenTask{std::stoi(taskID)};
    --chosenTask; // Adjusting by one so that the index starts at one,not at zero
    std::string descriptionToChange{orderedTasks[chosenTask].description};

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

    orderedTasks[chosenTask].description = descriptionToChange;

    for (auto& task : orderedTasks)
    {
       myFile <<"[" << task.id  << "] " << task.description << " [" << task.completion << "]" << '\n';
    }

    std::cout << "The task's description has been successfully updated!" << '\n';

    myFile.close();
}

void listTasks(std::vector<Task>& orderedTasks) {

    using namespace ftxui;
    std::ifstream myFile("tasks.txt");

    std::vector<Elements> tableContent{{text("ID") | bold | italic,
                                           text("Task") | bold | italic ,
                                           text("Status") | bold | italic }};

    float cancelledTasks{};
    float runningTasks{};
    float successTasks{};

    for (auto& task : orderedTasks)
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

void extraDetail(const DetailType type)
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

bool endErrorHandle(int argc,char* argv[],const std::vector<Task>& orderedTasks)
{
    if (argc <4)
    {
        extraDetail(end);
        return false;
    }
    if (!boundaryCheck(argv[2],orderedTasks)){return false;}

    const std::string_view statusType{(argv[3])};

    if (statusType != "s" && statusType != "x")
    {
        extraDetail(end);
        return false;
    }

    return true;
}

bool deleteErrorHandle(int argc,char* argv[],const std::vector<Task>& orderedTasks)
{
    if (argc <3)
    {
        extraDetail(deletion);
        return false;
    }
    const std::string_view deletionType{argv[2]};

    if (deletionType == "all"){return true;}

    if (!boundaryCheck(argv[2],orderedTasks)){return false;}

    return true;
}

bool addErrorHandle(int argc)
{
    if (argc <3)
    {
        extraDetail(add);
        return false;
    }

    return true;
}

bool changeErrorHandle(int argc,char* argv[],const std::vector<Task>& orderedTasks)
{
    if (argc <3)
    {
        extraDetail(change);
        return false;
    }

    if (!boundaryCheck(argv[2],orderedTasks)){return false;}

    return true;
}

bool duplicateCheck(char* argv[],const std::vector<Task>& orderedTasks) {

    bool isDuplicate{false};

    const std::string taskName{argv[2]};

    for (const auto& task : orderedTasks) {
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

bool boundaryCheck(const char* argv,const std::vector<Task>& orderedTasks)
{
    try
    {
        const int taskID{std::stoi(argv)};

        if (taskID < 1 || taskID > orderedTasks.size())
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

float dividingTasks(const float x,const float y)
{
    if (y == 0){return 0;}
    return x/y;
}


