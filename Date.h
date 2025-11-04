#ifndef UNTITLED_DATE_H
#define UNTITLED_DATE_H

#include "ftxui/component/component.hpp"
#include <vector>

#include "ftxui/component/screen_interactive.hpp"
std::vector<std::string> returnCalendarMonth(int count);

namespace DateInformation {
    struct DateCalculation {
        int prevMonth{-1};
        int prevYear{};
        int leapYear{};
    };

    struct DaysPerMonth {
        const std::vector<std::string> thirtyOne{};
        const std::vector<std::string> thirty {};
        const std::vector<std::string> twentyNine{};
        const std::vector<std::string> twentyEight{};

        DaysPerMonth() : thirtyOne(returnCalendarMonth(31)),thirty(returnCalendarMonth(30)),
                         twentyNine(returnCalendarMonth(29)),twentyEight(returnCalendarMonth(28))
        {
        }
    };

    struct MonthNames {
        std::vector<std::string> full{};
        std::vector<std::string> taskStart{};
        std::vector<std::string> taskEnd{};
        std::vector<std::string> inner{};
    };

    struct DayMonthYear {
        int day{};
        int month{};
        int year{};
    };

    struct Indices {
        int startDay{};
        int startMonth{};
        int currentDay{};
        int currentMonth{};
    };

    struct YearChecks {
        bool isLeap{};
        bool areYearsEqual{};

    };

    //experimental payload deliverer
    struct EndDateDropdownPayload {
        ftxui::ScreenInteractive& screen;
        ftxui::Component& layout;

        YearChecks& yearCheck;
        MonthNames& months;
        DaysPerMonth& daysPerMonth;
        Indices& index;
        DayMonthYear& taskStart;

        std::vector<std::string>& taskStartDays;
        std::vector<std::string>& taskEndDays;
        std::vector<std::string>& yearsPast;

        std::vector<std::string>& checkedMonths;
        std::vector<std::string>& checkedDays;
    };

    enum DateType {
        due,
        past,
        end,
    };

};

std::vector<DateInformation::DayMonthYear> dateDropdown();

ftxui::Component taskStartDate(DateInformation::DayMonthYear &dates,
                               const std::vector<std::string> &years,
                               const std::vector<std::string> &days,
                               const std::vector<std::string> &months,
                               DateInformation::DateType calendarType);

std::vector<std::string> calculateYears(DateInformation::DateType dateType,
                                        int = 0);



std::vector<std::string> returnMonthNames();

bool isPastGreater(const DateInformation::DayMonthYear &past,
                   const DateInformation::DayMonthYear &future);

ftxui::Component makeComponent(ftxui::Element &element);

void changeDropdown(DateInformation::DateCalculation &action,
                    DateInformation::DayMonthYear &date,
                    std::vector<std::string> &days,
                    const std::vector<std::string> &years,
                    ftxui::Component &input);

void combineDates(DateInformation::DayMonthYear &pastDate,
                  DateInformation::DayMonthYear &futureDate,
                  ftxui::ScreenInteractive &screen, ftxui::Element &exceedDate,
                  ftxui::Component &combinedDates);

DateInformation::DayMonthYear endDateDropdown(const std::string& startDate);

void sameYearEndDateDropdown(DateInformation::EndDateDropdownPayload& payload);

void differentYearEndDropdown(DateInformation::EndDateDropdownPayload& payload);

void dateCheckConditions(const DateInformation::DayMonthYear &taskStart,
                         int monthIndex,
                         std::vector<std::string> &checkedDays,std::vector<std::string>& yearsPast,
                         const DateInformation::DaysPerMonth& daysPerMonth);
std::vector<int> leapYearBetweenDates(int start,int end);
int daysNumberFirstMonth(int startMonth,bool isLeap);
DateInformation::DayMonthYear returnStartDateInt(const std::string& startDate);


#endif //UNTITLED_DATE_H
