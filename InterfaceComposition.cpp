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

        std::vector<Elements> tableContent{{ text("ID") | bold | italic,
                                                text("Task") | bold | italic ,
                                                text("Status") | bold | italic,
                                                text("Date started") | bold | italic}};


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
            values.push_back(text(std::format("{}",task.id)) | bold );
            //can be simplified,since std::format is not needed for all

            if (task.completion == Constants::failureMark)
            {
                values.push_back(paragraph(std::format("{}",task.description)) | bold | strikethrough);
                values.push_back(text(std::format("{}",task.completion)) | bold | color(Color::Red));
                task.date.size() == 0 ? values.push_back(text(task.date) | bold | strikethrough) : values.push_back(text(task.date) | bold);
            }
            else if (task.completion == Constants::ongoingMark)
            {
                values.push_back(paragraph(std::format("{}",task.description)) | bold);
                values.push_back(text(std::format("{}",task.completion)) | bold | strikethrough );
                task.date.size() == 0 ? values.push_back(text(task.date) | bold | strikethrough) : values.push_back(text(task.date) | bold);
            }
            else
            {
                values.push_back(paragraph(std::format("{}",task.description)) | bold );
                values.push_back(text(std::format("{}",task.completion)) | bold | color(Color::Green));
                task.date.size() == 0 ? values.push_back(text(task.date) | bold | strikethrough) : values.push_back(text(task.date) | bold);
            }

            tableContent.push_back(values);
        }

    }

    void tableLookInfo(ftxui::Table& table)
    {
        using namespace ftxui;

        table.SelectAll().Border(LIGHT);

        table.SelectColumn(0).Border(LIGHT);
        table.SelectColumn(1).Border(LIGHT);
        table.SelectColumn(2).Border(LIGHT);

        table.SelectRow(0).Border(LIGHT);
        table.SelectColumn(2).DecorateCells(center);
    }

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
        float totalTasks{successTasks + runningTasks + cancelledTasks};

        auto completed = hbox(
            window(text("Completed") | bold | italic,text(std::format("{}%",dividingTasks(successTasks,totalTasks ) * 100)) | bold),
            gauge(successTasks/totalTasks) | bold | color(Color::DarkGreen )| border
            );

        auto uncompleted = hbox(
            window(text("Unfinished") | bold | italic,text(std::format("{}%",dividingTasks(runningTasks,totalTasks ) * 100)) | bold ),
            gauge(runningTasks/totalTasks) | bold | color(Color::NavajoWhite1) | border
        );

        auto failed = hbox(
            window(text("Failed") | bold | italic,text(std::format("{}%",dividingTasks(cancelledTasks,totalTasks ) * 100)) | bold),
            gauge(cancelledTasks/totalTasks) | bold | color(Color::Red) | border

        );

        return vbox(completed,uncompleted,failed);
    }
}
