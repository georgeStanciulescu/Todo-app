#include <ErrorHandling.h>
#include <Constants.h>
#include <format>
#include <iostream>

#include "Interface.h"

//to be honest,it shouldn't even be a class,no that I've denuded it


bool ErrorHandling::stringArgCheck(const std::string_view arg,const CommandType cmd)
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

bool ErrorHandling::minArgs(const int totalArgs,const ArgCheck argCheck)
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

std::vector<int> ErrorHandling::argBreakdown(const char* arg,const int taskNumber) const
{
    std::istringstream stream{arg};
    std::string id{};
    std::vector<int> ids{};
    while (std::getline(stream,id,','))
    {
        try {
            const int val{std::stoi(id)};

            if (val < 1 || val > taskNumber) {
                Interface::exceptionErrorMessage(outOfRange,0);
                return {};
            }

            ids.push_back(val);
        } catch (const std::exception&) {
            Interface::exceptionErrorMessage(incorrectArg,0);
            return {};
        }

    }
    std::cout << ids.size() << '\n';
    return ids;
}





