#ifndef UNTITLED_INTERFACE_H
#define UNTITLED_INTERFACE_H

#include <ErrorHandling.h>

#include "ftxui/component/component_base.hpp"
#include "ftxui/dom/elements.hpp"


namespace Interface
{
    void displayText(const ftxui::Element& printedText);
    void extraDetail(TaskManager::DetailType type);
    void successMessage(TaskManager::DetailType type,TaskManager::DeletionType deletionType = TaskManager::DeletionType::single);
    void changeTaskInput(std::string& descriptionToChange);
    bool userWantsDuplicate();
    void displayTotalList(const std::vector<TaskManager::Task>& tasks);
    void deletionMessage(TaskManager::DeletionType type);
    bool errorResponse(TaskManager::DetailType type);
    void exceptionErrorMessage(ErrorHandling::ErrorType type,int line);
}


#endif //UNTITLED_INTERFACE_H