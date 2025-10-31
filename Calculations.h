
#ifndef UNTITLED_CALCULATIONS_H
#define UNTITLED_CALCULATIONS_H
#include "Date.h"

constexpr float dividingTasks(const float x,const float y)
{
    return y == 0 ? 0 : x/y;
}

constexpr bool is_leap_year(int y) {
    return (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
}

constexpr std::chrono::sys_days returnChronoDate(const DateInformation::DayMonthYear& date)
{
    return std::chrono::year_month_day{
        std::chrono::year(date.year),
        std::chrono::month(date.month),
        std::chrono::day(date.day)
    };
}

inline int presentToDueDate(const DateInformation::DayMonthYear& endDate)
{
    const auto presentDate = floor<std::chrono::days>(std::chrono::system_clock::now());
    const auto dueDate = returnChronoDate(endDate);

    const auto dayDifference = dueDate - presentDate;

    std::cout << "FROM THE CALCULATE DATE FUNCTION DIRECTLY,THE DIFFERENCE IS: " << dayDifference << '\n';

    return static_cast<int>(dayDifference.count());

}



#endif //UNTITLED_CALCULATIONS_H

