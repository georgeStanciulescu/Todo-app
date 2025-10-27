#ifndef UNTITLED_ERRORHANDLING_H
#define UNTITLED_ERRORHANDLING_H
#include "TaskManager.h"

class ErrorHandling
{
private:
    const TaskManager& taskManager{};

public:
    explicit ErrorHandling(const TaskManager& tasker)
        :taskManager{tasker}
    {
    }

    bool endErrorHandle(int argc,char* argv[]) const;
    bool deleteErrorHandle(int argc,char* argv[]) const;
    bool addErrorHandle(int argc) const;
    bool changeErrorHandle(int argc,char* argv[]) const;

    bool duplicateCheck(char* argv[]) const;
    bool boundaryCheck(const char* argv) const;

};

#endif //UNTITLED_ERRORHANDLING_H