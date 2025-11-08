#include <InterfaceComposition.h>
#include <Calculations.h>
#include <Constants.h>
#include <iostream>

#include "Interface.h"

namespace InterfaceComposition {
    ftxui::Element makeVbox(const std::string &first) {
        return ftxui::vbox(
            ftxui::text(" "),
            ftxui::text(first) | ftxui::bold);
    }

    ftxui::Element listTableCreation(
        const std::vector<TaskManager::Task> &tasks) {
        using namespace ftxui;

        std::vector<Elements> tableContent{
            {
                text("ID") | bold | italic,
                text("Task") | bold | italic,
                text("Status") | bold | italic,
                text("Due date") | bold | italic,
                text("Days left") | bold | italic,
            }
        };

        tableDataCalculations(tasks, tableContent);

        auto table = Table(tableContent);

        tableLookInfo(table);

        return table.Render();
    }

    void tableDataCalculations(const std::vector<TaskManager::Task> &tasks,
                               std::vector<ftxui::Elements> &tableContent) {
        using namespace ftxui;
        for (auto &task: tasks) {
            Elements values{};

            if (task.completion == Constants::failureMark) {
                fillTableValues(task, values, Color::Red);
            } else if (task.completion == Constants::ongoingMark) {
                fillTableValues(task, values, Color::Default);
            } else { fillTableValues(task, values, Color::Green); }

            tableContent.push_back(values);
        }
    }

    void tableLookInfo(ftxui::Table &table) {
        using namespace ftxui;

        table.SelectAll().Border(LIGHT);

        for (int x{0}; x < 5; ++x) {
            table.SelectColumn(x).Border(LIGHT);
            if (x != 1) { table.SelectColumn(x).DecorateCells(center); }
        }
        table.SelectRow(0).Border(LIGHT);
    }

    //The gauges do not use dividingTasks() because there is no difference to their appearance if totalTasks is zero
    ftxui::Element progressBarCreation(
        const std::vector<TaskManager::Task> &tasks) {
        using namespace ftxui;

        float cancelledTasks{};
        float runningTasks{};
        float successTasks{};

        for (auto &task: tasks) {
            if (task.completion ==
                Constants::failureMark) { ++cancelledTasks; } else if (
                task.completion ==
                Constants::ongoingMark) { ++runningTasks; } else {
                ++successTasks;
            }
        }
        const float totalTasks{successTasks + runningTasks + cancelledTasks};

        auto hboxCreator = [&totalTasks](const std::string &label,
                                         const float value,
                                         const Color &colour) {
            return hbox(
                window(text(label) | bold | italic,
                       text(std::format(
                           "{}%", dividingTasks(value, totalTasks) * 100)) |
                       bold),
                gauge(dividingTasks(value, totalTasks)) | bold | color(colour) |
                border
            );
        };

        const auto completed = hboxCreator("Completed", successTasks,
                                           Color::DarkGreen);
        const auto uncompleted = hboxCreator("Unfinished", runningTasks,
                                             Color::NavajoWhite1);
        const auto failed = hboxCreator("Failed", cancelledTasks, Color::Red);

        return vbox(completed, uncompleted, failed);
    }

    void fillTableValues(const TaskManager::Task &task, ftxui::Elements &values,
                         const ftxui::Color &colour) {
        using namespace ftxui;

        values.push_back(text(std::format("{}", task.id)) | bold);
        //the symbol after the description is used for clarity when reading from the list
        colour == Color::Default
            ? values.push_back(
                paragraph(task.description + " 𝔡")  | bold | size(WIDTH, LESS_THAN, 40))
            : values.push_back(
                paragraph(task.description + " 𝔡")  | dim | size(WIDTH, LESS_THAN, 40));
        values.push_back(
            text(std::format("{}", task.completion)) | bold | color(colour));
        colour == Color::Default
            ? values.push_back(text(task.dueDate) | bold)
            : values.push_back(
                text(task.dueDate) | bold | strikethrough | color(colour));
        colour == Color::Default
            ? values.push_back(text(task.daysLeft) | bold)
            : values.push_back(
                text(task.daysLeft) | bold | strikethrough | color(colour));
    }

