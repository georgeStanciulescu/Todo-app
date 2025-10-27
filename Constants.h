#ifndef UNTITLED_CONSTANTS_H
#define UNTITLED_CONSTANTS_H
#include <iostream>

namespace Constants
{
    inline constexpr std::string fileName{"tasks.txt"};

    inline constexpr char successMark{'s'};
    inline constexpr char failureMark{'x'};
    inline constexpr char ongoingMark{'-'};

    inline std::string listASCII{R"(
  ██╗     ██╗███████╗████████╗
  ██║     ██║██╔════╝╚══██╔══╝
  ██║     ██║███████╗   ██║
  ██║     ██║╚════██║   ██║
  ███████╗██║███████║   ██║
  ╚══════╝╚═╝╚══════╝   ╚═╝   )"};

    inline std::string optionsASCII{ R"(
   ██████╗ ██████╗ ████████╗██╗ ██████╗ ███╗   ██╗███████╗
  ██╔═══██╗██╔══██╗╚══██╔══╝██║██╔═══██╗████╗  ██║██╔════╝
  ██║   ██║██████╔╝   ██║   ██║██║   ██║██╔██╗ ██║███████╗
  ██║   ██║██╔═══╝    ██║   ██║██║   ██║██║╚██╗██║╚════██║
  ╚██████╔╝██║        ██║   ██║╚██████╔╝██║ ╚████║███████║
   ╚═════╝ ╚═╝        ╚═╝   ╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝
                                                        )"};

}
#endif //UNTITLED_CONSTANTS_H