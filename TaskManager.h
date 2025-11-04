#ifndef UNTITLED_TASKMANAGER_H
#define UNTITLED_TASKMANAGER_H

#include <vector>
#include <string>

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
        list,
    };

    enum DeletionType {
        single,
        all,
    };
    struct Task
    {
        int id;
        std::string description;
        char completion;
        std::string date;
        std::string dueDate;
        std::string daysLeft;
        std::string endDate;
        std::string startDateEndDateDifference;
    };
private:
    std::vector<Task> tasks{};
public:
    TaskManager();

    [[nodiscard]] const std::vector<Task>& getTasks() const {return tasks;}
    DeletionType deleteTask(const char* taskID);
    void endTask(const char* taskID,const char* status,const std::string& startDate);
    void addTask(char* argv[],int argc);
    void changeTask(const char* taskID);
    void listTasks();
    bool duplicateCheck(const char* duplicateArg);
    std::string getStartDate(const int id) const {return tasks[id].date;}
    std::string getEndDate(const int id) const {return tasks[id].endDate;}

};
#endif //UNTITLED_TASKMANAGER_H