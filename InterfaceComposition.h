#ifndef UNTITLED_INTERFACECOMPOSITION_H
#define UNTITLED_INTERFACECOMPOSITION_H

#include <format>

#include "TaskManager.h"
#include "ftxui/dom/elements.hpp"
#include "ftxui/dom/table.hpp"

namespace InterfaceComposition
{
    ftxui::Element makeVbox(const std::string& first);

    ftxui::Element listTableCreation(const std::vector<TaskManager::Task>& tasks);
    void tableDataCalculations(const std::vector<TaskManager::Task>& tasks,std::vector<ftxui::Elements>& tableContent);
    void tableLookInfo(ftxui::Table& table);

    ftxui::Element progressBarCreation(const std::vector<TaskManager::Task>& tasks);

}




#endif //UNTITLED_INTERFACECOMPOSITION_H