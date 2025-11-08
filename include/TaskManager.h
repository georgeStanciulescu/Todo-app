#ifndef UNTITLED_TASKMANAGER_H
#define UNTITLED_TASKMANAGER_H

#include <vector>
#include <string>

class TaskManager {
public:
    enum class DetailType
    {
        basic,
        add,
        deletion,
        end,
        info,
        change,
        list,
    };

    enum class DeletionType {
        single,
        multiple,
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
        std::string dueDateEndDateDifference;
    };
private:
    std::vector<Task> tasks{};
public:
    TaskManager();

    [[nodiscard]] const std::vector<Task>& getTasks() const {return tasks;}
    [[nodiscard]]std::string getStartDate(const int id) const {return tasks[id].date;}
    [[nodiscard]]std::string getEndDate(const int id) const {return tasks[id].endDate;}
    [[nodiscard]]std::string getDueDate(const int id) const {return tasks[id].dueDate;}

    void deleteAllTasks();
    void deleteTasks(const std::vector<int>& taskIDs);
    void endTask(int taskID,const char* status,const std::string& startDate,const std::string& dueDate);
    void addTask(char* argv[],int argc);
    void changeTask(int taskID);
    void listSimple();
    void listDetailed();
    bool duplicateCheck(const char* duplicateArg);


};
#endif //UNTITLED_TASKMANAGER_H