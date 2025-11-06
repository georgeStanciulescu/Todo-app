#ifndef UNTITLED_DATECREATION_H
#define UNTITLED_DATECREATION_H

#include <vector>
#include <chrono>
#include "ftxui/component/loop.hpp"

namespace DateInformation {
    struct DayMonthYear;
    enum class DateType;
}

// #include <Date.h>

ftxui::Component taskStartDate(DateInformation::DayMonthYear &dates,const std::vector<std::string> &years,
                               const std::vector<std::string> &days,const std::vector<std::string> &months,
                               DateInformation::DateType calendarType);
ftxui::Component makeComponent(ftxui::Element &element);
ftxui::Component makeComponentVALUE(ftxui::Element element);
ftxui::Component createDateDropdown(ftxui::Component& layout);
ftxui::Component createDateDropdown(ftxui::Component& layout,ftxui::Component& dueDate);
ftxui::Element createDateInfoWindow();

std::vector<std::string> calculateYears(DateInformation::DateType dateType,int start = 0);
std::vector<std::string> returnCalendarMonth(int count);
std::vector<std::string> returnMonthNames();
std::vector<int> leapYearBetweenDates(int start,int end);


bool isPastGreater(DateInformation::DayMonthYear &past,DateInformation::DayMonthYear &future);
int daysNumberFirstMonth(int startMonth, int year);
std::chrono::year_month_day returnPresentDate();

DateInformation::DayMonthYear returnStartDateInt(const std::string& startDate);
DateInformation::DayMonthYear convertIndices(const DateInformation::DayMonthYear& date,const std::vector<std::string>& years);


#endif //UNTITLED_DATECREATION_H