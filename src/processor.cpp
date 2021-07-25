#include "processor.h"
#include "linux_parser.h"
#include <string>
#include <vector>

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
    Init_Usage();
    std::vector<std::string> pid_cpu_usage = LinuxParser::CpuUtilization();
    for(std::string cpu_usage: pid_cpu_usage){
        Total_Usage(std::stof(cpu_usage));
    }
    return Total_Usage(); 
}