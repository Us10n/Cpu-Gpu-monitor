#include "Gpu.h"

void Gpu::intialize(std::vector<std::string> vector)
{
    setGpuId(vector[0]);
    setGpuName(vector[1]);
    setGpuGraphicUsage(std::stod(vector[2]));
    setTemperature(std::stoi(vector[3]));
    setTotalMemory(std::stoi(vector[4]));
    setUsedMemory(std::stoi(vector[5]));
    double tmp = getUsedMemory() * 100 / getTotalMemory();
    setMemoryUsage(tmp);
    setMemoryUtilization(std::stod(vector[6]));
    setGraphicClocks(std::stoi(vector[7]));
    setMemoryClocks(std::stoi(vector[8]));
}