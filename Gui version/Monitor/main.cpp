#include "Controller.h"
#include <iostream>
#include<iomanip>

int delay = -1;
bool getFlagsAndDelay(int, char **);

int main(int argc, char *argv[])
{
    if(!getFlagsAndDelay(argc,argv)){
        return 0;
    }
    Controller controller;
    controller.Start();
    return 0;
}

bool getFlagsAndDelay(int argc, char **argv)
{
    if(argc==1){
        return true;
    }
    if (argc > 2)
    {
        std::cout << "Невалидная комбинация входных параметров. Запустите './monitor -h' для подсказки.\n";
        return false;
    }
    ++argv;
    if (argv[0][0] != '-')
    {
        std::cout << "Невалидная комбинация входных параметров. Запустите './monitor -h' для подсказки.\n";
        return false;
    }

    switch (argv[0][1])
    {
    case 'h':
    {
        if (argv[0][2])
        {
            std::cout << "Неизвестный аргумент" << *argv << ". Запустите './monitor -h' для подсказки.\n";
            return false;
        }
        std::cout << "CPU/GPU Monitoring (No GUI)\nПрограмма собирает информацию об оборудовании и делает простой вывод.\n"
        <<"Основная задача программы предоставлять данные для отображения в \"MonitorGui\"\n\n"
                  << "[ОПЦИИ]:\n"
                  << std::setw(15) << std::right << "-h:"
                  << "       Печать подсказки и выход\n"
                  << std::setw(15) << std::right << "без параметров:"
                  << "       Единожды выводит данные о CPU/GPU\n";
        return false;
    }
    break;
    default:
    {
        std::cout << "Неизвестный аргумент" << *argv << ". Запустите './monitor -h' для подсказки.\n";
        return false;
    }
    break;
    }
}