#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>
#include <numeric>
#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  float totalSysMemo, useSysMemo;
  string line, keys, values;
  std::ifstream memoInfoStream(kProcDirectory + kMeminfoFilename);
  if (memoInfoStream.is_open()){
    while(std::getline(memoInfoStream, line)){
      std::istringstream linestream(line);
      while (linestream >> keys >> values) {
        if (keys == "MemTotal:") {
          totalSysMemo = std::stof(values) / 1000;
        }
        else if (keys == "MemFree:"){
          useSysMemo = totalSysMemo - std::stof(values) / 1000;
        }
      }
    }
  }
  return (useSysMemo / totalSysMemo) ;
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
  string line, sys_uptime{"0"}, core_idle_time;
  std::ifstream upTimeStream(kProcDirectory + kUptimeFilename);
  if (upTimeStream.is_open()){
  	std::getline(upTimeStream, line);
  	std::istringstream linestream(line);
  	linestream >> sys_uptime >> core_idle_time;
  }
  // sys_uptime -> already sec
  return std::stol(sys_uptime);
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  string fileName{kProcDirectory + kStatFilename};
  string line, value;
  std::ifstream stream(fileName);
  float tot_jiffie=0;
  bool b_cpu_line=false;
  if (stream.is_open()) {
    while(std::getline(stream, line)){
      std::istringstream linestream(line);
      vector<float> values;
      while(linestream >> value){
        if(value=="cpu"){
          b_cpu_line=true;
          continue;
        }
        if(b_cpu_line){
          values.push_back(std::stof(value));
        } 
      }
      b_cpu_line=false;
      tot_jiffie = std::accumulate(values.begin(), values.end(), 0);
      tot_jiffie = tot_jiffie/(float)sysconf(_SC_CLK_TCK);
      return (long)tot_jiffie;
    }
  }
  return (long)tot_jiffie;
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) {
  string fileName{kProcDirectory + to_string(pid) + kStatFilename};
  std::ifstream stream(fileName);
  vector<string> values;
  if (stream.is_open()) {
    string value;
    while(std::getline(stream, value, ' ')){
      values.push_back(value);
    }
  }
  float total_time = std::stof(values[13]) + std::stof(values[14]);
  total_time = total_time + std::stof(values[15]) + std::stof(values[16]);
  total_time = total_time / (float)sysconf(_SC_CLK_TCK);
  return (long)total_time;
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  string fileName{kProcDirectory + kStatFilename};
  string line, value;
  std::ifstream stream(fileName);
  bool b_cpu_line=false;
  float active_jiffie=0;
  if (stream.is_open()) {
    while(std::getline(stream, line)){
      std::istringstream linestream(line);
      vector<float> values;
      while(linestream >> value){
        if(value=="cpu"){
          b_cpu_line=true;
          continue;
        }
        if(b_cpu_line){
          values.push_back(std::stof(value));
        }
      }
      b_cpu_line=false;
      active_jiffie = (values[1] + values[2] + values[3] + values[6] + values[7] + values[8]) / (float)sysconf(_SC_CLK_TCK);
      return (long)active_jiffie;
    }
  }
  return (long)active_jiffie;
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  string fileName{kProcDirectory + kStatFilename};
  string line, value;
  std::ifstream stream(fileName);
  float idle_jiffie=0;
  if (stream.is_open()) {
    while(std::getline(stream, line)){
      std::istringstream linestream(line);
      bool b_cpu_line=false;
      vector<float> values;
      while(linestream >> value){
        if(value=="cpu"){
          b_cpu_line=true;
          continue;
        }
        if (b_cpu_line){
          values.push_back(std::stof(value));
        }
      }
      b_cpu_line=false;
      idle_jiffie = (values[4] + values[5]) / (float)sysconf(_SC_CLK_TCK);
      return (long)idle_jiffie;
    }
  }
  return (long)idle_jiffie;
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  vector<int> pids = LinuxParser::Pids();
  //stack in percentage
  vector<string> pid_cpu_usage ;
  for (int pid: pids){
    long total_time_sec = LinuxParser::ActiveJiffies(pid);
    long uptime = LinuxParser::UpTime();
    long start_time = LinuxParser::UpTime(pid);
    float delta = (float)uptime - (float)start_time;
    if(delta!=0){
      float cpu_utilize = (float)total_time_sec / delta;
      pid_cpu_usage.push_back(to_string(cpu_utilize));
    }
  } 
  return pid_cpu_usage;
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string line, keys, values;
  int processes{0};
  std::ifstream statStream(kProcDirectory + kStatFilename);
  if (statStream.is_open()){
    while(std::getline(statStream, line)){
      std::istringstream linestream(line);
      while (linestream >> keys >> values) {
        if (keys == "processes") {
          processes = std::stoi(values);
        }
      }
    }
  }
  return processes;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string line, keys, values;
  int processes{0};
  std::ifstream runProcStream(kProcDirectory + kStatFilename);
  if (runProcStream.is_open()){
    while(std::getline(runProcStream, line)){
      std::istringstream linestream(line);
      while (linestream >> keys >> values) {
        if (keys == "procs_running") {
          processes = std::stoi(values);
        }
      }
    }
  }
  return processes;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  string fileName{kProcDirectory + to_string(pid) + kCmdlineFilename};
  string line="";
  std::ifstream cmdStream(fileName);
  if (cmdStream.is_open()){
    std::getline(cmdStream, line);
  }
  return line;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
  string fileName{kProcDirectory + to_string(pid) + kStatusFilename};
  std::ifstream stream(fileName);
  string line, keys, values;
  float mem = 0;
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
        std::istringstream linestream(line);
        while (linestream >> keys >> values) {
        if (keys == "VmSize:") {
          mem = std::stof(values) / 1000;
          break;
        }
      }
    }
  }
  return to_string((int)mem);
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
  string fileName{kProcDirectory + to_string(pid) + kStatusFilename};
  string line="", uid="";
  std::ifstream uidStream(fileName);
  if (uidStream.is_open()){
    vector<string> values={};
    int cnt=0;
    while(std::getline(uidStream, line)){
      std::istringstream linestream(line);
      string value, uid;
      cnt+=1;
      if (cnt>9) break;
      while(linestream >> value){
        if (cnt==9){
          values.push_back(value);
        }
      }
    }
    uid = values[2];
  }
  return uid;
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) { 
  std::ifstream usrStream(kPasswordPath);
  string line, value, usr="";
  if(usrStream.is_open()){
    string uid = LinuxParser::Uid(pid);
    while(std::getline(usrStream, line)){
      vector<string> values={};
      std::istringstream linestream(line);
      while(std::getline(linestream, value, ':')){
        values.push_back(value);
      }
      if (values[2]==uid) usr=values[0];
    }
  }
  return usr;
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {
  string fileName{kProcDirectory + to_string(pid) + kStatFilename};
  std::ifstream uptimeStream(fileName);
  string value;
  float uptime=0.0;
  if (uptimeStream.is_open()){
    vector<string> values;
    while(std::getline(uptimeStream, value, ' ')){
      values.push_back(value);
    }
    uptime = std::stof(values[21]) / (float)sysconf(_SC_CLK_TCK);
  }
  return uptime;
}
