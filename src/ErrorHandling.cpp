#include <ErrorHandling.h>
#include "Interface.h"

namespace ErrorHandling
{
    bool stringArgCheck(const std::string_view arg,const CommandType cmd)
    {
        switch (cmd) {
            case CommandType::list:
                if (arg != "-d"){return false;}
                break;
            case CommandType::end:
                if (arg.size() != 1){return false;}
                break;
            case CommandType::deletion:
                if (arg != "all"){return false;}
                break;
        }

        return true;
    }

    bool minArgs(const int totalArgs,const ArgCheck argCheck)
    {
        switch (argCheck) {
            case ArgCheck::min:
                if (totalArgs < 2){return false;}
                break;
            case  ArgCheck::general:
                if (totalArgs < 3){return false;}
                break;
            case ArgCheck::max:
                if (totalArgs < 4){return false;}
                break;
        }

        return true;
    }

    ArgParseResult argBreakdown(const char* arg,const int taskNumber)
    {
        std::istringstream stream{arg};
        std::string id{};

        ArgParseResult result{};

        while (std::getline(stream,id,','))
        {
            try {
                const int val{std::stoi(id)};

                if (val < 1 || val > taskNumber) {
                    //Interface::exceptionErrorMessage(ErrorType::outOfRange,0);
                    result.errorType = ErrorType::outOfRange;
                    return {};
                }

                result.ids.push_back(val);
            } catch (const std::exception&) {

                //Interface::exceptionErrorMessage(ErrorType::incorrectArg,0);
                result.errorType = ErrorType::outOfRange;
                return {};
            }

        }

        return result;
    }

}





