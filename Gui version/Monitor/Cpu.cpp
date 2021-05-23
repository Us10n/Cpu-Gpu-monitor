#include "Cpu.h"

void Cpu::intialize(std::vector<std::string> vector)
{
    std::vector<double> procutil;
    std::vector<double> procfreq;
    std::vector<int> proctemp;

    setCpuName(vector[1]);
    setCpuCacheSize(std::stoi(vector[3]));
    setCpuCoreAmount(std::stoi(vector[4]));
    setCpuProcessorAmount(std::stoi(vector[0]) / std::stoi(vector[4]));
    int processors = getCpuProcessorAmount();
    int cores = getCpuCoreAmount();
    for (int i = 0; i < processors; i++)
    {
        procutil.push_back(100 - std::stod(vector[1 + std::stoi(vector[0]) + i]));
        procfreq.push_back(std::stod(vector[2 + i * 4]));
    }
    for (int i = 0; i < cores; i++)
    {
        proctemp.push_back(std::stoi(vector[vector.size() - cores + i]) / 1000);
    }

    setCpuProcessorUtilization(procutil);
    setCpuperthreadfrequence(procfreq);
    setCpuCoreTemperature(proctemp);
}