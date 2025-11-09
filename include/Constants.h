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

    inline constexpr std::string_view listASCII{R"(
  ██╗     ██╗███████╗████████╗
  ██║     ██║██╔════╝╚══██╔══╝
  ██║     ██║███████╗   ██║
  ██║     ██║╚════██║   ██║
  ███████╗██║███████║   ██║
  ╚══════╝╚═╝╚══════╝   ╚═╝   )"};

    inline constexpr std::string_view optionsASCII{ R"(
   ██████╗ ██████╗ ████████╗██╗ ██████╗ ███╗   ██╗███████╗
  ██╔═══██╗██╔══██╗╚══██╔══╝██║██╔═══██╗████╗  ██║██╔════╝
  ██║   ██║██████╔╝   ██║   ██║██║   ██║██╔██╗ ██║███████╗
  ██║   ██║██╔═══╝    ██║   ██║██║   ██║██║╚██╗██║╚════██║
  ╚██████╔╝██║        ██║   ██║╚██████╔╝██║ ╚████║███████║
   ╚═════╝ ╚═╝        ╚═╝   ╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝
                                                        )"};

}
#endif //UNTITLED_CONSTANTS_H