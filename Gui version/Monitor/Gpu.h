/*
Gpu class is storing data about gpu perfomance such as temperature, memory and so on;
*/

#include <iostream>
#include <string>
#include <vector>
#include <ostream>

class Gpu
{
private:
    std::string gpuid_;     //contains gpu id ex: [gpu:0]
    std::string gpuname_;   //contains gpu nvidia name
    double gpuutilization_; //contains gpu utilization in %
    int temperature_;       //contains gpu temperature in Celsium
    int totalmemory_;       //contains total gpu size of memory in megabytes
    int usedmemory_;        //contains used gpu size of memory in megabytes
    double memoryusage_;    //contains gpu memory utilization in %
    double memoryutil_;
    int graphiclocks_; //contains graphic clocks in MHz
    int memoryclocks_; //contains memory clocks in MHz

public:
    Gpu(){};
    ~Gpu(){};

    //getters
    inline std::string getGpuName() { return gpuname_; };
    inline std::string getGpuId() { return gpuid_; };
    inline int getTotalMemory() { return totalmemory_; };
    inline int getUsedMemory() { return usedmemory_; };
    inline double getMemoryUsage() { return memoryusage_; };
    inline double getMemoryUtilization() { return memoryutil_; };
    inline double getGpuUtilization() { return gpuutilization_; };
    inline int getTemperature() { return temperature_; };
    inline int getGraphicClocks() { return graphiclocks_; };
    inline int getMemoryClocks() { return memoryclocks_; };

    //setters
    inline void setGpuName(std::string value) { gpuname_ = value; }
    inline void setGpuId(std::string value) { gpuid_ = value; }
    inline void setTotalMemory(int value) { totalmemory_ = value; }
    inline void setUsedMemory(int value) { usedmemory_ = value; }
    inline void setMemoryUsage(double value) { memoryusage_ = value; }
    inline void setMemoryUtilization(double value) { memoryutil_ = value; }
    inline void setGpuUtilization(double value) { gpuutilization_ = value; }
    inline void setTemperature(int value) { temperature_ = value; }
    inline void setGraphicClocks(int value) { graphiclocks_ = value; };
    inline void setMemoryClocks(int value) { memoryclocks_ = value; };

    void intialize(std::vector<std::string>);
    friend std::ostream &operator<<(std::ostream &, const Gpu &);
};

inline std::ostream &operator<<(std::ostream &out, const Gpu &gpu)
{
    out << gpu.gpuid_ << '\n'
        << gpu.gpuname_ << '\n'
        << gpu.temperature_ << '\n'
        << gpu.memoryusage_ << '\n'
        << gpu.gpuutilization_ << '\n'
        << gpu.memoryutil_ << '\n'
        << gpu.graphiclocks_ << '\n'
        << gpu.memoryclocks_;

    return out;
}