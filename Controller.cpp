#include "Controller.h"

bool Controller::RefreshGpuInfo()
{
    std::vector<std::string> gpudata;
    try
    {
        if (
            commands_.loadGpuName(&gpudata) &&
            commands_.loadGpuData(&gpudata, gpudata[0]) &&
            commands_.loadGpuClocks(&gpudata))
        {
            gpu_.intialize(gpudata);
            return true;
        }
    }
    catch (...)
    {
        return false;
    }
    return false;
}

bool Controller::RefreshCpuInfo()
{
    std::vector<std::string> cpudata;
    try
    {
        if (
            commands_.loadCpuData(&cpudata) &&
            commands_.loadCpuUtilization(&cpudata) &&
            commands_.loadCpuCoreTemp(&cpudata, cpudata[4]))
        {
            cpu_.intialize(cpudata);
            return true;
        }
    }
    catch (...)
    {
        return false;
    }
    return false;
}

bool Controller::Start(bool *flags, int sleeptime)
{
    if (flags[0])
    {
        if (!RefreshGpuInfo())
        {
            std::cout << "Gpu loading information error.\n";
            return false;
        }
    }
    if (flags[1])
    {
        if (!RefreshCpuInfo())
        {
            std::cout << "Cpu loading information error.\n";
            return false;
        }
    }
    if (!console_.isPrinted)
    {
        if (!console_.PrintTable(flags, &cpu_, &gpu_))
        {
            return false;
        }
    }
    if (!console_.PrintData(flags, &cpu_, &gpu_))
    {
        return false;
    }
    return true;
}