#include <Interface.h>
#include <Constants.h>
#include <InterfaceComposition.h>
#include "ftxui/component/component.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include <iostream>
#include <ranges>

#include "TaskManager.h"

namespace Interface {

    void extraDetail(const TaskManager::DetailType type)
    {
        using namespace ftxui;
        using enum TaskManager::DetailType;
        using namespace InterfaceComposition;

        switch (type) {
            case basic: {
                displayText(ftxui::vbox(
                    text(""),
                    paragraph(static_cast<std::string>(Constants::optionsASCII)) | bold,
                    text(""),
                    text("[list] -- Presents the list of tasks") | bold,
                    text("[add] -- Add a task") | bold,
                    text("[end] -- Add a marker indicating completion,either success or failure")
                    | bold,
                    text("[change] -- Change a task") | bold,
                    text("[info] -- Information regarding the to do app") |
                    bold,
                    text("[delete] -- Delete the task completely") | bold
                ));
                break;
            }
            case add: {
                displayText(
                    makeVbox("Task addition form: task add [description]"));
                break;
            }
            case deletion: {
                displayText(vbox( text(""),
                    text("Task deletion form: task delete [ID]"),
                    text("Or you can chain several ids by using ',' between them : task delete [id,id,id]")) | bold);
                break;
            }
            case change: {
                displayText(makeVbox("Task change form: task change [ID] "));
                break;
            }
            case end: {
                displayText(ftxui::vbox(
                    text(""),
                    text("Task end form: task end [ID] [status] ") | bold,
                    text(" Status can be either x for failure,or s for success")
                    | bold
                ));
                break;
            }
            case info:
                displayText(ftxui::vbox(
                    text("") | bold,
                    paragraph(
                        "The purpose of the to do app is to log whatever task you would like to do.\n"
                        "The app also tracks your progress;it allows you to change the tasks' description,to delete them if necessary,etc.\n")
                    | bold
                ));
                break;
            case list:
                displayText(ftxui::vbox(
                    text(""),
                    text("Task list form: task list [optional extra]") | bold,
                    text("Extra options: -d for the detailed list")
                    | bold
                ));
                break;
        }
    }

    //might change;sending an int pointlessly when the user's not deleting
    void successMessage(const TaskManager::DetailType type)
    {
        using enum TaskManager::DetailType;
        using namespace InterfaceComposition;
        switch (type) {
            case end:
                displayText(makeVbox(
                    "The status of the task has been successfully updated!"));
                break;
            case add:
                displayText(makeVbox("Task successfully added!"));
                break;
            case change:
                displayText(makeVbox(
                    "The task's description has been successfully updated!"));
        }
    }

    void exceptionErrorMessage(const ErrorHandling::ErrorType type,int line)
    {
        using enum ErrorHandling::ErrorType;
        using namespace InterfaceComposition;

        if (line > 0){displayText(makeVbox("List of issues:"));}

        switch (type) {
            case outOfRange:
                displayText(makeVbox("The task ID cannot be found! Introduce an ID for a task in the list!"));
                break;
            case failedCreation:
                displayText(makeVbox("The file has failed to be created!"));
                break;
            case failedOpening:
                displayText(makeVbox("The file has failed to be opened!"));
                break;
            case incorrectArg:
                displayText(makeVbox("The provided arguments are not ids!"));
                break;
            case id:
                displayText(makeVbox(std::format("Line {}:missing or invalid id",line)));
                break;
            case description:
                displayText(makeVbox(std::format("Line {}:malformed description",line)));
                break;
            case status:
                displayText(makeVbox(std::format("Line {}:missing status",line)));
                break;
            case startDate:
                displayText(makeVbox(std::format("Line {}:malformed start date",line)));
                break;
            case dueDate:
                displayText(makeVbox(std::format("Line {}:malformed due date",line)));
                break;
            case daysLeft:
                displayText(makeVbox(std::format("Line {}:malformed present-to-due date calculation",line)));
                break;
            case endDate:
                displayText(makeVbox(std::format("Line {}:malformed end date calculation",line)));
                break;
            case startDateEndDateDifference:
                displayText(makeVbox(std::format("Line {}:malformed start-to-end-date calculation",line)));
                break;
            case dueDateEndDateDifference:
                displayText(makeVbox(std::format("Line {}:malformed due-to-end-date calculation",line)));
                break;
        }
    }

    void displayText(const ftxui::Element &printedText)
    {
        using namespace ftxui;

        auto document = printedText;
        auto screen = ftxui::Screen::Create(
            ftxui::Dimension::Fit(document,/*extend_beyond_screen=*/true),
            ftxui::Dimension::Fit(document,/*extend_beyond_screen=*/true));

        Render(screen, document);
        screen.Print();

        std::cout << '\n';
    }

    void changeTaskInput(std::string &descriptionToChange)
    {
        auto input = ftxui::Input(&descriptionToChange,
                                  "Edit task description");
        auto screen = ftxui::ScreenInteractive::TerminalOutput();

        input |= ftxui::CatchEvent([&](const ftxui::Event &event) {
            if (event == ftxui::Event::Return) {
                screen.Exit();
                return true;
            }
            return false;
        });
        screen.Loop(input);
    }

    bool userWantsDuplicate()
    {
        std::string input{};
        displayText(ftxui::paragraph(
                        "There seems to be a task with the name just entered.\n"
                        "Would you like to add another task with the same name?(Y/n)")
                    | ftxui::bold);

        while (true) {
            std::cin >> input;

            if (input.size() == 1 && (
                    input == "n" || input == "y" ||
                    input == "N" || input == "Y")) {
                break;
            }

            displayText(
                ftxui::text("The only valid answers aresss Y/n") | ftxui::bold);
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        if (input == "n" || input == "N"){return false;}
        return true;
    }


    void displayTotalList(const std::vector<TaskManager::Task> &tasks)
    {
        using namespace ftxui;
        const Element table{InterfaceComposition::listTableCreation(tasks)};
        const Element progressBar{InterfaceComposition::progressBarCreation(tasks)};

         displayText(vbox(
             text(""),
             paragraph(static_cast<std::string>(Constants::listASCII)) | bold | italic,
             text(""),
             table,
             progressBar | size(HEIGHT,EQUAL,10)
         ));
    }

    void deletionMessage(const TaskManager::DeletionType type)
    {

        switch (type) {
            case TaskManager::DeletionType::single:
                displayText(InterfaceComposition::makeVbox(
                    "The task has been successfully deleted!") | ftxui::bold);
                break;
            case TaskManager::DeletionType::multiple:
                displayText(InterfaceComposition::makeVbox(
                    "The tasks have been successfully deleted!") | ftxui::bold);
                break;
            case TaskManager::DeletionType::all:
                displayText(InterfaceComposition::makeVbox(
                    "All the tasks have been successfully deleted!") | ftxui::bold);
                break;
        }

    }

    bool errorResponse(const TaskManager::DetailType type) {
        extraDetail(type);
        return false;
    }

}



