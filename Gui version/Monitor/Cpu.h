/*
Cpu class is storing data about cpu perfomance such as utilization, frequence and so on;
*/

#include <string>
#include <vector>
#include <ostream>

class Cpu
{
private:
    std::string cpuname_;                          //contains cpu name ex: Intel...
    int cpucoreamount_;                            //contains phisical cores amount
    int cpuprocessoramount_;                       //contains logical cores amount
    std::vector<double> cpuproccessorutilization_; //contains utilization per each logical core
    std::vector<double> cpuproccessorfrequence_;   //contains frequence per each logical core
    std::vector<int> cpucoretemperature_;
    int cpucachesize_; //contains cache sizein megabytes
public:
    Cpu(){};
    ~Cpu(){};

    //getters
    inline std::string getCpuName()
    {
        return cpuname_;
    }
    inline int getCpuCoreAmount()
    {
        return cpucoreamount_;
    }
    inline std::vector<int> getCpuCoreTemperature()
    {
        return cpucoretemperature_;
    }
    inline int getCpuProcessorAmount()
    {
        return cpuprocessoramount_;
    }
    inline std::vector<double> getCpuProcessorUtilization()
    {
        return cpuproccessorutilization_;
    }
    inline std::vector<double> getCpuProcessorFrequence()
    {
        return cpuproccessorfrequence_;
    }
    inline int getCpuCacheSize()
    {
        return cpucachesize_;
    }

    //setters
    inline void setCpuName(std::string &cpuname)
    {
        cpuname_ = cpuname;
    }
    inline void setCpuCoreAmount(int coreamount)
    {
        cpucoreamount_ = coreamount;
    }
    inline void setCpuProcessorAmount(int cpuprocamount)
    {
        cpuprocessoramount_ = cpuprocamount;
    }
    inline void setCpuCoreTemperature(std::vector<int> &cputemp)
    {
        cpucoretemperature_ = cputemp;
    }
    inline void setCpuProcessorUtilization(std::vector<double> &cpuprocutilization)
    {
        cpuproccessorutilization_ = cpuprocutilization;
    }
    inline void setCpuperthreadfrequence(std::vector<double> &cpuprocfrequence)
    {
        cpuproccessorfrequence_ = cpuprocfrequence;
    }
    inline void setCpuCacheSize(int cpucachesize)
    {
        cpucachesize_ = cpucachesize;
    }

    void intialize(std::vector<std::string>);
    friend std::ostream &operator<<(std::ostream &, const Cpu &);
};

inline std::ostream &operator<<(std::ostream &out, const Cpu &cpu)
{
    out << cpu.cpuname_ << '\n'
        << cpu.cpucoreamount_ << '\n'
        << cpu.cpucachesize_ << '\n'
        << cpu.cpuprocessoramount_;
    for (int i = 0; i < cpu.cpuprocessoramount_; i++)
    {
        out << '\n'
            << cpu.cpuproccessorutilization_[i] << '\n'
            << cpu.cpuproccessorfrequence_[i];
    }
    for (int i = 0; i < cpu.cpucoreamount_; i++)
    {
        out << '\n'
            << cpu.cpucoretemperature_[i];
    }

    return out;
}