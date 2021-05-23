/*
Class stores bash commands and by executing them stores data in vector
*/
#include <string>
#include <sstream>
#include <vector>
#include <iostream>

class Commands
{
private:
    //Gpu state commands
    void getGpuName(std::string &);
    void getGpuClocks(std::string &);
    void getGpuAllStates(std::string &, std::string);
    void getGpuMemoryUtilization(std::string &);
    //Cpu state commands
    void getCpuProcessorUtilization(std::string &);
    void getCpuState(std::string &);
    void getCpuCoreTemp(std::string &, int);

public:
    Commands(){};
    ~Commands(){};

    bool loadGpuData(std::vector<std::string> *, std::string);
    bool loadGpuName(std::vector<std::string> *);
    bool loadGpuClocks(std::vector<std::string> *);

    bool loadCpuData(std::vector<std::string> *);
    bool loadCpuUtilization(std::vector<std::string> *);
    bool loadCpuCoreTemp(std::vector<std::string> *, std::string);

    void parseGpuNameString(std::vector<std::string> *);
    void parseGpuClocksString(std::vector<std::string> *);
    void parseGpuDataString(std::vector<std::string> *);

    void parseCpuNameString(std::vector<std::string> *);
    void parseCpuString(std::vector<std::string> *);
    void parseCpuUtilizationString(std::vector<std::string> *);
};