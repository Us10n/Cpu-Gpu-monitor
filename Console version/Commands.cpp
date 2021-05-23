#include "Commands.h"

void Commands::getGpuName(std::string &command)
{
    command = "nvidia-settings -t -q gpus | sed -e 's/^ *//'";
}
void Commands::getGpuAllStates(std::string &command, std::string gpuid)
{
    command = "nvidia-settings -t -q " + gpuid + "/GPUUtilization -q " + gpuid + "/GPUCoreTemp -q " + gpuid + "/TotalDedicatedGPUMemory -q " + gpuid + "/UsedDedicatedGPUMemory";
}
void Commands::getGpuMemoryUtilization(std::string &command)
{
    command = "nvidia-smi --query-gpu=utilization.memory --format=csv | sed 's/[^0-9.]//g'";
}
void Commands::getGpuClocks(std::string &command)
{
    command = "nvidia-smi --query-gpu=clocks.mem,clocks.gr --format=csv,nounits,noheader";
}

void Commands::getCpuProcessorUtilization(std::string &command)
{
    command = "top 1 -b -i -n 1 | grep Cpu | tr ',' '.'";
}
void Commands::getCpuCoreTemp(std::string &command, int coreamount)
{
    std::stringstream sstream;
    sstream << "cat /sys/bus/platform/devices/coretemp.0/hwmon/hwmon*/temp[2-" << coreamount << "]_input";
    command = sstream.str();
}
void Commands::getCpuState(std::string &command)
{
    command = "cat /proc/cpuinfo | grep -P 'cache size|cpu MHz|cpu cores|model name'";
}

bool Commands::loadGpuClocks(std::vector<std::string> *vector)
{
    int size = vector->size();
    std::string command;
    getGpuClocks(command);
    FILE *in;
    char buff[512];

    if (!(in = popen(command.c_str(), "r")))
    {
        return false;
    }
    if (fgets(buff, sizeof(buff), in))
    {
        vector->push_back(buff);
    }
    if (size == vector->size())
    {
        pclose(in);
        return false;
    }
    parseGpuClocksString(vector);

    pclose(in);
    return true;
}
bool Commands::loadGpuName(std::vector<std::string> *vector)
{
    int size = vector->size();
    std::string command;
    getGpuName(command);
    FILE *in;
    char buff[512];

    if (!(in = popen(command.c_str(), "r")))
    {
        return false;
    }
    while (fgets(buff, sizeof(buff), in) != NULL)
    {
        if (buff[0] == '[')
        {
            vector->push_back(buff);
        }
    }
    if (size == vector->size())
    {
        pclose(in);
        return false;
    }
    parseGpuNameString(vector);

    pclose(in);
    return true;
}
bool Commands::loadGpuData(std::vector<std::string> *vector, std::string gpuid)
{
    int size = vector->size();
    std::string command;
    getGpuAllStates(command, gpuid);
    FILE *in;
    char buff[512];

    if (!(in = popen(command.c_str(), "r")))
    {
        return false;
    }

    while (fgets(buff, sizeof(buff), in) != NULL)
    {
        vector->push_back(buff);
    }
    if (size == vector->size())
    {
        pclose(in);
        return false;
    }
    parseGpuDataString(vector);

    getGpuMemoryUtilization(command);

    if (!(in = popen(command.c_str(), "r")))
    {
        return false;
    }

    while (fgets(buff, sizeof(buff), in) != NULL)
    {
        if (buff[0] >= '0' && buff[0] <= '9')
        {
            vector->push_back(buff);
        }
    }
    if (size == vector->size())
    {
        pclose(in);
        return false;
    }
    parseCpuString(vector);

    pclose(in);
    return true;
}

