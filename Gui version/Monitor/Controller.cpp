#include "Controller.h"

bool Controller::RefreshGpuInfo()
{
    std::vector<std::string> gpudata;
    try
    {
        if (
            commands_.loadGpuName(&gpudata) &&
            commands_.loadGpuData(&gpudata, gpudata[0]) &&
            commands_.loadGpuUtilizations(&gpudata) &&
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
        if (!commands_.loadCpuData(&cpudata))
        {
            return false;
        }
        if (!commands_.loadCpuUtilization(&cpudata))
        {
            return false;
        }
        if (!commands_.loadCpuCoreTemp(&cpudata, cpudata[4]))
        {
            for (int i = 0; i < std::stoi(cpudata[4]); i++)
                cpudata.push_back("0");
        }
        cpu_.intialize(cpudata);
        return true;
    }
    catch (...)
    {
        return false;
    }
    return false;
}

void Controller::Start()
{
    bool gpuRefreshStatus, cpuRefreshStatus;

    gpuRefreshStatus = RefreshGpuInfo();
    cpuRefreshStatus = RefreshCpuInfo();

    if (gpuRefreshStatus)
    {
        std::cout << gpu_ << '\n';
    }
    else
    {
        std::cout << "[ERROR] Ошибка загрузки онформации о GPU.\n";
    }

    if (cpuRefreshStatus)
    {
        std::cout << cpu_ << '\n';
    }
    else
    {
        std::cout << "[ERROR] Ошибка загрузки онформации о CPU.\n";
    }
}