#ifndef UNTITLED_DATECREATION_H
#define UNTITLED_DATECREATION_H

#include <vector>
#include <Date.h>

ftxui::Component taskStartDate(DateInformation::DayMonthYear &dates,const std::vector<std::string> &years,
                               const std::vector<std::string> &days,const std::vector<std::string> &months,
                               DateInformation::DateType calendarType);
ftxui::Component makeComponent(ftxui::Element &element);
ftxui::Component createDateDropdown(ftxui::Component& layout);
ftxui::Component createDateDropdown(ftxui::Component& layout,ftxui::Component& dueDate);
ftxui::Element createInfoWindow();

std::vector<std::string> calculateYears(DateInformation::DateType dateType,int start = 0);
std::vector<std::string> returnMonthNames();
std::vector<int> leapYearBetweenDates(int start,int end);

std::chrono::year_month_day returnPresentDate();
bool isPastGreater(const DateInformation::DayMonthYear &past,const DateInformation::DayMonthYear &future);
int daysNumberFirstMonth(int startMonth,bool isLeap);

DateInformation::DayMonthYear returnStartDateInt(const std::string& startDate);
DateInformation::DayMonthYear returnEndDateInt(const std::vector<std::string>& fullMonths,const std::string& day,
                                               const std::string& month,const std::string& year);


#endif //UNTITLED_DATECREATION_H