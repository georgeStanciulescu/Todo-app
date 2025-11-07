#ifndef UNTITLED_CONSTANTS_H
#define UNTITLED_CONSTANTS_H
#include <string>

namespace Constants
{
    inline constexpr std::string_view fileName{"tasks.txt"};

    inline constexpr char successMark{'s'};
    inline constexpr char failureMark{'x'};
    inline constexpr char ongoingMark{'-'};
    inline constexpr char pipeDelimiter{'|'};

    inline constexpr int minEndArgs{4};
    inline constexpr int minStartArgs{2};
    inline constexpr int minGeneralArgs{3};

    inline constexpr int firstArg{0};
    inline constexpr int secondArg{1};
    inline constexpr int thirdArg{2};
    inline constexpr int fourthArg{3};

    constexpr std::string_view listASCII{R"(
  ██╗     ██╗███████╗████████╗
  ██║     ██║██╔════╝╚══██╔══╝
  ██║     ██║███████╗   ██║
  ██║     ██║╚════██║   ██║
  ███████╗██║███████║   ██║
  ╚══════╝╚═╝╚══════╝   ╚═╝   )"};

    constexpr std::string_view optionsASCII{ R"(
   ██████╗ ██████╗ ████████╗██╗ ██████╗ ███╗   ██╗███████╗
  ██╔═══██╗██╔══██╗╚══██╔══╝██║██╔═══██╗████╗  ██║██╔════╝
  ██║   ██║██████╔╝   ██║   ██║██║   ██║██╔██╗ ██║███████╗
  ██║   ██║██╔═══╝    ██║   ██║██║   ██║██║╚██╗██║╚════██║
  ╚██████╔╝██║        ██║   ██║╚██████╔╝██║ ╚████║███████║
   ╚═════╝ ╚═╝        ╚═╝   ╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝
                                                        )"};

}
#endif //UNTITLED_CONSTANTS_H