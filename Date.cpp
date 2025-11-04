#include <Date.h>
#include <future>

#include "Constants.h"
#include "ftxui/component/screen_interactive.hpp"
#include <iostream>
#include <InterfaceComposition.h>
#include <DateCreation.h>

//This file contains two variations for the date dropdown,one for entering the start and expected dates
//for task completion;the other deals with the actual end date,which is also more complex

std::vector<DateInformation::DayMonthYear> dateDropdown() {
    using namespace DateInformation;

    const auto yearsPast{calculateYears(past)};
    const auto yearsFuture{calculateYears(due)};

    auto daysPast{returnCalendarMonth(31)};
    auto daysFuture{returnCalendarMonth(31)};

    const auto months{returnMonthNames()};

    DayMonthYear pastDate{};
    DayMonthYear futureDate{};

    ftxui::Element exceedDate{ftxui::text("")};
    auto infoWindow = createInfoWindow();

    auto layout = taskStartDate(pastDate, yearsPast, daysPast, months, past);
    auto dueDate = taskStartDate(futureDate, yearsFuture, daysFuture, months,due);

    const auto info = makeComponent(infoWindow);

    std::cout << "\033[2J\033[H" << std::flush; // used to flush the screen

    auto screen = ftxui::ScreenInteractive::FitComponent();

    DateCalculation pAction{};
    DateCalculation dAction{};

    changeDropdown(pAction, pastDate, daysPast, yearsPast, layout);
    changeDropdown(dAction, futureDate, daysFuture, yearsFuture, dueDate);

    auto combinedDates = createDateDropdown(layout,dueDate);
    combineDates(pastDate, futureDate, screen, exceedDate, combinedDates);

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

void changeDropdown(DateInformation::DateCalculation &action,const DateInformation::DayMonthYear &date,
                    std::vector<std::string> &days,const std::vector<std::string> &years,
                    ftxui::Component &input)
{
    static DateInformation::DaysPerMonth staticDays{};

    input |= ftxui::CatchEvent([&](const ftxui::Event &event) {
        if (date.month != action.prevMonth || date.year != action.prevYear)
            {
            action.prevMonth = date.month;
            action.prevYear = date.year;

            if (date.month == 3 || date.month == 5 || date.month == 8 || date.month == 10)
            {
                days = staticDays.thirty;
            }
            else if (date.month == 1){
                action.leapYear = std::stoi(years[date.year]);

                if (static_cast<std::chrono::year>(action.leapYear).is_leap())
                {
                    days = staticDays.twentyNine;
                }
                else { days = staticDays.twentyEight; }
            }
            else{ days = staticDays.thirtyOne;}
        }
        return false; // Let other events pass through
    });
}

void combineDates(DateInformation::DayMonthYear &pastDate,DateInformation::DayMonthYear &futureDate,
                  ftxui::ScreenInteractive &screen, ftxui::Element &exceedDate,
                  ftxui::Component &combinedDates)
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

DateInformation::DayMonthYear endDateDropdown(const std::string &startDate)
{
    using namespace DateInformation;

    const auto ymd {returnPresentDate()};

    const auto [day, month, year]{returnStartDateInt(startDate)};

    auto yearsPast{calculateYears(end, year)};
    const auto fullMonths{returnMonthNames()};

    Indices index{day - 1, month - 1,
        static_cast<int>(static_cast<unsigned int>(ymd.day())),
      static_cast<int>(static_cast<unsigned int>(ymd.month()))};
    MonthNames months{            fullMonths,
        std::vector(fullMonths.begin() + index.startMonth, fullMonths.end()),
          std::vector(fullMonths.begin(),fullMonths.begin() + index.currentMonth),
            std::vector(fullMonths.begin() + index.startMonth,fullMonths.begin() + index.currentMonth)};
    DayMonthYear taskStart{index.currentDay, index.currentMonth, static_cast<int>(yearsPast.size())};
    DaysPerMonth daysPerMonth{};
    YearChecks yearCheck{std::chrono::year(year).is_leap(), std::chrono::year(year) == ymd.year()};

    int daysInMonth{daysNumberFirstMonth(index.startMonth, yearCheck.isLeap)};

    // the days in the month remaining for the start date and the days in the month up to the date for the end date
    auto taskStartDays = std::vector(daysPerMonth.thirtyOne.begin() + index.startDay,
                                     daysPerMonth.thirtyOne.begin() + daysInMonth); //
    auto taskEndDays = std::vector(  daysPerMonth.thirtyOne.begin(),
                                     daysPerMonth.thirtyOne.begin() + index.currentDay);

    //used for updating months and years
    auto checkedMonths{months.taskEnd};
    auto checkedDays{taskEndDays};

    auto infoWindow = createInfoWindow();
    const auto info = makeComponent(infoWindow);

    auto layout = taskStartDate(taskStart, yearsPast, checkedDays,checkedMonths, end);
    auto finalLayout = createDateDropdown(layout);

    std::cout << "\033[2J\033[H" << std::flush; // used to flush the screen

    auto screen = ftxui::ScreenInteractive::FitComponent();

    EndDateDropdownPayload payload{
        screen, finalLayout, yearCheck, months, daysPerMonth, index, taskStart,
        taskStartDays, taskEndDays, yearsPast, checkedMonths, checkedDays
    };

    yearCheck.areYearsEqual ? sameYearEndDateDropdown(payload) : differentYearEndDropdown(payload);

    auto total = ftxui::Container::Vertical({info,finalLayout});

    screen.Loop(total);

    return { returnEndDateInt(fullMonths,checkedDays[taskStart.day],
    checkedMonths[taskStart.month],yearsPast[taskStart.year])};
}

void sameYearEndDateDropdown(DateInformation::EndDateDropdownPayload &payload)
{
    payload.layout |= ftxui::CatchEvent([&](const ftxui::Event &event) {
        if (event == ftxui::Event::Escape) {
            payload.screen.Exit();
            return true;
        }
        if (payload.yearCheck.areYearsEqual) {
            payload.checkedMonths = payload.months.inner;
            payload.index.currentMonth = payload.months.inner.size() - 1;
            if (payload.taskStart.month == 0) {
                payload.checkedDays = payload.taskStartDays;
            } else if (payload.taskStart.month == payload.index.currentMonth) {
                payload.checkedDays = payload.taskEndDays;
            } else {
                dateCheckConditions(payload.taskStart, payload.index.startMonth,
                                    payload.checkedDays, payload.yearsPast,
                                    payload.daysPerMonth);
            }
        }

        return false;
    });
}

void differentYearEndDropdown(DateInformation::EndDateDropdownPayload &payload)
{
    payload.layout |= ftxui::CatchEvent([&](const ftxui::Event &event) {
        if (event == ftxui::Event::Escape) {
            payload.screen.Exit();
            return true;
        }
        if (payload.taskStart.year == 0) {
            payload.checkedMonths = payload.months.taskStart;
            if (payload.taskStart.month == 0) {
                payload.checkedDays = payload.taskStartDays;
            } else {
                dateCheckConditions(payload.taskStart, payload.index.startMonth,
                                    payload.checkedDays, payload.yearsPast,
                                    payload.daysPerMonth);
            }
        } else if (payload.taskStart.year == payload.yearsPast.size() - 1) {
            payload.checkedMonths = payload.months.taskEnd;
            payload.index.currentMonth = payload.months.taskEnd.size() - 1;
            if (payload.taskStart.month == payload.index.currentMonth) {
                payload.checkedDays = payload.taskEndDays;
            } else {
                dateCheckConditions(payload.taskStart, 0, payload.checkedDays,
                                    payload.yearsPast, payload.daysPerMonth);
            }
        } else {
            payload.checkedMonths = payload.months.full;
            dateCheckConditions(payload.taskStart, 0, payload.checkedDays,
                                payload.yearsPast, payload.daysPerMonth);
        }

        return false;
    });
}

void dateCheckConditions(const DateInformation::DayMonthYear &taskStart,const int monthIndex,
                         std::vector<std::string> &checkedDays,const std::vector<std::string> &yearsPast,
                         const DateInformation::DaysPerMonth &daysPerMonth)
{
    const auto leapYear = static_cast<std::chrono::year>(std::stoi(yearsPast[taskStart.year]));

    if (taskStart.month == 3 - monthIndex || taskStart.month == 5 - monthIndex
        || taskStart.month == 8 - monthIndex || taskStart.month == 10 -monthIndex)
    {
        checkedDays = daysPerMonth.thirty;
    }
    else if (taskStart.month == 1 - monthIndex)
        {
        if (leapYear.is_leap()) { checkedDays = daysPerMonth.twentyNine; }
        else {checkedDays = daysPerMonth.twentyEight;}
        }
    else{ checkedDays = daysPerMonth.thirtyOne; }
}
