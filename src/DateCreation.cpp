#include "DateCreation.h"

#include <Date.h>
#include "ftxui/component/component.hpp"

ftxui::Component taskStartDate(DateInformation::DayMonthYear &dates,const std::vector<std::string> &years,
                               const std::vector<std::string> &days,const std::vector<std::string> &months,
                               const DateInformation::DateType calendarType) {
    using namespace ftxui;
    const auto dayDrop = Dropdown((&days), &dates.day);
    const auto monthDrop = Dropdown(&months, &dates.month);
    const auto yearDrop = Dropdown(&years, &dates.year);

    std::string title{};
    if (calendarType ==
        DateInformation::DateType::past) { title = "Task start date"; } else if
    (calendarType == DateInformation::DateType::due) {
        title = "Task due date";
    } else { title = "Task completion date"; }

    auto layout = Container::Vertical({
        Renderer([title] { return text(title); }) | ftxui::bold | ftxui::italic
        | center,
        Container::Horizontal({
            //Renderer([&yearDrop] { return window(text("Year"),yearDrop)})
            yearDrop | bold,
            monthDrop | bold,
            dayDrop | bold
        })
    });

    return layout;
}

ftxui::Component makeComponent(ftxui::Element &element)
{
    return ftxui::Renderer([&element] { return element; });
}


ftxui::Component createDateDropdown(ftxui::Component& layout)
{
    return ftxui::Container::Horizontal({
             ftxui::Renderer([] { return ftxui::text("               "); }),
             ftxui::Renderer([] { return ftxui::separator(); }),
             layout,
             ftxui::Renderer([] { return ftxui::separator(); }),
         });
}

ftxui::Component createDateDropdown(ftxui::Component& layout,ftxui::Component& dueDate)
{
    return ftxui::Container::Horizontal({
             ftxui::Renderer([] { return ftxui::text("    "); }),
             layout,
             ftxui::Renderer([] { return ftxui::separator(); }),
             dueDate
         });
}

ftxui::Element createDateInfoWindow()
{
   return ftxui::window(ftxui::text("Info") | ftxui::bold,
                      ftxui::paragraph(
                           "You can navigate the date dropdown using the mouse \n"
                                    "Alternatively,TAB,Enter,and the arrow keys can be used \n"
                                    "After you have entered the dates,press Escape to finish \n\n")
                                 | ftxui::bold | ftxui::italic);

}

std::vector<std::string> calculateYears(const DateInformation::DateType dateType,const int start)
{
    using namespace DateInformation;
    const auto presentYear = returnPresentDate().year();

    std::vector<std::string> years{};

    switch (dateType) {
        case DateType::past:
            for (int x{presentYear}; x > (static_cast<int>(presentYear) - 5); --x)
            {
                years.emplace_back(std::to_string(x));
            }
            break;
        case DateType::due:
            for (int x{presentYear}; x < (static_cast<int>(presentYear) + 5); ++x)
            {
                years.emplace_back(std::to_string(x));
            }
            break;
        case DateType::end:
            for (int x{start}; x <= (static_cast<int>(presentYear)); ++x)
            {
                years.emplace_back(std::to_string(x));
            }
            break;
    }
    return years;
}

std::vector<std::string> returnCalendarMonth(const int count)
{
    static const std::vector<std::string> bigMonthDays{
        "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13",
        "14", "15", "16", "17", "18","19", "20", "21", "22", "23", "24", "25",
        "26", "27", "28", "29", "30","31"};

    return {bigMonthDays.begin(), bigMonthDays.begin() + count};
}

std::vector<std::string> returnMonthNames()
{
    return {
        "January", "February", "March", "April", "May", "June", "July",
        "August", "September", "October", "November", "December"};
}

std::vector<int> leapYearBetweenDates(const int start, const int end)
{
    std::vector<int> leapYears{};
    for (int x{start}; x <= end; ++x)
    {
        if (static_cast<std::chrono::year>(x).is_leap()) {
            leapYears.emplace_back(static_cast<int>(x));
        }
    }
    return leapYears;
}

//Kinda hacky,if one's purpose were to use this function outside of the FTXUI-bound,indexed approach
// past.year and future.year are both indices into yearsPast and yearsFuture;thus if they both equal zero,they're the same year
bool isPastGreater(const DateInformation::DayMonthYear &past,
                   const DateInformation::DayMonthYear &future)
{
    if (past.year == 0 && future.year == 0)
    {
        if (past.month > future.month) { return true; }
        if (past.month == future.month && past.day > future.day) {return true;}
    }
    return false;
}

int daysNumberFirstMonth(const int startMonth, const int year)
{
    if (startMonth == 3 || startMonth == 5 ||
        startMonth == 8 || startMonth ==10){ return 30; }

    if (startMonth == 1)
    {
        if (static_cast<std::chrono::year>(year).is_leap()) { return 29; }
        return 28;
    }
    return 31;
}

std::chrono::year_month_day returnPresentDate()
{
    return floor<std::chrono::days>(std::chrono::system_clock::now());
}

DateInformation::DayMonthYear returnStartDateInt(const std::string &startDate)
{
    const auto dayEnd{startDate.find_first_of('/')};
    const auto monthEnd{startDate.find_last_of('/')};

    const auto day{startDate.substr(0, dayEnd)};
    const auto month{startDate.substr(dayEnd + 1, monthEnd - dayEnd - 1)};
    const auto year{startDate.substr(monthEnd + 1)};

    return {std::stoi(day), std::stoi(month), std::stoi(year)};
}

DateInformation::DayMonthYear convertIndices(const DateInformation::DayMonthYear& date,
                                             const std::vector<std::string>& years)
{
    return {    date.day + 1,date.month + 1,std::stoi(years[date.year])};
}

