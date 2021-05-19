#include <iostream>
#include <iomanip>
#include "Cpu.h"
#include "Gpu.h"

class Console
{
public:
    bool isPrinted = false;
    Console(){};
    ~Console(){};
    bool PrintTable(bool *, Cpu *, Gpu *);
    bool PrintData(bool *, Cpu *, Gpu *);
};