#include <Date.h>

#include "ftxui/component/screen_interactive.hpp"
#include <iostream>
#include <InterfaceComposition.h>
#include <DateCreation.h>

//This file contains two variations for the date dropdown,one for entering the start and expected dates
//for task completion;the other deals with the actual end date,which is also more complex

std::vector<DateInformation::DayMonthYear> dateDropdown() {
    using namespace DateInformation;

    const auto yearsPast{calculateYears(DateType::past)};
    const auto yearsFuture{calculateYears(DateType::due)};

    auto daysPast{returnCalendarMonth(31)};
    auto daysFuture{returnCalendarMonth(31)};

    const auto months{returnMonthNames()};

    DayMonthYear pastDate{};
    DayMonthYear futureDate{};

    ftxui::Element exceedDate{ftxui::text("")};
    auto infoWindow = createDateInfoWindow();

    auto layout = taskStartDate(pastDate, yearsPast, daysPast, months, DateType::past);
    auto dueDate = taskStartDate(futureDate, yearsFuture, daysFuture, months, DateType::due);

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

    return { convertIndices(pastDate,yearsPast),
                convertIndices(futureDate,yearsFuture)};
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

            dateCheckConditions(date.year,date.month+1,days,years,staticDays);
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
            isPastGreater(pastDate,futureDate) ? exceedDate = ftxui::text("\n\nThe due date is earlier than the start date!")| ftxui::bold | ftxui::color(ftxui::Color::IndianRedBis)
                : exceedDate = ftxui::text("");

            return false;
        });
}

DateInformation::DayMonthYear endDateDropdown(const std::string &startDate)
{
    using namespace DateInformation;

    const auto ymd {returnPresentDate()};

    const auto [day, month, year]{returnStartDateInt(startDate)};

    auto yearsPast{calculateYears(DateType::end, year)};
    const auto fullMonths{returnMonthNames()};

    Indices index{day - 1, month - 1,
        static_cast<int>(static_cast<unsigned int>(ymd.day())),
      static_cast<int>(static_cast<unsigned int>(ymd.month()))};
    MonthNames months{      fullMonths,
            std::vector(fullMonths.begin() + index.startMonth, fullMonths.end()),
              std::vector(fullMonths.begin(),fullMonths.begin() + index.currentMonth),
                std::vector(fullMonths.begin() + index.startMonth,fullMonths.begin() + index.currentMonth)};
    DayMonthYear taskStart{index.currentDay - 1, index.currentMonth, static_cast<int>(yearsPast.size() - 1)};
    DaysPerMonth daysPerMonth{};

    int daysInMonth{daysNumberFirstMonth(index.startMonth, year)};

    // the days in the month remaining for the start date and the days in the month up to the date for the end date
    auto taskStartDays = std::vector(daysPerMonth.thirtyOne.begin() + index.startDay,
                                     daysPerMonth.thirtyOne.begin() + daysInMonth);
    auto taskEndDays = std::vector(  daysPerMonth.thirtyOne.begin(),
                                     daysPerMonth.thirtyOne.begin() + index.currentDay);

    //used for updating months and years
    auto checkedMonths{months.taskEnd};
    auto checkedDays{taskEndDays};

    auto infoWindow = createDateInfoWindow();
    const auto info = makeComponent(infoWindow);

    auto layout = taskStartDate(taskStart, yearsPast, checkedDays,checkedMonths, DateType::end);
    auto finalLayout = createDateDropdown(layout);

    std::cout << "\033[2J\033[H" << std::flush; // used to flush the screen

    auto screen = ftxui::ScreenInteractive::FitComponent();

    EndDateDropdownPayload payload{
        screen, finalLayout, months, daysPerMonth, index, taskStart,
        taskStartDays, taskEndDays, yearsPast, checkedMonths, checkedDays
    };

    endDateDropdownUpdate(payload);

    auto total = ftxui::Container::Vertical({info,finalLayout});

    screen.Loop(total);

    return{ std::stoi(checkedDays[taskStart.day]),
        monthMap.at(checkedMonths[taskStart.month]),
        std::stoi(yearsPast[taskStart.year])};
}

void endDateDropdownUpdate(DateInformation::EndDateDropdownPayload &payload)
{
    std::cout << payload.checkedMonths.size() - 1 << '\n';
    std::cout << payload.taskStart.year << " THE YEAR BEGINNING IS:" << '\n';
    std::cout << payload.yearsPast.size() - 1 << "THE SIZE,BTICH IS:" << '\n';
    payload.layout |= ftxui::CatchEvent([&](const ftxui::Event &event)
        {
            if (event == ftxui::Event::Escape) {
                payload.screen.Exit();
                return true;
            }

            int realMonthIndex{};

            if (payload.taskStart.year == payload.yearsPast.size() - 1 && payload.taskStart.year == 0 ) {
                payload.checkedMonths = payload.months.inner;
                realMonthIndex = payload.taskStart.month + payload.index.startMonth + 1;
            }
            else if (payload.taskStart.year == payload.yearsPast.size() - 1) {
                    payload.checkedMonths = payload.months.taskEnd;
                    realMonthIndex = payload.taskStart.month + 1;
                }
            else if (payload.taskStart.year == 0) {
                payload.checkedMonths = payload.months.taskStart;
                realMonthIndex = payload.taskStart.month + payload.index.startMonth + 1;
            }
            else {
                payload.checkedMonths = payload.months.full;
                realMonthIndex = payload.taskStart.month + 1;
            }

            if (payload.taskStart.year == 0 && payload.taskStart.month == 0){payload.checkedDays = payload.taskStartDays;}
            else if ((payload.taskStart.year == payload.yearsPast.size() - 1) && (payload.taskStart.month == payload.checkedMonths.size() - 1 ))
            {
                payload.checkedDays = payload.taskEndDays;
            }
            else{dateCheckConditions(payload.taskStart.year,realMonthIndex,payload.checkedDays,payload.yearsPast,payload.daysPerMonth);}

            return false;
    });

}

void dateCheckConditions(const int yearIndex,const int realMonth,
                         std::vector<std::string> &checkedDays,const std::vector<std::string> &yearsPast,
                         const DateInformation::DaysPerMonth &daysPerMonth)
{
    const auto leapYear = static_cast<std::chrono::year>(std::stoi(yearsPast[yearIndex]));

    if (realMonth == 4 || realMonth == 6 || realMonth == 9 || realMonth == 11)
    {
        checkedDays = daysPerMonth.thirty;
    }
    else if (realMonth == 2)
        {
        if (leapYear.is_leap()) { checkedDays = daysPerMonth.twentyNine; }
        else {checkedDays = daysPerMonth.twentyEight;}
        }
    else{ checkedDays = daysPerMonth.thirtyOne; }
}