    void tabStatsCreation(const std::vector<TaskManager::Task> &tasks) {
        if (!tasks.empty()) std::cout << "\033[2J\033[H" << std::flush;

        const auto tab { createTabContent(tasks)};

        int idSelected{0};
        const auto tabMenu = ftxui::Menu(&tab.ids, &idSelected) | ftxui::color(
                           ftxui::Color::LightCoral) | ftxui::bold | ftxui::italic;

        auto infoWindow = createListInfoWindow();
        const auto info = makeComponent(infoWindow);
        const auto tabContainer = ftxui::Container::Tab(tab.taskContent, &idSelected);


        const auto totalContainer = ftxui::Container::Vertical(
            {tabMenu, tabContainer});

        auto renderer = Renderer(totalContainer, [&] {
            return ftxui::vbox({
                infoWindow | ftxui::notflex,
                ftxui::hbox({
                    tabMenu->Render(),
                    ftxui::separator(),
                    tabContainer->Render(),
                }) | ftxui::border
            });
        });

        auto screen = ftxui::ScreenInteractive::FitComponent();

        renderer |= ftxui::CatchEvent([&](const ftxui::Event &event) {
            if (event == ftxui::Event::Escape) {
                screen.Exit();
                return true;
            }
            return false;
        });

        const auto noTaskMessage =
                ftxui::text("There are no tasks to display!") | ftxui::bold;

        tasks.empty() ? Interface::displayText(noTaskMessage)
                      : screen.Loop(renderer);
    }

    TabContent createTabContent(const std::vector<TaskManager::Task> &tasks)
    {
        TabContent tab{};

         for (const auto &task: tasks) {
            tab.ids.push_back(std::to_string(task.id));

            const int daysLeftNum {std::stoi(task.daysLeft)};

            const std::string_view daysLeft{lateOrLeft(daysLeftNum)};
            std::string_view daysOrNot{};

            if (task.completion != Constants::ongoingMark){daysOrNot = "days";}

            const auto extraPadding = createExtraPadding(task.description.size());

            const auto statusBox{createStatusBox(task.completion)};

            tab.taskContent.push_back(ftxui::Renderer(
                [&task,daysLeft,statusBox,daysOrNot,extraPadding,daysLeftNum] {
                    return
                            ftxui::vbox(
                                ftxui::paragraph(std::format(
                                    "Description: {}", task.description)),
                                statusBox,
                                ftxui::text(
                                    std::format("Start date: {}", task.date)),
                                ftxui::text(
                                    std::format("Due date: {}", task.dueDate)),
                                ftxui::text(
                                    std::format(
                                        "{} {} days", daysLeft,
                                        std::abs(daysLeftNum))),
                                ftxui::text(
                                    std::format("End date: {}", task.endDate)),
                                ftxui::text(std::format(
                                    "Actual time to complete: {} {}",
                                    task.startDateEndDateDifference,
                                    daysOrNot)),
                                ftxui::text(std::format(
                                    "Time over/earlier than expected: {} {}",
                                    task.dueDateEndDateDifference, daysOrNot)),
                                extraPadding) | ftxui::bold;
                }
            ));
        }
        return tab;
    }

    ftxui::Element createStatusBox(const char completion)
    {
        ftxui::Element status{};

        if (completion == Constants::ongoingMark) {
            status = ftxui::text("ONGOING") | ftxui::bold | ftxui::color(
                         ftxui::Color::Cornsilk1);
        } else if (completion == Constants::failureMark) {
            status = ftxui::text("FAILURE") | ftxui::bold | ftxui::color(
                         ftxui::Color::Red);
        } else {
            status = ftxui::text("SUCCESS") | ftxui::bold | ftxui::color(
                         ftxui::Color::Green);
        }

        return ftxui::hbox(ftxui::text("Completion status: "), status);
    }

    ftxui::Elements createExtraPadding(const std::size_t descriptionSize)
    {
        constexpr std::size_t maxSize{60};

        ftxui::Elements extraPadding{};

        for (std::size_t x{0}; x < descriptionSize / maxSize; ++x) {
            extraPadding.push_back(ftxui::text(""));
        }

        return extraPadding;
    }

    std::string_view lateOrLeft(const int daysLeft)
    {
        return daysLeft < 0 ? "† Days late:" : "† Days left:";
    }

    ftxui::Element createListInfoWindow() {
        return ftxui::window(ftxui::text("Info") | ftxui::bold,
                             ftxui::paragraph(
                                 "You can navigate the list using the mouse \n"
                                 "Alternatively,TAB or the arrow keys can be used \n"
                                 "When you are done,press Escape to exit the list \n"
                                 "†-The days left/late,if the task is ongoing;else,the days left\n"
                                 "/late at the time at which the end is recorded\n\n")
                             | ftxui::bold | ftxui::italic);
    }
}
