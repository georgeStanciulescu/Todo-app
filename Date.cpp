#include <Date.h>
#include <future>

#include "Constants.h"
#include "ftxui/component/screen_interactive.hpp"
#include <iostream>

std::vector<DateInformation::DayMonthYear> dateDropdown() {
    using namespace DateInformation;

    const auto yearsPast{calculateYears(past)};
    const auto yearsFuture{calculateYears(due)};

    auto daysPast{returnCalendarMonth(31)};
    auto daysFuture{returnCalendarMonth(31)};

    const auto months {returnMonthNames()};

    DayMonthYear pastDate{};
    DayMonthYear futureDate{};

    ftxui::Element exceedDate{ftxui::text("")};
    auto infoWindow = ftxui::window(ftxui::text("Info") | ftxui::bold,
                                                  ftxui::paragraph(
                                                                    "You can navigate the date dropdown using the mouse \n"
                                                                       "Alternatively,TAB,Enter,and the arrow keys can be used \n"
                                                                       "After you have entered the dates,press Escape to finish \n\n")
                                                                        | ftxui::bold | ftxui::italic);

    auto layout = taskStartDate(pastDate, yearsPast, daysPast, months, past);
    auto dueDate = taskStartDate(futureDate, yearsFuture, daysFuture, months,due);

    const auto info = makeComponent(infoWindow);

    std::cout << "\033[2J\033[H" << std::flush; // used to flush the screen

    auto screen = ftxui::ScreenInteractive::FitComponent();

    DateCalculation pAction{};
    DateCalculation dAction{};

    changeDropdown(pAction, pastDate, daysPast, yearsPast, layout);
    changeDropdown(dAction, futureDate, daysFuture, yearsFuture, dueDate);


    auto combinedDates = ftxui::Container::Horizontal(
        {
            ftxui::Renderer([] { return ftxui::text("    "); }),
            layout,
            ftxui::Renderer([] { return ftxui::separator(); }),
            dueDate
        });

    combineDates(pastDate,futureDate,screen,exceedDate,combinedDates);

    auto total = ftxui::Container::Vertical({
        info,
        combinedDates,
        makeComponent(exceedDate)
    });

    screen.Loop(total);

    pastDate.year = std::stoi(yearsPast[pastDate.year]);
    futureDate.year = std::stoi(yearsFuture[futureDate.year]);
    pastDate.day += 1;
    pastDate.month += 1;
    futureDate.month += 1;
    futureDate.day += 1;

    return {pastDate, futureDate};
}

