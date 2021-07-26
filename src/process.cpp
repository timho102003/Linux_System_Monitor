#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include "linux_parser.h"
#include "process.h"

using std::string;
using std::to_string;
using std::vector;

// TODO: Return this process's ID
int Process::Pid() {
    return Process::pid_;
}

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { 
    long total_time_sec = LinuxParser::ActiveJiffies(Pid());
    long uptime = LinuxParser::UpTime();
    long start_time = LinuxParser::UpTime(Pid());
    float delta = (float)uptime - (float)start_time;
    float cpu_utilize = (float)total_time_sec / delta;
    return cpu_utilize;
}

// TODO: Return the command that generated this process
string Process::Command() { return LinuxParser::Command(Pid());}

// TODO: Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(Pid()); }

// TODO: Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(Pid()); }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime() - LinuxParser::UpTime(Pid()); }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& p) const {
    return mem_ < p.mem_;
}

bool Process::operator>(Process const& p) const {
    return mem_ > p.mem_;
}