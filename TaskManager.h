#ifndef UNTITLED_TASKMANAGER_H
#define UNTITLED_TASKMANAGER_H

#include <vector>
#include <iostream>


class TaskManager {
public:
    enum DetailType
    {
        basic,
        add,
        deletion,
        end,
        info,
        change,
    };

private:
    struct Task
    {
        int id;
        std::string description;
        char completion;
    };

    std::vector<Task> tasks{};

public:
    TaskManager();

    const std::vector<Task>& getTasks() const {return tasks;}
    void deleteTask(const char* taskID);
    void endTask(const char* taskID,const char* status) const;
    void addTask(char* argv[],int argc) const ;
    void changeTask(const char* taskID);
    void listTasks();
    void extraDetail(DetailType type) const ;
};
#endif //UNTITLED_TASKMANAGER_H