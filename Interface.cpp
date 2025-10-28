#include <Interface.h>
#include <Constants.h>

#include "Calculations.h"
#include "InterfaceComposition.h"
#include "ftxui/component/component.hpp"
#include "ftxui/component/event.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/table.hpp"

void extraDetail(const TaskManager::DetailType type) {

    using namespace ftxui;
    using enum TaskManager::DetailType;
    using namespace InterfaceComposition;

    switch (type)
    {
        case basic: {
            displayText(ftxui::vbox(
                               text(""),
                               paragraph(Constants::optionsASCII) | bold,
                               text(""),
                               text("[list] -- Presents the list of tasks") | bold ,
                               text("[add] -- Add a task") | bold ,
                               text("[end] -- Add a marker indicating completion,either success or failure") | bold ,
                               text("[change] -- Change a task")  | bold,
                               text("[info] -- Information regarding the to do app") | bold,
                               text("[delete] -- Delete the task completely") | bold
                               ));
            break;
        }
        case add:
        {
            displayText(makeVbox("Task addition form: task add [description]"));
            break;
        }
        case deletion:
        {
            displayText(makeVbox("Task deletion form: task delete [ID] "));
            break;
        }
        case change:
        {
            displayText(makeVbox("Task deletion form: task change [ID] "));
            break;
        }
        case end:
        {
            displayText(ftxui::vbox(
                               text(""),
                               text("Task end form: task end [ID] [status] ") | bold,
                               text( " Status can be either x for failure,or s for success") | bold
                        ));
            break;
        }
        case info:
            displayText(ftxui::vbox(
                               text("") | bold,
                               paragraph("The purpose of the to do app is to log whatever task you would like to do.\n"
                               "The app also tracks your progress;it allows you to change the tasks' description,to delete them if necessary,etc.\n"
                               "Soon,I shall add further functionality,via date-keeping and -updating!") | bold
                        ));
            break;
        case list: break;
    }
}
void successMessage(TaskManager::DetailType type,bool deleteAll)
{
    using enum TaskManager::DetailType;
    using namespace InterfaceComposition;
    switch (type)
    {
        case deletion: {
            const std::string listResponse{deleteAll ?  "All the tasks have been successfully deleted!" : "The task has been successfully deleted!"};
            displayText(makeVbox(listResponse));
            break;
        }
        case end:
            displayText(makeVbox("The status of the task has been successfully updated!"));
            break;
        case add:
            displayText(makeVbox("Task successfully added!"));
            break;
        case change:
            displayText(makeVbox("The task's description has been successfully updated!"));
    }
}

void displayText(const ftxui::Element& printedText)
{
    using namespace ftxui;

    auto document = printedText;
    auto screen = ftxui::Screen::Create(ftxui::Dimension::Fit(document,/*extend_beyond_screen=*/true),
                                       ftxui::Dimension::Fit(document,/*extend_beyond_screen=*/true));

    Render(screen, document);
    screen.Print();

    std::cout << '\n';
}

void changeTaskInput(std::string& descriptionToChange)
{
    auto input = ftxui::Input(&descriptionToChange, "Edit task description");
    auto screen = ftxui::ScreenInteractive::TerminalOutput();

    input |= ftxui::CatchEvent([&](const ftxui::Event& event) {
    if (event == ftxui::Event::Return) {
        screen.Exit();
        return true;  // Consume the event and exit
    }
    return false;  // Let other events pass through
});
    screen.Loop(input);
}



void displayTotalList(const std::vector<TaskManager::Task>& tasks)
{
    using namespace ftxui;
    Element table{InterfaceComposition::listTableCreation(tasks) };
    Element progressBar{InterfaceComposition::progressBarCreation(tasks)};

    displayText(vbox(
        text(""),
        paragraph( Constants::listASCII) | bold | italic,
        text(""),
        table,
        progressBar
        ));
}


