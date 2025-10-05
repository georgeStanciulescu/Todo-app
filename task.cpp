#include <iostream>
#include <vector>
#include <fstream>
#include <utility>
#include <list>

enum DetailType
{
    basic,
    add,
    deletion,
    end,
};

std::vector<std::pair<int,std::string>> taskOrder()
{
    std::ifstream myFile("first.txt");

    int count{0};
    std::string myTask{};
    std::vector<std::pair<int,std::string>> orderedTasks;

    while (std::getline(myFile,myTask))
    {
        std::size_t taskName{myTask.find_first_of(' ') + 1};
        ++count;
        std::pair<int,std::string> pair{count,myTask.substr(taskName)};


        orderedTasks.push_back(pair);

    }

    return orderedTasks;
}

void deleteTask(const char* task)
{
    if (static_cast<std::string>(task) == "all")
    {
        std::remove("first.txt");
        return;
    }

    std::ifstream myFile("first.txt",std::ios::in);
    std::ofstream newFile("new.txt",std::ios::out | std::ios::app);

    std::vector orderedTasks{taskOrder()};

    int chosenTask{std::stoi(task)};
    --chosenTask; // Adjusting by one so that the index starts at one,not at zero

    orderedTasks.erase(orderedTasks.begin() + chosenTask);

    for (auto& pair : orderedTasks)
    {
        if (pair.first <= chosenTask){newFile <<"[" << pair.first << "] " << pair.second << '\n'; }

        else{newFile <<"[" << --pair.first << "] " << pair.second << '\n'; }
    }

    std::remove("first.txt");
    std::rename("new.txt","first.txt");

    newFile.close();
}

void endTask(const char* task,const char* status)
{
    std::ifstream myFile("first.txt",std::ios::in);
    std::ofstream newFile("new.txt",std::ios::out | std::ios::app);

    std::vector orderedTasks{taskOrder()};

    int chosenTask{std::stoi(task)};
    --chosenTask; // Adjusting by one so that the index starts at one,not at zero

    std::string endedTask{orderedTasks[chosenTask].second};
    std::size_t taskStatus{endedTask.find_last_of(']') - 1 };

    endedTask.erase(taskStatus,1);
    endedTask.insert(taskStatus,static_cast<std::string>(status));

    for (const auto& pair : orderedTasks)
    {
        if (pair.first != ++chosenTask)
        {
            newFile <<"[" << pair.first << "] " << pair.second << '\n';

        }
        else{newFile <<"[" << pair.first << "] " << endedTask << '\n';}
    }

    std::remove("first.txt");
    std::rename("new.txt","first.txt");

    newFile.close();
}

void addTask(const char* task)
{
    std::ofstream myFile("first.txt",std::ios::app);

    std::vector orderedTasks{taskOrder()};
    myFile <<"[" << orderedTasks.size() + 1 << "] " << task << "[-]" << '\n';
}

void listTasks()
{
    std::ifstream myFile("first.txt",std::ios::in);
    std::string task{};

    std::cout << '\n' << "-------------------" << '\n';

    while (std::getline(myFile,task)){std::cout << task << '\n';}

    std::cout << "-------------------" << "\n\n";

}

void extraDetail(DetailType type)
{
    switch (type)
    {
        case basic:
            std::cout << '\n' << "Options:" << '\n'
                  << "[list] -- Presents the list of tasks" << '\n'
                  << "[add] -- Adds a task" << '\n'
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

int main(const int argc,char* argv[])
{

    if (argc < 2)
    {
        extraDetail(basic);
        return 1;
    }

    std::string_view command{argv[1]};

    if (command == "add")
    {
        if (argc <3)
        {
            extraDetail(add);
            return 1;
        }

        addTask(argv[2]);
    }

    if (command == "list"){listTasks();}

    if (command == "delete")
    {
        if (argc <3)
        {
            extraDetail(deletion);
            return 1;
        }

        deleteTask(argv[2]);
    }

    if (command == "end")
    {
        if (argc <3)
        {
            extraDetail(end);
            return 1;
        }

        endTask(argv[2],argv[3]);
    }

    return 0;
    }