#include <Date.h>
#include <fstream>
#include <future>

#include "Constants.h"
#include "InterfaceComposition.h"

struct DayMonthYear
{
    int day{};
    int month{};
    int year{};
};

namespace DateInformation {


    std::vector<std::string> months{
        "January", "February", "March", "April", "May", "June", "July",
        "August", "September", "October", "November", "December"
    };
    std::vector<std::string> february{
        "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13",
        "14", "15", "16", "17", "18",
        "19", "20", "21", "22", "23", "24", "25", "26", "27", "28"
    };
    std::vector<std::string> februaryLeap{
        "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13",
        "14", "15", "16", "17", "18",
        "19", "20", "21", "22", "23", "24", "25", "26", "27", "28", "29"
    };
    std::vector<std::string> standardAmountOfDays{
        "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13",
        "14", "15", "16", "17", "18",
        "19", "20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "30"
    };
    std::vector<std::string> bigAmountOfDays{
        "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13",
        "14", "15", "16", "17", "18",
        "19", "20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "30",
        "31"
};
}

    std::string dateDropdown() {

        using namespace std::chrono;
        using namespace ftxui;
        using namespace DateInformation;

        auto yearsPast = calculateYears(past);
        auto yearsFuture = calculateYears(due);


        DayMonthYear pastDate{};
        DayMonthYear futureDate{};

        std::vector days{bigAmountOfDays};
        std::vector daysFuture{bigAmountOfDays};


    //     ftxui::Component dayDrop = ftxui::Dropdown(&days, &pastDate.day);
    //     ftxui::Component monthDrop = ftxui::Dropdown(&DateInformation::months, &pastDate.month);
    //     ftxui::Component yearDropPast = ftxui::Dropdown(&yearsPast, &pastDate.year);
    //
    //     ftxui::Component dayDropFuture = ftxui::Dropdown(&daysFuture, &futureDate.day);
    //     ftxui::Component monthDropFuture = ftxui::Dropdown(&DateInformation::months, &futureDate.month);
    //     ftxui::Component yearDropFuture = ftxui::Dropdown(
    //     &yearsFuture, &futureDate.year);
    //
    //     std::cout << "\033[2J\033[H" << std::flush; // used to flush the screen
    //
    // auto layout =Container::Vertical({
    //         Renderer([] {return text("Task start date");}) | ftxui::bold | ftxui::italic,
    //         Container::Horizontal({
    //         yearDropPast,
    //         monthDrop,
    //         dayDrop
    //         })
    //     });
    //
    //
    // auto dueDate = Container::Vertical({
    //         Renderer([] {return text("Task due date");}) | ftxui::bold | ftxui::italic,
    //         Container::Horizontal({
    //         yearDropFuture,
    //         monthDropFuture,
    //         dayDropFuture,
    //     })
    // });

    auto layout = taskStartDate(pastDate,yearsPast,days);
    auto dueDate = taskStartDate(futureDate,yearsFuture,daysFuture);

    std::cout << "\033[2J\033[H" << std::flush; // used to flush the screen
    int prevMonthDue = -1;
    int prevYearDue = 0;
    int yearLeapFuture{};
        auto screen = ftxui::ScreenInteractive::FitComponent();
         auto screens = ftxui::ScreenInteractive::FitComponent();
        dueDate |= ftxui::CatchEvent([&](const ftxui::Event &event) {
            if (event == ftxui::Event::Escape) {
                screen.Exit();
                return true; // Consume the event and exit
            }


            //static int prev_month = -1;
           // static int prev_year = 0;
            if (futureDate.month != prevMonthDue || futureDate.year != prevYearDue) {
                prevMonthDue = futureDate.month;
                prevYearDue =  futureDate.year;
                if (futureDate.month == 0 || futureDate.month == 2 ||futureDate.month == 4 ||
                    futureDate.month == 6 || futureDate.month == 7 || futureDate.month == 9 ||
                    futureDate.month == 11) {
                    daysFuture = bigAmountOfDays;
                } else if (futureDate.month == 3 || futureDate.month == 5 || futureDate.month == 8
                           || futureDate.month == 10) {
                    daysFuture = standardAmountOfDays;
                } else {
                    yearLeapFuture = std::stoi(yearsFuture[ futureDate.year]);
                    if (static_cast<year>(yearLeapFuture).is_leap()) {
                        daysFuture = februaryLeap;
                    } else { daysFuture = february; }
                }
            }

            return false; // Let other events pass through
        });

    int prevMonth{-1};
    int prevYear{0};
    int yearLeapPast{};
        layout |= ftxui::CatchEvent([&](const ftxui::Event &event) {
            if (event == ftxui::Event::Escape) {
                screen.Exit();
                return true; // Consume the event and exit
            }
            if (pastDate.month != prevMonth ||  pastDate.year != prevYear) {
                prevMonth = pastDate.month;
                prevYear = pastDate.year;
                if (pastDate.month == 0 || pastDate.month == 2 ||pastDate.month == 4 || pastDate.month == 6 || pastDate.month==
                    7 || pastDate.month == 9 || pastDate.month == 11) {
                    days = bigAmountOfDays;
                } else if (pastDate.month == 3 || pastDate.month == 5 || pastDate.month == 8 || pastDate.month == 10) {
                    days = standardAmountOfDays;
                } else {
                    yearLeapPast = std::stoi(yearsPast[ pastDate.year]);
                    if (static_cast<year>(yearLeapPast).is_leap()) {
                        days = februaryLeap;
                    } else { days = february; }
                }
            }

            return false; // Let other events pass through
        });


        auto combinedDates = ftxui::Container::Horizontal({layout ,dueDate});

         screen.Loop(combinedDates);



    std::cout << "YEARFUTR" << yearsFuture[futureDate.year] << " MONTH:" << DateInformation::months
                [futureDate.month] << " DAY:" << bigAmountOfDays[futureDate.day] << '\n';
    std::cout << "YEAR:" << yearsPast[pastDate.year] << " MONTH:" << DateInformation::months[pastDate.month] <<
            " DAY:" << bigAmountOfDays[pastDate.day] << '\n';

    std::cout << pastDate.year << "      " << pastDate.month << "        " << pastDate.day << '\n';


    return std::format("{} {} {}",bigAmountOfDays[pastDate.day],DateInformation::months[pastDate.month],yearsPast[pastDate.year]);

            // auto dates = returnDates();
            // return dates[0];
    }


