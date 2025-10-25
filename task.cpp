#include <iostream>
#include <vector>
#include <fstream>
#include <optional>


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
};

std::vector<Task> taskOrder();
void deleteTask(std::vector<Task>& orderedTasks,const char* taskID);
void endTask(const std::vector<Task>& orderedTasks,const char* taskID,const char* status,int argc);
void addTask(char* argv[],int argc,const std::vector<Task>& orderedTasks);
void listTasks();
void extraDetail(DetailType type);

bool endErrorHandle(int argc,char* argv[],const std::vector<Task>& orderedTasks);
bool deleteErrorHandle(int argc,char* argv[],const std::vector<Task>& orderedTasks);
bool addErrorHandle(int argc);
bool duplicateCheck(char* argv[],const std::vector<Task>& orderedTasks);

int main(const int argc,char* argv[])
{
    if (argc < 2)
    {
        extraDetail(basic);
        return 1;
    }

    const std::string_view command{argv[1]}; // leave it here;if you put it at the start of main it will cause a segmentation fault

    if (command == "list"){listTasks();}

    std::vector orderedTasks{taskOrder()};

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

    std::string endedTask{orderedTasks[chosenTask].description};
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

void listTasks()
{
    std::ifstream myFile("tasks.txt");

    std::string task{};

    std::cout << '\n' << "X      X  XXXXX XXXXX" << '\n' <<
                         "X      X  X       X"   << '\n' <<
                         "X      X  XXXXX   X"   << '\n' <<
                         "X      X      X   X"   << '\n' <<
                         "XXXXX  X  XXXXX   X"   << '\n';

    std::cout << '\n' << "-------------------" << '\n';

    while (std::getline(myFile,task)){std::cout << task << '\n';}

    std::cout << "-------------------" << "\n\n";

}

void extraDetail(DetailType type)
{
    switch (type)
    {
        case basic:
            std::cout << '\n'<< "XXXXX XXXXX XXXXX  X  XXXXX X   X XXXXX" << '\n' <<
                                "X   X X   X   X    X  X   X XX  X X"     << '\n' <<
                                "X   X XXXXX   X    X  X   X X X X XXXXX" << '\n' <<
                                "X   X X       X    X  X   X X  XX     X" << '\n' <<
                                "XXXXX X       X    X  XXXXX X   X XXXXX" << '\n';

            std::cout << '\n' << "Options:" << '\n'
                  << "[list] -- Presents the list of tasks" << '\n'
                  << "[add] -- Add a task" << '\n'
                  << "[end] -- Add a marker indicating completion,either success or failure" << '\n'
                  << "[delete] -- Delete the task completely" << "\n\n";
            break;
        case add:
            std::cout << '\n' << "Task addition form: task add [description] " << "\n\n";
            break;
        case deletion:
            std::cout << '\n' << "Task deletion form: task delete [ID] " << "\n\n";
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

    try
    {
        const int taskID{std::stoi(argv[2])};

        if (taskID < 1 || taskID > orderedTasks.size())
        {
            std::cout << "The task ID is outside of the range! Introduce an ID for a task in the list!" << "\n";
            return false;
        }
    } catch (std::exception&) {

        std::cout  << "You must enter the ID of the task;anything else will lead to an error!" << "\n";
        return false;
    }

    const auto statusType{static_cast<std::string_view>(argv[3])};

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

    if (static_cast<std::string>(argv[2]) == "all"){return true;}

    try
    {
        const int taskID{std::stoi(argv[2])};

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

bool addErrorHandle(int argc)
{
    if (argc <3)
    {
        extraDetail(add);
        return false;
    }

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
