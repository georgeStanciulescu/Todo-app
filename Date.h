#ifndef UNTITLED_DATE_H
#define UNTITLED_DATE_H

#include "ftxui/component/captured_mouse.hpp"  // for ftxui
#include "ftxui/component/component.hpp"  // for Dropdown, Horizontal, Vertical
#include "ftxui/component/screen_interactive.hpp"  // for ScreenInteractive
#include <chrono>
#include <vector>
#include <iostream>

struct DayMonthYear;
namespace DateInformation {
    enum DateType
    {
        past,
        due,
    };
};

std::string dateDropdown();
ftxui::Component taskStartDate(DayMonthYear& dates,std::vector<std::string>& years,std::vector<std::string>& days);
void dateChoiceInput(DayMonthYear& dates,std::vector<std::string>& days,std::vector<std::string>& years,
                                 ftxui::Component& layout,ftxui::ScreenInteractive& screen);
std::vector<std::string> returnDates();
std::vector<std::string> calculateYears(DateInformation::DateType dateType);

inline bool is_leap_year(int y) {
    return (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
}


#endif //UNTITLED_DATE_H