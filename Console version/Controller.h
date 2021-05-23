#include "Console.h"
#include "Commands.h"
#include <unistd.h>
class Controller
{
private:
    Gpu gpu_;
    Cpu cpu_;
    Console console_;
    Commands commands_;

public:
    Controller(){};
    ~Controller(){};

    bool Start(bool *, int);
    bool RefreshGpuInfo();
    bool RefreshCpuInfo();
};