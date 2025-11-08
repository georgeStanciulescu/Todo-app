#ifndef UNTITLED_INTERFACECOMPOSITION_H
#define UNTITLED_INTERFACECOMPOSITION_H

#include <format>

#include "TaskManager.h"
#include "ftxui/component/component_base.hpp"
#include "ftxui/dom/elements.hpp"
#include "ftxui/dom/table.hpp"

namespace InterfaceComposition
{
    struct TabContent {
        ftxui::Components taskContent{};
        std::vector<std::string> ids{};
    };

    ftxui::Element makeVbox(const std::string& first);
    ftxui::Element listTableCreation(const std::vector<TaskManager::Task>& tasks);
    void tableDataCalculations(const std::vector<TaskManager::Task>& tasks,std::vector<ftxui::Elements>& tableContent);
    void tableLookInfo(ftxui::Table& table);
    ftxui::Element progressBarCreation(const std::vector<TaskManager::Task>& tasks);
    void fillTableValues(const TaskManager::Task& task,ftxui::Elements& values,const ftxui::Color& colour);
    void tabStatsCreation(const std::vector<TaskManager::Task>& tasks);
    TabContent createTabContent(const std::vector<TaskManager::Task> &tasks);
    ftxui::Element createStatusBox(char completion);
    ftxui::Elements createExtraPadding(std::size_t descriptionSize);
    ftxui::Element createListInfoWindow();
    std::string_view lateOrLeft(int daysLeft);
}

#endif //UNTITLED_INTERFACECOMPOSITION_H