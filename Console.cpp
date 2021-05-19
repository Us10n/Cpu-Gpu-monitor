#include "Console.h"
#define goto(x, y) printf("%c[%d;%dH", 0x1B, x, y)

bool Console::PrintTable(bool *flags, Cpu *cpu, Gpu *gpu)
{
    system("clear");
    int processors = cpu->getCpuProcessorAmount();
    int cores = cpu->getCpuCoreAmount();
    //gpu table
    if (flags[0])
    {
        try
        {
            std::cout << std::setw(79) << std::setfill('-') << std::left << '+' << "+\n"
                      << std::setw(79) << std::setfill(' ') << std::left << "| Id:            Gpu Name: "
                      << "|\n"
                      << std::setw(79) << std::setfill('-') << std::left << '|' << "|\n"
                      << std::setw(79) << std::setfill(' ') << std::left << "|   Temp      Gpu-Util    Memory-Usage/Util    Graphic-Clocks     Memory-Clocks"
                      << "|\n"
                      << std::setw(79) << std::setfill('=') << std::left << '|' << "|\n"
                      << std::setw(79) << std::setfill(' ') << std::left << '|' << "|\n"
                      << std::setw(79) << std::setfill(' ') << std::left << '|' << "|\n"
                      << std::setw(79) << std::setfill('-') << std::left << '+' << "+\n\n";
        }
        catch (...)
        {
            std::cout << "Print table error";
            return false;
        }
    }
    //cpu table
    if (flags[1])
    {
        try
        {
            std::cout << std::setw(79) << std::setfill('-') << std::left << '+' << "+\n"
                      << std::setw(79) << std::setfill(' ') << std::left << "| Cpu Name:                                                       Cores: "
                      << "|\n"
                      << std::setw(79) << std::setfill('-') << std::left << '|' << "|\n"
                      << std::setw(79) << std::setfill(' ') << std::left << "|   Cpu#    Utililzation    Frequence    Cache    Temp"
                      << "|\n"
                      << std::setw(79) << std::setfill('=') << std::left << '|' << "|\n";
            for (int i = 0; i < processors; i++)
            {
                std::cout << std::setw(79) << std::setfill(' ') << std::left << '|' << "|\n";
            }
            std::cout << std::setw(79) << std::setfill('-') << std::left << '+' << "+\n";
        }
        catch (...)
        {
            std::cout << "Print table error";
            return false;
        }
    }
    printf("%c[%dm", 0x1B, 47);
    printf("%c[%dmPress Ctrl+C to exit program", 0x1B, 30);
    printf("%c[%dm", 0x1B, 0);
    isPrinted = true;
    return true;
}

bool Console::PrintData(bool *flags, Cpu *cpu, Gpu *gpu)
{
    if (flags[0])
    {
        try
        {
            printf("%c[s", 0x1B);
            goto(2, 7);
            std::cout << gpu->getGpuId();
            goto(2, 28);
            std::cout << gpu->getGpuName();
            goto(6, 5);
            std::cout << std::setw(3) << std::setfill(' ') << std::left << gpu->getTemperature() << " C";
            goto(6, 17);
            std::cout << std::setw(6) << std::setfill(' ') << std::left << gpu->getGpuGraphicUsage() << " %";
            goto(6, 34);
            std::cout << std::setw(6) << std::setfill(' ') << std::left << gpu->getMemoryUsage() << " %";
            goto(6, 49);
            std::cout << std::setw(6) << std::setfill(' ') << std::left << gpu->getGraphicClocks() << " MHz";
            goto(6, 67);
            std::cout << std::setw(6) << std::setfill(' ') << std::left << gpu->getMemoryClocks() << " MHz";
            goto(7, 34);
            std::cout << std::setw(6) << std::setfill(' ') << std::left << gpu->getMemoryUtilization() << " %";
            printf("%c[u", 0x1B);
        }
        catch (...)
        {
            printf("Print data error");
            return false;
        }
    }
    if (flags[1])
    {
        int offset;
        if (flags[0])
        {
            offset = 0;
        }
        else
        {
            offset = -9;
        }

        std::vector<double> procutil = cpu->getCpuProcessorUtilization();
        std::vector<double> procfreq = cpu->getCpuProcessorFrequence();
        std::vector<int> proctemp = cpu->getCpuCoreTemperature();
        int cpucores = cpu->getCpuCoreAmount();
        int cpuproc = cpu->getCpuProcessorAmount();
        try
        {
            printf("%c[s", 0x1B);
            goto(11 + offset, 13);
            std::cout << cpu->getCpuName();
            goto(11 + offset, 74);
            std::cout << cpu->getCpuCoreAmount();
            for (int i = 0; i < cpuproc; i++)
            {
                goto(15 + i + offset, 5);
                std::cout << "Cpu%" << i;
                goto(15 + i + offset, 17);
                std::cout << std::setw(5) << std::setfill(' ') << std::left << procutil[i] << " %";
                goto(15 + i + offset, 29);
                std::cout << std::setw(6) << std::setfill(' ') << std::right << std::setprecision(1) << std::fixed << procfreq[i] << " MHz";
                goto(15 + i + offset, 41);
                std::cout << cpu->getCpuCacheSize() << " Mb";
                if (i % (cpuproc / cpucores) == 0)
                {
                    goto(15 + i + offset, 51);
                    std::cout << proctemp[i / (cpuproc / cpucores)] << " C";
                }
            }
            printf("%c[u", 0x1B);
        }
        catch (...)
        {
            printf("Print data error");
            return false;
        }
    }
    return true;
}