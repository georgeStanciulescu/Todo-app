#ifndef UNTITLED_INTERFACE_H
#define UNTITLED_INTERFACE_H
#include "ftxui/dom/elements.hpp"
#include <TaskManager.h>

#include "ftxui/component/task.hpp"
#include "ftxui/dom/table.hpp"


void displayText(const ftxui::Element& printedText);
void extraDetail(TaskManager::DetailType type);
void successMessage(TaskManager::DetailType type,bool deleteAll = false);
void changeTaskInput(std::string& descriptionToChange);
void displayTotalList(const std::vector<TaskManager::Task>& tasks);

#endif //UNTITLED_INTERFACE_H