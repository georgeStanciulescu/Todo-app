#include <ProgramLogic.h>

int main(const int argc,char* argv[])
{
    return programLogic(argc,argv) == ProgramResult::failure ? EXIT_FAILURE : EXIT_SUCCESS;
}

