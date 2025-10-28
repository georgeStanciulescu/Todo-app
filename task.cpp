#include <TaskManager.h>
#include <ProgramLogic.h>
#include <fstream>
#include <ftxui/component/screen_interactive.hpp>
#include <chrono>

int main(const int argc,char* argv[])
{
    return programLogic(argc,argv) == ProgramResult::failure ? EXIT_FAILURE : EXIT_SUCCESS;
}