ftxui::Component taskStartDate(DayMonthYear& dates,std::vector<std::string>& years,std::vector<std::string>& days)
{
    using namespace ftxui;
    ftxui::Component dayDrop = ftxui::Dropdown((&days), &dates.day);
    ftxui::Component monthDrop = ftxui::Dropdown(&DateInformation::months, &dates.month);
    ftxui::Component yearDrop = ftxui::Dropdown(&years, &dates.year);


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

// void dateChoiceInput(DayMonthYear& dates,std::vector<std::string>& days,std::vector<std::string>& years,
//                                  ftxui::Component& layout,ftxui::ScreenInteractive& screen)
// {
//     int prevMonth{-1};
//     int prevYear{0};
//     int yearLeap{0};
//     layout |= ftxui::CatchEvent([&](const ftxui::Event &event) {
//         if (event == ftxui::Event::Escape) {
//             screen.Exit();
//             return true; // Consume the event and exit
//         }
//         if (dates.month != prevMonth || dates.year != prevYear) {
//             prevMonth = dates.month;
//             prevYear = dates.year;
//             if (dates.month == 0 || dates.month == 2 || dates.month == 4 || dates.month == 6 || dates.month ==
//                 7 || dates.month == 9 || dates.month == 11) {
//                 // *days = DateInformation::bigAmountOfDays;
//                 days.clear();
//                 days.insert(days.begin(),
//             DateInformation::bigAmountOfDays.begin(),
//             DateInformation::bigAmountOfDays.end());
//             } else if (dates.month == 3 || dates.month == 5 || dates.month == 8 || dates.month == 10) {
//                 // *days = DateInformation::standardAmountOfDays;
//                 days.clear();
//                 days.insert(days.begin(),
//             DateInformation::standardAmountOfDays.begin(),
//             DateInformation::standardAmountOfDays.end());
//             } else {
//                 yearLeap = std::stoi(years[dates.year]);
//                 if (is_leap_year(yearLeap)) {
//                     // *days = DateInformation::februaryLeap;
//                     days.clear();
//                     days.insert(days.begin(),
//             DateInformation::februaryLeap.begin(),
//             DateInformation::februaryLeap.end());
//                 } else {
//                     // *days = DateInformation::february;
//                     days.clear();
//                     days.insert(days.begin(),
//             DateInformation::february.begin(),
//             DateInformation::february.end());
//                 }
//             }
//         }
//
//         return false; // Let other events pass through
//     });
//
// }
//
// std::vector<std::string> returnDates() {
//     auto screen = ftxui::ScreenInteractive::FitComponent();
//     DayMonthYear startDate{};
//     DayMonthYear dueDate{};
//
//     // auto startDays = std::make_shared<std::vector<std::string>>(DateInformation::bigAmountOfDays);
//     // auto dueDays   = std::make_shared<std::vector<std::string>>(DateInformation::bigAmountOfDays);
//
//     auto startDays = std::vector<std::string>(DateInformation::bigAmountOfDays);
//     auto dueDays   = std::vector<std::string>(DateInformation::bigAmountOfDays);
//
//     std::string startResult{};
//     std::string endResult{};
//
//     std::cout << "\033[2J\033[H" << std::flush; // used to flush the screen
//
//     auto pastYears = calculateYears(DateInformation::past);
//     auto futureYears = calculateYears(DateInformation::due);
//
//     std::cout << pastYears[2] << '\n';
//
//     auto pastDates = taskStartDate(screen,startDate, DateInformation::past,pastYears,startDays);
//     auto dueDates = taskStartDate(screen,dueDate,DateInformation::due,futureYears,dueDays);
//
//     // ftxui::Component dayDrop = ftxui::Dropdown((&startDays), &startDate.day);
//     // ftxui::Component monthDrop = ftxui::Dropdown(&DateInformation::months, &startDate.month);
//     // ftxui::Component yearDrop = ftxui::Dropdown(&pastYears, &startDate.year);
//     //
//     // auto layout1 = ftxui::Container::Vertical({
//     //         ftxui::Renderer([] {return ftxui::text("Task start date");}) | ftxui::bold | ftxui::italic,
//     //         ftxui::Container::Horizontal({
//     //         yearDrop,
//     //         monthDrop,
//     //         dayDrop
//     //         })
//     //     });
//     //
//     //
//     // ftxui::Component dayDrop1 = ftxui::Dropdown((&dueDays), &dueDate.day);
//     // ftxui::Component monthDrop1 = ftxui::Dropdown(&DateInformation::months, &dueDate.month);
//     // ftxui::Component yearDrop1 = ftxui::Dropdown(&futureYears, &dueDate.year);
//     //
//     // auto layout2 = ftxui::Container::Vertical({
//     //         ftxui::Renderer([] {return ftxui::text("Task start date");}) | ftxui::bold | ftxui::italic,
//     //         ftxui::Container::Horizontal({
//     //         yearDrop1,
//     //         monthDrop1,
//     //         dayDrop1
//     //         })
//     //     });
//
//     //dateChoiceInput(startDate,startDays,pastYears,pastDates,screen);
//     // dateChoiceInput(dueDate,dueDays,futureYears,dueDates,screen);
//
//     //auto combinedDates = ftxui::Container::Horizontal({pastDates ,dueDates});
//
//
//     //combinedDates->TakeFocus();
//
//     // combinedDates |= ftxui::CatchEvent([&](ftxui::Event e) {
//     //     if (e == ftxui::Event::Escape || e == ftxui::Event::Return) {
//     //         startResult = startDays[startDate.day] + " " +
//     //                       DateInformation::months[startDate.month] + " " +
//     //                       pastYears[startDate.year];
//     //
//     //         endResult = dueDays[dueDate.day] + " " +
//     //                     DateInformation::months[dueDate.month] + " " +
//     //                     futureYears[dueDate.year];
//     //
//     //         screen.Exit();
//     //         return true;
//     //     }
//     //     return false;
//     // });
//
//     //screen.Loop(pastDates);
//     //screen.Loop(combinedDates);
//
//     // std::vector<std::string> dates{
//     //     std::format("{} {} {}", DateInformation::bigAmountOfDays.at(startDate.day),
//     //                                     DateInformation::months.at(startDate.month),pastYears.at(startDate.year)),
//     //     std::format("{} {} {}", DateInformation::bigAmountOfDays.at(dueDate.day),
//     //                                     DateInformation::months.at(dueDate.month),futureYears.at(dueDate.year))
//     //
//     // };
//
//     return {startResult, endResult};
//     //return dates;
//
// }
//
std::vector<std::string> calculateYears(DateInformation::DateType dateType)
{
    std::chrono::year_month_day ymd = floor<std::chrono::days>(std::chrono::system_clock::now());
    const std::chrono::year foo = ymd.year();

    std::vector<std::string> years{};

    switch (dateType)
    {
        case DateInformation::past:
            for (int x{foo}; x > (static_cast<int>(foo) - 5); --x) {
                //years.emplace_back(std::format("{}", x));
                years.emplace_back(std::to_string(x));
            }
            break;
        case DateInformation::due:
            for (int x{foo}; x < (static_cast<int>(foo) + 5); ++x) {
                //years.emplace_back(std::format("{}", x));
                years.emplace_back(std::to_string(x));
            }
            break;
    }

    return years;

}

