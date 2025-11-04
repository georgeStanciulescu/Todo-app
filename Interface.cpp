#include <Interface.h>
#include <Constants.h>
#include <InterfaceComposition.h>
#include "ftxui/component/component.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include <iostream>

namespace Interface {

    void extraDetail(const TaskManager::DetailType type) {
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
                    text(
                        "[end] -- Add a marker indicating completion,either success or failure")
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
                displayText(makeVbox("Task deletion form: task delete [ID] "));
                break;
            }
            case change: {
                displayText(makeVbox("Task deletion form: task change [ID] "));
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
                        "The app also tracks your progress;it allows you to change the tasks' description,to delete them if necessary,etc.\n"
                        "Soon,I shall add further functionality,via date-keeping and -updating!")
                    | bold
                ));
                break;
            case list: break;
        }
    }

    void successMessage(const TaskManager::DetailType type,const bool deleteAll) {
        using enum TaskManager::DetailType;
        using namespace InterfaceComposition;
        switch (type) {
            case deletion: {
                const std::string listResponse{
                    deleteAll
                        ? "All the tasks have been successfully deleted!"
                        : "The task has been successfully deleted!"
                };
                displayText(makeVbox(listResponse));
                break;
            }
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

    void exceptionErrorMessage(const ErrorHandling::ErrorType type,int line) {
        using enum ErrorHandling::ErrorType;
        using namespace InterfaceComposition;

        if (line > 0){displayText(makeVbox("List of issues:"));}

        switch (type) {
            case outOfRange:
                displayText(makeVbox("The task ID cannot be found! Introduce an ID for a task in the list!"));
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

    void displayText(const ftxui::Element &printedText) {
        using namespace ftxui;

        auto document = printedText;
        auto screen = ftxui::Screen::Create(
            ftxui::Dimension::Fit(document,/*extend_beyond_screen=*/true),
            ftxui::Dimension::Fit(document,/*extend_beyond_screen=*/true));

        Render(screen, document);
        screen.Print();

        std::cout << '\n';
    }

    void displayText(const ftxui::Component& printedText) {
        using namespace ftxui;

        auto document = printedText;
        auto screen = ScreenInteractive::TerminalOutput();

        auto container = Container::Vertical({document});

        //Render(screen, document);
        screen.Loop(container);

        //std::cout << '\n';
    }

    void changeTaskInput(std::string &descriptionToChange) {
        auto input = ftxui::Input(&descriptionToChange,
                                  "Edit task description");
        auto screen = ftxui::ScreenInteractive::TerminalOutput();

        input |= ftxui::CatchEvent([&](const ftxui::Event &event) {
            if (event == ftxui::Event::Return) {
                screen.Exit();
                return true; // Consume the event and exit
            }
            return false; // Let other events pass through
        });
        screen.Loop(input);
    }

    bool userWantsDuplicate(const bool isDuplicate) {
        if (isDuplicate) {
            char duplicateAnswer{};
            std::cout <<
                    "There seems to be a task with the name just entered.Would you like to another task with the same name?(Y/n)";
            std::cin >> duplicateAnswer;

            if (duplicateAnswer == 'n' || duplicateAnswer == 'N') {
                return false;
            }
            if (duplicateAnswer == 'Y' || duplicateAnswer == 'y') {
                return true;
            }
        }

        return true;
    }


    void displayTotalList(const std::vector<TaskManager::Task> &tasks) {
        using namespace ftxui;
        Element table{InterfaceComposition::listTableCreation(tasks)};
        Element progressBar{InterfaceComposition::progressBarCreation(tasks)};

         displayText(vbox(
             text(""),
             paragraph(static_cast<std::string>(Constants::listASCII)) | bold | italic,
             text(""),
             table,
             progressBar | size(HEIGHT,EQUAL,10)
         ));

    }

    bool errorResponse(const TaskManager::DetailType type) {
        extraDetail(type);
        return false;
    }
}



