#ifndef UNTITLED_ERRORHANDLING_H
#define UNTITLED_ERRORHANDLING_H
#include <TaskManager.h>

class ErrorHandling
{
public:
    enum ErrorType
    {
        outOfRange,
    };
private:
    const TaskManager& taskManager{};

public:
    explicit ErrorHandling(const TaskManager& tasker)
        :taskManager{tasker}
    {
    }

    [[nodiscard]] bool programStartHandle(int totalArgs) const;
    bool endErrorHandle(int totalArgs,char* endArgs[]) const;
    [[nodiscard]] bool deleteErrorHandle(int totalArgs,char* deletionArg[]) const;
    [[nodiscard]] bool addErrorHandle(int totalArgs) const;
    [[nodiscard]] bool changeErrorHandle(int totalArgs,char* changeArg[]) const;

    [[nodiscard]] bool duplicateCheck(const char* duplicateArg);
    [[nodiscard]] bool boundaryCheck(const char* boundaryArg) const;

};

#endif //UNTITLED_ERRORHANDLING_H