ftxui::Component taskStartDate(DateInformation::DayMonthYear &dates,
                               const std::vector<std::string> &years,
                               const std::vector<std::string> &days,
                               const std::vector<std::string> &months,
                               const DateInformation::DateType calendarType) {
    using namespace ftxui;
    Component dayDrop = Dropdown((&days), &dates.day);
    Component monthDrop = Dropdown(&months, &dates.month);
    Component yearDrop = Dropdown(&years, &dates.year);

    std::string title{};
    if (calendarType == DateInformation::DateType::past){title = "Task start date";}
    else if (calendarType == DateInformation::DateType::due){title = "Task due date";}
    else{title = "Task completion date";}

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

std::vector<std::string> calculateYears(
    const DateInformation::DateType dateType,const int start ) {
    const std::chrono::year_month_day ymd{
        floor<std::chrono::days>(std::chrono::system_clock::now())
    };
    const auto presentYear = ymd.year();

    std::vector<std::string> years{};

    switch (dateType) {
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
        case DateInformation::end:
            for (int x{start}; x <= (static_cast<int>(presentYear)); ++x) {
                years.emplace_back(std::to_string(x));
            }
            break;
    }

    return years;
}

std::vector<std::string> returnCalendarMonth(const int count) {
    static const std::vector<std::string> bigMonthDays{
        "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13",
        "14", "15", "16", "17", "18",
        "19", "20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "30",
        "31"
    };

    return {bigMonthDays.begin(), bigMonthDays.begin() + count};
}

std::vector<std::string> returnMonthNames() {
    return {
        "January", "February", "March", "April", "May", "June", "July",
        "August", "September", "October", "November", "December"
    };
}

//Kinda hacky,if one's purpose were to use this function outside of the FTXUI-bound,indexed approach
// past.year and future.year are both indices into yearsPast and yearsFuture;thus if they both equal zero,they're the same year
bool isPastGreater(const DateInformation::DayMonthYear &past,
                   const DateInformation::DayMonthYear &future) {
    if (past.year == 0 && future.year == 0) {
        if (past.month > future.month) { return true; }
        if (past.month == future.month && past.day > future.day) {
            return true;
        }
    }
    return false;
}

ftxui::Component makeComponent(ftxui::Element& element)
{
        return ftxui::Renderer([&element] {return element;});
}

void changeDropdown(DateInformation::DateCalculation &action, DateInformation::DayMonthYear &date,
                    std::vector<std::string> &days,
                    const std::vector<std::string> &years,
                    ftxui::Component &input)
{
    input |= ftxui::CatchEvent([&](const ftxui::Event &event) {
        if (date.month != action.prevMonth || date.year !=
            action.prevYear) {
            action.prevMonth = date.month;
            action.prevYear = date.year;

            if (date.month == 0 || date.month == 2 || date.month == 4 ||
                date.month == 6 || date.month == 7 || date.month == 9 ||
                date.month == 11) {
                days = returnCalendarMonth(31);
            } else if (date.month == 3 || date.month == 5 ||
                       date.month == 8 || date.month == 10) {
                days = returnCalendarMonth(30);
            } else {
                action.leapYear = std::stoi(years[date.year]);

                if (static_cast<std::chrono::year>(action.leapYear).
                    is_leap()) {
                    days = returnCalendarMonth(29);
                } else { days = returnCalendarMonth(28); }
            }
        }
        return false; // Let other events pass through
    });
}

void combineDates(DateInformation::DayMonthYear &pastDate, DateInformation::DayMonthYear &futureDate,
                    ftxui::ScreenInteractive& screen,ftxui::Element& exceedDate,ftxui::Component& combinedDates)
{
    combinedDates |= ftxui::CatchEvent(
    [&pastDate,&futureDate,&screen,&exceedDate](const ftxui::Event &event) {
        if (event == ftxui::Event::Escape) {
            screen.Exit();
            return true; // Consume the event and exit
        }
        isPastGreater(pastDate, futureDate)
            ? exceedDate = ftxui::text("\n\nThe due date is earlier than the start date!")
                                       | ftxui::bold | ftxui::color(ftxui::Color::IndianRedBis)
            : exceedDate = ftxui::text("");

        return false;
    });
}

DateInformation::DayMonthYear endDateDropdown(const std::string& startDate) {
    using namespace DateInformation;

    const std::chrono::year_month_day ymd{
        floor<std::chrono::days>(std::chrono::system_clock::now())
    };

    auto [day, month, year]{returnStartDateInt(startDate)};

    auto yearsPast{calculateYears(end,year)};
    auto fullMonths {returnMonthNames()};

    Indices index{  day - 1,month - 1,
            static_cast<int>(static_cast<unsigned int>(ymd.day())),static_cast<int>(static_cast<unsigned int>(ymd.month()))};
    MonthNames months{fullMonths,
              std::vector(fullMonths.begin()+index.startMonth,fullMonths.end()),
                std::vector(fullMonths.begin(),fullMonths.begin() + index.currentMonth),
                  std::vector(fullMonths.begin() + index.startMonth,fullMonths.begin() + index.currentMonth)};

    DayMonthYear taskStart{index.currentDay,index.currentMonth,static_cast<int>(yearsPast.size())};
    DaysPerMonth daysPerMonth{};
    YearChecks yearCheck{std::chrono::year(year).is_leap(),std::chrono::year(year) == ymd.year()};

    int daysInMonth{daysNumberFirstMonth(index.startMonth,yearCheck.isLeap)};

    // the days in the month remaining for the start date and the days in the month up to the date for the end date
    auto taskStartDays = std::vector<std::string>(daysPerMonth.thirtyOne.begin() + index.startDay,daysPerMonth.thirtyOne.begin() + daysInMonth); //
    auto taskEndDays = std::vector<std::string>(daysPerMonth.thirtyOne.begin(),daysPerMonth.thirtyOne.begin() + index.currentDay);

    auto checkedMonths{months.taskEnd};
    auto checkedDays{taskEndDays};

    auto infoWindow = ftxui::window(ftxui::text("Info") | ftxui::bold,
                                                  ftxui::paragraph(
                                                                    "You can navigate the date dropdown using the mouse \n"
                                                                       "Alternatively,TAB,Enter,and the arrow keys can be used \n"
                                                                       "After you have entered the dates,press Escape to finish \n\n")
                                                                        | ftxui::bold | ftxui::italic);

    const auto info = makeComponent(infoWindow);
    auto layout = taskStartDate(taskStart, yearsPast, checkedDays, checkedMonths, end);

    auto finalLayout = ftxui::Container::Horizontal(
        {
            ftxui::Renderer([] { return ftxui::text("               "); }),
            ftxui::Renderer([] { return ftxui::separator(); }),
            layout,
            ftxui::Renderer([] { return ftxui::separator(); }),

        });

    std::cout << "\033[2J\033[H" << std::flush; // used to flush the screen

    auto screen = ftxui::ScreenInteractive::FitComponent();

    DateCalculation pAction{};
    EndDateDropdownPayload payload{screen,finalLayout,yearCheck,months,daysPerMonth,index,taskStart,taskStartDays,taskEndDays,yearsPast,checkedMonths,checkedDays};

    yearCheck.areYearsEqual ? sameYearEndDateDropdown(payload) : differentYearEndDropdown(payload);

    auto total = ftxui::Container::Vertical({
        info,
        finalLayout,
    });

    screen.Loop(total);

    int actualDay{std::stoi(checkedDays[taskStart.day])};

    int actualMonth{1};
    for (int x{0}; x <= fullMonths.size();++x) {
        if (fullMonths[x] == checkedMonths[taskStart.month]) {
            actualMonth += x;
        }
    }

    int chosenYear{std::stoi(yearsPast[taskStart.year])};

    return {actualDay,actualMonth,chosenYear};
}

void sameYearEndDateDropdown(DateInformation::EndDateDropdownPayload& payload)
{

    payload.layout |= ftxui::CatchEvent([&](const ftxui::Event &event) {
         if (event == ftxui::Event::Escape) {
             payload.screen.Exit();
             return true;
         }
         if (payload.yearCheck.areYearsEqual) {
             payload.checkedMonths = payload.months.inner;
             payload.index.currentMonth = payload.months.inner.size() - 1;
             if (payload.taskStart.month == 0){payload.checkedDays = payload.taskStartDays;}
             else if (payload.taskStart.month == payload.index.currentMonth){payload.checkedDays = payload.taskEndDays;}
             else{
                 dateCheckConditions(payload.taskStart,payload.index.startMonth,payload.checkedDays,payload.yearsPast,payload.daysPerMonth);
             }
         }

     return false;
 });
}

 void differentYearEndDropdown(DateInformation::EndDateDropdownPayload& payload)      {

     payload.layout |= ftxui::CatchEvent([&](const ftxui::Event &event) {
         if (event == ftxui::Event::Escape) {
             payload.screen.Exit();
             return true;
         }
         if (payload.taskStart.year == 0) {
            payload.checkedMonths = payload.months.taskStart;
            if (payload.taskStart.month == 0) {
                payload.checkedDays = payload.taskStartDays;
            }
            else{
                dateCheckConditions(payload.taskStart,payload.index.startMonth,payload.checkedDays,payload.yearsPast,payload.daysPerMonth);
            }
        }
         else if (payload.taskStart.year == payload.yearsPast.size() - 1) {
            payload.checkedMonths = payload.months.taskEnd;
            payload.index.currentMonth = payload.months.taskEnd.size() - 1;
            if (payload.taskStart.month == payload.index.currentMonth) {
                payload.checkedDays = payload.taskEndDays;
            }
            else {
                dateCheckConditions(payload.taskStart,0,payload.checkedDays,payload.yearsPast,payload.daysPerMonth);
            }
        }
         else {
            payload.checkedMonths = payload.months.full;
            dateCheckConditions(payload.taskStart,0,payload.checkedDays,payload.yearsPast,payload.daysPerMonth);
        }

     return false;
 });
}

void dateCheckConditions(const DateInformation::DayMonthYear& taskStart,const int monthIndex,
                         std::vector<std::string>& checkedDays,std::vector<std::string>& yearsPast,const DateInformation::DaysPerMonth& daysPerMonth) {

    const auto leapYear = static_cast<std::chrono::year>(std::stoi(yearsPast[taskStart.year]));

    if (taskStart.month == 3 - monthIndex || taskStart.month == 5 - monthIndex || taskStart.month == 8  - monthIndex || taskStart.month == 10  - monthIndex) {checkedDays = daysPerMonth.thirty;}
    else if (taskStart.month ==1  - monthIndex) {
        if (leapYear.is_leap()) {checkedDays = daysPerMonth.twentyNine;}
        else{checkedDays = daysPerMonth.twentyEight; }
    }
    else{checkedDays = daysPerMonth.thirtyOne;}
}

std::vector<int> leapYearBetweenDates(const int start,const int end) {


    std::vector<int> leapYears{};
    for (int x{start};x <= end ;++x ) {
            if (static_cast<std::chrono::year>(x).is_leap()) {
                leapYears.emplace_back(static_cast<int>(x));
            }
    }
    return leapYears;
}

int daysNumberFirstMonth(const int startMonth,const bool isLeap) {

    if (startMonth == 3 || startMonth == 5 || startMonth == 8 || startMonth == 10) {return 30;}
    if (startMonth ==1) {
        if (isLeap){return 29;}
        return 28;
    }
    return 31;
}

DateInformation::DayMonthYear returnStartDateInt(const std::string& startDate) {

    const std::size_t dayEnd{startDate.find_first_of('/')};
    const std::size_t monthEnd{startDate.find_last_of('/')};

    std::string day {startDate.substr(0,dayEnd)};
    std::string month {startDate.substr(dayEnd + 1,monthEnd - dayEnd - 1)};
    std::string year {startDate.substr(monthEnd + 1)};

    return {std::stoi(day),std::stoi(month),std::stoi(year)};

}



