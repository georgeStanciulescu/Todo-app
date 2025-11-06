#include <InterfaceComposition.h>
#include <Calculations.h>
#include <Constants.h>

#include "Interface.h"

namespace InterfaceComposition
{
    ftxui::Element makeVbox(const std::string& first)
    {
        return  ftxui::vbox(
                ftxui::text(" "),
                ftxui::text(first) | ftxui::bold);
    }

    ftxui::Element listTableCreation(const std::vector<TaskManager::Task>& tasks)
    {
        using namespace ftxui;

        std::vector<Elements> tableContent{
                {
                    text("ID") | bold | italic,
                       text("Task") | bold | italic ,
                       text("Status") | bold | italic,
                       text("Due date") | bold | italic,
                       text("Days left") | bold | italic,
                }};

        tableDataCalculations(tasks,tableContent);

        auto table = Table(tableContent);

        tableLookInfo(table);

        return table.Render();
    }

    void tableDataCalculations(const std::vector<TaskManager::Task>& tasks,std::vector<ftxui::Elements>& tableContent)
    {
        using namespace ftxui;
        for (auto& task : tasks)
        {
            Elements values{};

            if (task.completion == Constants::failureMark){fillTableValues(task,values,Color::Red);}
            else if (task.completion == Constants::ongoingMark)  {fillTableValues(task,values,Color::Default);}
            else{fillTableValues(task,values,Color::Green);}

            tableContent.push_back(values);
        }

    }

    void tableLookInfo(ftxui::Table& table)
    {
        using namespace ftxui;

        table.SelectAll().Border(LIGHT);

        for (int x{0}; x < 5;++x) {
            table.SelectColumn(x).Border(LIGHT);
            if (x != 1) {table.SelectColumn(x).DecorateCells(center);}
        }
        table.SelectRow(0).Border(LIGHT);
    }

    //The gauges do not use dividingTasks() because there is no difference to their appearance if totalTasks is zero
    ftxui::Element progressBarCreation(const std::vector<TaskManager::Task>& tasks) {

        using namespace ftxui;

        float cancelledTasks{};
        float runningTasks{};
        float successTasks{};

        for (auto& task : tasks) {
            if (task.completion == Constants::failureMark) {++cancelledTasks;}
            else if (task.completion == Constants::ongoingMark) {++runningTasks;}
            else{++successTasks;}
        }
        const float totalTasks{successTasks + runningTasks + cancelledTasks};

        auto hboxCreator = [&totalTasks](const std::string& label,const float value,const Color& colour) {
                return hbox(
                    window(text(label) | bold | italic,
                        text(std::format("{}%",dividingTasks(value,totalTasks)* 100)) | bold),
                        gauge(dividingTasks(value,totalTasks)) | bold | color(colour) | border
                );
        };

        const auto completed = hboxCreator("Completed",successTasks,Color::DarkGreen);
        const auto uncompleted = hboxCreator("Unfinished",runningTasks,Color::NavajoWhite1);
        const auto failed = hboxCreator("Failed",cancelledTasks,Color::Red);

        return vbox(completed,uncompleted,failed);
    }

    void fillTableValues(const TaskManager::Task& task,ftxui::Elements& values,const ftxui::Color& colour)
    {
        using namespace ftxui;

        values.push_back(text(std::format("{}",task.id)) | bold );
        colour == Color::Default ? values.push_back(paragraph(task.description) | bold | size(WIDTH,LESS_THAN,20)) //| size(WIDTH,LESS_THAN,50))
                                    : values.push_back(paragraph(task.description)  | dim  | size(WIDTH,LESS_THAN,20));//| size(WIDTH,LESS_THAN,50));
        values.push_back(text(std::format("{}",task.completion)) | bold | color(colour));
        values.push_back(text(task.dueDate) | bold);
        colour == Color::Default ? values.push_back(text(task.daysLeft) | bold)
                                    : values.push_back(text(task.daysLeft) | bold | strikethrough | color(colour));
    }

    void tabStatsCreation(const std::vector<TaskManager::Task> &tasks)
    {
        if (!tasks.empty()) std::cout << "\033[2J\033[H" << std::flush; // used to flush the screen

        ftxui::Components taskContent{};
        std::vector<std::string> ids{};

        int idSelected{0};
        auto tabMenu = ftxui::Menu(&ids, &idSelected) | ftxui::color(ftxui::Color::LightCoral) | ftxui::bold | ftxui:: italic;

        for (auto &task: tasks)
        {
            ids.emplace_back(std::to_string(task.id));

            std::string status{};
            if (task.completion == Constants::ongoingMark){status = "ONGOING";}
            else if (task.completion == Constants::failureMark){status = "FAILED";}
            else{status = "SUCCEEDED";}

            //if (std::stoi(task.daysLeft) < 0){}

            taskContent.push_back(ftxui::Renderer([&task,status]
                {
                    return
                            ftxui::vbox(
                                ftxui::paragraph(std::format(
                                    "Description: {}", task.description)),
                                ftxui::text(std::format(
                                    "Completion status: {}", status)),
                                ftxui::text(
                                    std::format("Start date: {}", task.date)),
                                ftxui::text(
                                    std::format("Due date: {}", task.dueDate)),
                                ftxui::text(
                                    std::format(
                                        "Days left: {}", task.daysLeft)),
                                ftxui::text(
                                    std::format("End date: {}", task.endDate)),
                                ftxui::text(std::format(
                                    "S-E diff: {}",
                                    task.startDateEndDateDifference)),
                                ftxui::text(std::format(
                                    "D-E diff: {}",
                                    task.dueDateEndDateDifference))) | ftxui::bold;
                }
            ));
        }
        auto infoWindow = createInfoWindow();
        auto info = makeComponent(infoWindow);
        auto tabContainer = ftxui::Container::Tab({taskContent}, &idSelected);

        auto totalContainer = ftxui::Container::Vertical({tabMenu, tabContainer});

        auto renderer = Renderer(totalContainer, [&] {
            return ftxui::vbox( {
                        infoWindow | ftxui::notflex,
                ftxui::hbox({
                       tabMenu->Render(),
                       ftxui::separator(),
                       tabContainer->Render()
                   }) | ftxui::border
            });

        });

        auto screen = ftxui::ScreenInteractive::FitComponent();

        renderer |= ftxui::CatchEvent([&](const ftxui::Event &event)
        {
            if (event == ftxui::Event::Escape) {
                screen.Exit();
                return true;
            }
            return false;
        });

        const auto noTaskMessage = ftxui::text("There are no tasks to display!") | ftxui::bold;

        tasks.empty() ? Interface::displayText(noTaskMessage): screen.Loop(renderer);
    }

}

