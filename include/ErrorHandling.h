#ifndef UNTITLED_ERRORHANDLING_H
#define UNTITLED_ERRORHANDLING_H
#include <TaskManager.h>

class ErrorHandling
{
public:
    enum ErrorType
    {
        id,
        description,
        status,
        startDate,
        dueDate,
        daysLeft,
        endDate,
        startDateEndDateDifference,
        dueDateEndDateDifference,
        outOfRange,
        failedCreation,
        failedOpening,
        incorrectArg,
    };

    enum class ArgCheck
    {
        min,
        general,
        max,
    };

    enum class CommandType {
        list,
        deletion,
        end
    };

    bool stringArgCheck(std::string_view arg,CommandType cmd);
    std::vector<int> argBreakdown(const char* arg,int taskNumber) const;
    bool minArgs(int totalArgs,ArgCheck argCheck);

};

#endif //UNTITLED_ERRORHANDLING_H