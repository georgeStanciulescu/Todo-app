#ifndef UNTITLED_CONSTANTS_H
#define UNTITLED_CONSTANTS_H
#include <string>

namespace Constants
{
    inline constexpr std::string_view fileName{"tasks.txt"};

    inline constexpr char successMark{'s'};
    inline constexpr char failureMark{'x'};
    inline constexpr char ongoingMark{'-'};

    inline constexpr char openBrace{'{'};
    inline constexpr char closedBrace{'}'};
    inline constexpr char openBracket{'['};
    inline constexpr char closedBracket{']'};
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

    inline std::string& spaceAdder(std::string& ascii,int count)
    {
        for (int x{0}; x <= count;++x)
        {
            //ascii.append(" ");
            ascii.insert(ascii.begin(),count,' ');
        }

        return ascii;
    }

}
#endif //UNTITLED_CONSTANTS_H