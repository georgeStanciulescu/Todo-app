#ifndef UNTITLED_DATE_H
#define UNTITLED_DATE_H

#include "ftxui/component/component.hpp"
#include <vector>

#include "DateCreation.h"
#include "ftxui/component/screen_interactive.hpp"


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

    //experimental payload deliverer
    struct EndDateDropdownPayload {
        ftxui::ScreenInteractive& screen;
        ftxui::Component& layout;

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

    enum class DateType {
        due,
        past,
        end,
    };

    inline std::unordered_map<std::string,int> monthMap{
        {"January",1},{"February",2},{"March",3},{"April",4},{"May",5},
        {"June",6},{"July",7},{"August",8},{"September",9},{"October",10},
        {"November",11},{"December",12}
    };

};

std::vector<DateInformation::DayMonthYear> dateDropdown();
void changeDropdown(DateInformation::DateCalculation &action,const DateInformation::DayMonthYear &date,
                    std::vector<std::string> &days,const std::vector<std::string> &years,
                    ftxui::Component &input);
void combineDates(DateInformation::DayMonthYear &pastDate,DateInformation::DayMonthYear &futureDate,
                  ftxui::ScreenInteractive &screen, ftxui::Element &exceedDate,
                  ftxui::Component &combinedDates);


DateInformation::DayMonthYear endDateDropdown(const std::string& startDate);
void sameYearEndDateDropdown(DateInformation::EndDateDropdownPayload& payload);
void differentYearEndDropdown(DateInformation::EndDateDropdownPayload& payload);
void endDateDropdownUpdate(DateInformation::EndDateDropdownPayload &payload);
void dateCheckConditions(int yearIndex,int realMonth,
                         std::vector<std::string> &checkedDays,const std::vector<std::string> &yearsPast,
                         const DateInformation::DaysPerMonth &daysPerMonth);



#endif //UNTITLED_DATE_H
