#ifndef UNTITLED_INTERFACE_H
#define UNTITLED_INTERFACE_H

#include <ErrorHandling.h>

#include "ftxui/component/component_base.hpp"
#include "ftxui/dom/elements.hpp"


namespace Interface
{
    void displayText(const ftxui::Element& printedText);
    void displayText(const ftxui::Component& printedText);
    void extraDetail(TaskManager::DetailType type);
    void successMessage(TaskManager::DetailType type,bool deleteAll = false);
    void changeTaskInput(std::string& descriptionToChange);
    bool userWantsDuplicate(bool isDuplicate);
    void displayTotalList(const std::vector<TaskManager::Task>& tasks);
    bool errorResponse(TaskManager::DetailType type);
    void exceptionErrorMessage(ErrorHandling::ErrorType type,int line);
}


#endif //UNTITLED_INTERFACE_H