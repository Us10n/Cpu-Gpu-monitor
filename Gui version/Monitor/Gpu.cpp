#include "Gpu.h"

void Gpu::intialize(std::vector<std::string> vector)
{
    setGpuId(vector[0]);
    setGpuName(vector[1]);
    setTemperature(std::stoi(vector[2]));
    setTotalMemory(std::stoi(vector[3]));
    setUsedMemory(std::stoi(vector[4]));
    double tmp = getUsedMemory() * 100 / getTotalMemory();
    setMemoryUsage(tmp);
    setMemoryUtilization(std::stod(vector[5]));
    setGpuUtilization(std::stod(vector[6]));
    setMemoryClocks(std::stoi(vector[7]));
    setGraphicClocks(std::stoi(vector[8]));

}