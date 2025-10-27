#include <iostream>
#include <vector>
#include <fstream>
#include <ftxui/component/screen_interactive.hpp>
#include <chrono>
#include "TaskManager.h"
#include "ErrorHandling.h"

int main(const int argc,char* argv[])
{
    TaskManager tasker{};
    const ErrorHandling validator{tasker};
    
    if (argc < 2)
    {
        tasker.extraDetail(TaskManager::basic);
        return 1;
    }

    const std::string_view command{argv[1]}; // leave it here;if you put it at the start of main it will cause a segmentation fault


    if (command == "list"){tasker.listTasks();}

    if (command == "add")
    {
        if (!validator.addErrorHandle(argc)){return 1;}
        if (validator.duplicateCheck(argv)){return 1;}

        tasker.addTask(argv,argc);
    }

    if (command == "delete")
    {
        if (!validator.deleteErrorHandle(argc,argv)){return 1;}

        tasker.deleteTask(argv[2]);
    }

    if (command == "end")
    {
        if (!validator.endErrorHandle(argc,argv)){return 1;}

        tasker.endTask(argv[2],argv[3]);
    }

    if (command == "change")
    {
        if (!validator.changeErrorHandle(argc,argv)){return 1;}

        tasker.changeTask(argv[2]);
    }

    return 0;
}







