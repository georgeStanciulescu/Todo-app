#include <InterfaceComposition.h>

#include "Calculations.h"
#include "Constants.h"

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
                       text("Date started") | bold | italic,
                       text("Due date") | bold | italic,
                       text("Days left") | bold | italic
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
        }

        table.SelectRow(0).Border(LIGHT);
        table.SelectColumn(5).DecorateCells(center);
        table.SelectColumn(2).DecorateCells(center);
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

        auto hboxCreator = [&](const std::string& label,const float value,const Color& colour) {
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
        colour == Color::Default ? values.push_back(paragraph(task.description) | bold )
                                    : values.push_back(paragraph(task.description) | bold | strikethrough);
        values.push_back(text(std::format("{}",task.completion)) | bold | color(colour));
        values.push_back(text(task.date) | bold);
        values.push_back(text(task.dueDate) | bold);
        colour == Color::Default ? values.push_back(text(task.daysLeft) | bold)
                                    : values.push_back(text(task.daysLeft) | bold | strikethrough | color(colour));
    }
}

