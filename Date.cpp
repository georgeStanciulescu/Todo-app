#include <Date.h>
#include <fstream>
#include <future>

#include "Calculations.h"
#include "Constants.h"
#include "InterfaceComposition.h"



std::vector<DateInformation::DayMonthYear> dateDropdown() {
    using namespace DateInformation;

    const auto yearsPast = calculateYears(past);
    const auto yearsFuture = calculateYears(due);

    DayMonthYear pastDate{};
    DayMonthYear futureDate{};

    std::vector daysPast{returnCalendarMonth(31)};
    std::vector daysFuture{returnCalendarMonth(31)};

    const auto months = returnMonthNames();

    auto layout = taskStartDate(pastDate, yearsPast, daysPast,months);
    auto dueDate = taskStartDate(futureDate, yearsFuture, daysFuture,months);

    std::cout << "\033[2J\033[H" << std::flush; // used to flush the screen
    auto screen = ftxui::ScreenInteractive::FitComponent();

    DateCalculation pAction{};
    DateCalculation dAction{};

    auto changeDropdown = [&](DateCalculation& action,DayMonthYear& date,std::vector<std::string>& days,ftxui::Component& input)
    {
        input |= ftxui::CatchEvent([&](const ftxui::Event &event) {
        if (event == ftxui::Event::Escape) {
            screen.Exit();
            return true; // Consume the event and exit
        }
        if (date.month != action.prevMonth || date.year !=
            action.prevYear) {
            action.prevMonth = futureDate.month;
            action.prevYear = futureDate.year;

        if (date.month == 0 ||date.month == 2 || date.month ==4 ||
            date.month == 6 || date.month == 7 ||date.month == 9 ||
            date.month == 11)
        {
            days = returnCalendarMonth(31);
        }
        else if (date.month == 3 || date.month == 5 ||
                 date.month == 8 || date.month == 10)
        {
           days = returnCalendarMonth(30);
        }
        else
        {
            action.leapYear = std::stoi(yearsPast[date.year]);

            if (static_cast<std::chrono::year>(action.leapYear).is_leap())
            {
               days = returnCalendarMonth(29);
            }
            else { days = returnCalendarMonth(28);}
        }
    }
    return false; // Let other events pass through
});};

    changeDropdown(pAction,pastDate,daysPast,layout);
    changeDropdown(dAction,futureDate,daysFuture,dueDate);

    auto combinedDates = ftxui::Container::Horizontal({layout, dueDate});

    screen.Loop(combinedDates);

    pastDate.year = std::stoi(yearsPast[pastDate.year]);
    futureDate.year = std::stoi(yearsFuture[futureDate.year]);
    pastDate.day += 1;
    pastDate.month += 1;
    futureDate.month += 1;
    futureDate.day += 1;

    return {pastDate,futureDate};

}

ftxui::Component taskStartDate(DateInformation::DayMonthYear& dates,const std::vector<std::string>& years,
                               const std::vector<std::string>& days,const std::vector<std::string>& months)
{
    using namespace ftxui;
    Component dayDrop = Dropdown((&days), &dates.day);
    Component monthDrop = Dropdown(&months, &dates.month);
    Component yearDrop = Dropdown(&years, &dates.year);

    auto layout =Container::Vertical({
             Renderer([] {return text("Task start date");}) | ftxui::bold | ftxui::italic,
             Container::Horizontal({
             yearDrop,
             monthDrop,
             dayDrop
             })
         });

    return layout;
}

std::vector<std::string> calculateYears(const DateInformation::DateType dateType)
{
    const std::chrono::year_month_day ymd = floor<std::chrono::days>(std::chrono::system_clock::now());
    const auto presentYear = ymd.year();

    std::vector<std::string> years{};

    switch (dateType)
    {
        case DateInformation::past:
            for (int x{presentYear}; x > (static_cast<int>(presentYear) - 5); --x) {
                years.emplace_back(std::to_string(x));
            }
            break;
        case DateInformation::due:
            for (int x{presentYear}; x < (static_cast<int>(presentYear) + 5); ++x) {
                years.emplace_back(std::to_string(x));
            }
            break;
    }

    return years;

}

std::vector<std::string> returnCalendarMonth(const int count)
{
    static const std::vector<std::string> bigMonthDays{ "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13",
        "14", "15", "16", "17", "18",
        "19", "20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "30",
        "31"};

    return{bigMonthDays.begin(),bigMonthDays.begin() + count};
}

constexpr std::vector<std::string> returnMonthNames()
{
    return {"January", "February", "March", "April", "May", "June", "July",
               "August", "September", "October", "November", "December"};
}


