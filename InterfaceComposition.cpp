#include <InterfaceComposition.h>
#include <Calculations.h>
#include <Constants.h>

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
        colour == Color::Default
            ? values.push_back(
                paragraph(task.description) | bold | size(WIDTH, LESS_THAN, 20))
            //| size(WIDTH,LESS_THAN,50))
            : values.push_back(
                paragraph(task.description) | dim | size(WIDTH, LESS_THAN, 20));
        //| size(WIDTH,LESS_THAN,50));
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
        // used to flush the screen

        ftxui::Components taskContent{};
        std::vector<std::string> ids{};

        int idSelected{0};
        auto tabMenu = ftxui::Menu(&ids, &idSelected) | ftxui::color(
                           ftxui::Color::LightCoral) | ftxui::bold |
                       ftxui::italic;

        for (auto &task: tasks) {
            ids.emplace_back(std::to_string(task.id));

            ftxui::Element status{};
            ftxui::Color descriptionColour{};
            std::string daysLeft{};
            std::string daysOrNot{};
            std::stoi(task.daysLeft) < 0
                ? daysLeft = "†Days late:"
                : daysLeft = "†Days left:";

            if (task.completion == Constants::ongoingMark) {
                status = ftxui::text("ONGOING") | ftxui::bold | ftxui::color(
                             ftxui::Color::Cornsilk1);
                descriptionColour = ftxui::Color::Default;
            } else if (task.completion == Constants::failureMark) {
                status = ftxui::text("FAILURE") | ftxui::bold | ftxui::color(
                             ftxui::Color::Red);
                daysOrNot = "days";
            } else {
                status = ftxui::text("SUCCESS") | ftxui::bold | ftxui::color(
                             ftxui::Color::Green);
                daysOrNot = "days";
            }

            constexpr std::size_t maxSize{60};
            // could be made better;suffices for now
            const auto sizeDescription = task.description.size();

            ftxui::Elements extraPadding{};

            for (std::size_t x{0}; x < sizeDescription / maxSize; ++x) {
                extraPadding.push_back(ftxui::text(""));
            }

            ftxui::Element statusBox = ftxui::hbox(
                ftxui::text("Completion status: "), status);

            taskContent.push_back(ftxui::Renderer(
                [&task,daysLeft,statusBox,daysOrNot,extraPadding] {
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
                                        std::abs(std::stoi(task.daysLeft)))),
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
        auto infoWindow = createListInfoWindow();
        const auto info = makeComponent(infoWindow);
        auto tabContainer = ftxui::Container::Tab(taskContent, &idSelected);

        auto totalContainer = ftxui::Container::Vertical(
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

        tasks.empty()
            ? Interface::displayText(noTaskMessage)
            : screen.Loop(renderer);
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
