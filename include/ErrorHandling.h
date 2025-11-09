#ifndef UNTITLED_ERRORHANDLING_H
#define UNTITLED_ERRORHANDLING_H

#include <optional>
#include <vector>
#include <string_view>

namespace ErrorHandling {

    enum class ErrorType
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

    struct ArgParseResult {
        std::optional<ErrorType> errorType{};
        std::vector<int> ids{};
    };

    bool stringArgCheck(std::string_view arg,CommandType cmd);
    ArgParseResult argBreakdown(const char* arg,int taskNumber);
    bool minArgs(int totalArgs,ArgCheck argCheck);

}

#endif //UNTITLED_ERRORHANDLING_H