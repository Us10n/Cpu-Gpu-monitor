#include"Gpu.h"
#include"Cpu.h"
#include "Commands.h"
#include <unistd.h>

class Controller
{
private:
    Gpu gpu_;
    Cpu cpu_;
    Commands commands_;

public:
    Controller(){};
    ~Controller(){};

    void Start();
    bool RefreshGpuInfo();
    bool RefreshCpuInfo();
};