#ifndef UNTITLED_DATE_H
#define UNTITLED_DATE_H

#include "ftxui/component/captured_mouse.hpp"  // for ftxui
#include "ftxui/component/component.hpp"  // for Dropdown, Horizontal, Vertical
#include "ftxui/component/screen_interactive.hpp"  // for ScreenInteractive
#include <chrono>
#include <vector>
#include <iostream>

namespace DateInformation {

    struct DateCalculation
    {
        int prevMonth{-1};
        int prevYear{};
        int leapYear{};
    };

    struct DayMonthYear
    {
        int day{};
        int month{};
        int year{};
    };

    enum DateType {
        due,
        past,
    };

    // inline std::vector<std::string> months{
    //     "January", "February", "March", "April", "May", "June", "July",
    //     "August", "September", "October", "November", "December"
    // };
    // inline std::vector<std::string> february{
    //     "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13",
    //     "14", "15", "16", "17", "18",
    //     "19", "20", "21", "22", "23", "24", "25", "26", "27", "28"
    // };
    // inline std::vector<std::string> februaryLeap{
    //     "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13",
    //     "14", "15", "16", "17", "18",
    //     "19", "20", "21", "22", "23", "24", "25", "26", "27", "28", "29"
    // };
    // inline std::vector<std::string> standardAmountOfDays{
    //     "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13",
    //     "14", "15", "16", "17", "18",
    //     "19", "20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "30"
    // };
    // inline std::vector<std::string> bigAmountOfDays{
    //     "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13",
    //     "14", "15", "16", "17", "18",
    //     "19", "20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "30",
    //     "31"};

};





std::vector<DateInformation::DayMonthYear>  dateDropdown();
ftxui::Component taskStartDate(DateInformation::DayMonthYear& dates,const std::vector<std::string>& years,const std::vector<std::string>& days,const std::vector<std::string>& months);

std::vector<std::string> calculateYears(DateInformation::DateType dateType);
std::vector<std::string> returnCalendarMonth(int count);
constexpr std::vector<std::string> returnMonthNames();



#endif //UNTITLED_DATE_H