void Commands::parseGpuDataString(std::vector<std::string> *vector)
{
    std::string rawstring;
    std::string graphicutil;

    rawstring = (*vector)[vector->size() - 4];

    size_t startposofid = rawstring.find_first_of("=");
    size_t endposofid = rawstring.find_first_of(",");
    graphicutil = rawstring.substr(startposofid + 1, endposofid - startposofid - 1);

    (*vector)[vector->size() - 4] = graphicutil;

    for (int i = 3; i >= 1; i--)
    {
        rawstring = (*vector)[vector->size() - i];
        //rawstring[rawstring.size() - 1] = '\0';
        rawstring.resize(rawstring.size() - 1);
        (*vector)[vector->size() - i] = rawstring;
    }
}
void Commands::parseGpuNameString(std::vector<std::string> *vector)
{
    std::string rawstring;
    std::string gpuid;
    std::string gpuname;

    rawstring = (*vector)[vector->size() - 1];
    vector->pop_back();

    size_t startposofid = rawstring.find_last_of("[");
    size_t endposofid = rawstring.find_last_of("]");
    gpuid = rawstring.substr(startposofid, endposofid - startposofid + 1);

    size_t startposofname = rawstring.find_last_of("(");
    size_t endposofname = rawstring.find_last_of(")");
    gpuname = rawstring.substr(startposofname + 1, endposofname - startposofname - 1);

    vector->push_back(gpuid);
    vector->push_back(gpuname);
}
void Commands::parseGpuClocksString(std::vector<std::string> *vector)
{
    std::string rawstring;
    std::string memclock;
    std::string graphclock;

    rawstring = (*vector)[vector->size() - 1];
    vector->pop_back();

    size_t endposofmem = rawstring.find_first_of(",");
    memclock = rawstring.substr(0, endposofmem);

    size_t startposofgraph = rawstring.find_last_of(",");
    size_t endposofgraph = rawstring.find_last_of("\n");
    graphclock = rawstring.substr(startposofgraph + 2, endposofgraph - startposofgraph - 1);
    //graphclock[graphclock.size() - 1] = '\0';
    graphclock.resize(graphclock.size() - 1);
    vector->push_back(graphclock);
    vector->push_back(memclock);
}

bool Commands::loadCpuUtilization(std::vector<std::string> *vector)
{
    int size = vector->size();
    std::string command;
    getCpuProcessorUtilization(command);
    FILE *in;
    char buff[512];

    if (!(in = popen(command.c_str(), "r")))
    {
        return false;
    }

    while (fgets(buff, sizeof(buff), in) != NULL)
    {
        vector->push_back(buff);
        parseCpuUtilizationString(vector);
    }
    if (size == vector->size())
    {
        pclose(in);
        return false;
    }
    pclose(in);
    return true;
}
bool Commands::loadCpuCoreTemp(std::vector<std::string> *vector, std::string cores)
{
    int size = vector->size();
    std::string command;
    getCpuCoreTemp(command, std::stoi(cores) + 1);
    FILE *in;
    char buff[512];

    if (!(in = popen(command.c_str(), "r")))
    {
        return false;
    }

    while (fgets(buff, sizeof(buff), in) != NULL)
    {
        vector->push_back(buff);
        parseCpuString(vector);
    }
    if (size == vector->size())
    {
        pclose(in);
        return false;
    }

    pclose(in);
    return true;
}
bool Commands::loadCpuData(std::vector<std::string> *vector)
{
    vector->push_back("0");
    int size = vector->size();
    std::string command;
    getCpuState(command);
    FILE *in;
    char buff[512];

    if (!(in = popen(command.c_str(), "r")))
    {
        return false;
    }
    int amount = 0;
    while (fgets(buff, sizeof(buff), in) != NULL)
    {
        ++amount;
        vector->push_back(buff);
        parseCpuNameString(vector);
    }
    if (size == vector->size())
    {
        pclose(in);
        return false;
    }
    std::stringstream ss;
    ss << amount;
    (*vector)[0] = ss.str();
    pclose(in);
    return true;
}

void Commands::parseCpuNameString(std::vector<std::string> *vector)
{
    std::string namestr;
    namestr = (*vector)[vector->size() - 1];
    vector->pop_back();
    size_t startname = namestr.find_first_of(":");
    size_t endname = namestr.find_last_of("\n");
    namestr = namestr.substr(startname + 2, startname - endname - 1);
    //namestr[namestr.size() - 1] = '\0';
    namestr.resize(namestr.size() - 1);
    vector->push_back(namestr);
}
void Commands::parseCpuString(std::vector<std::string> *vector)
{
    std::string namestr;
    namestr = (*vector)[vector->size() - 1];
    vector->pop_back();
    //namestr[namestr.size() - 1] = '\0';
    namestr.resize(namestr.size() - 1);
    vector->push_back(namestr);
}
void Commands::parseCpuUtilizationString(std::vector<std::string> *vector)
{
    std::string utilstr;
    utilstr = (*vector)[vector->size() - 1];
    vector->pop_back();
    size_t startid = utilstr.find_first_of("d");
    size_t util;
    for (int i = 3;; i++)
    {
        if (utilstr[startid - i] == ' ')
        {
            util = (size_t)(startid - i + 1);
            break;
        }
        if (utilstr[startid - i] == 'i')
        {
            util = (size_t)(startid - i + 2);
            break;
        }
    }
    utilstr = utilstr.substr(util, startid - util - 2);
    vector->push_back(utilstr);
}
