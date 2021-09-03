#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>

#include "linux_parser.h"

using std::stod;
using std::string;
using std::to_string;
using std::vector;

/*
 * System Methods
*/

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

double LinuxParser::MemoryUtilization() { 
  int memTotal, memFree, buffers, cached, sReclaimable, shmem;

  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if(key == "MemTotal:"){
          memTotal = std::stoi(value);
        } else if(key == "MemFree:"){
          memFree = std::stoi(value);
        } else if(key == "Buffers:"){
          buffers = std::stoi(value);
        } else if(key == "Cached:"){
          cached = std::stoi(value);
        } else if(key == "SReclaimable:"){
          sReclaimable = std::stoi(value);
        } else if(key == "Shmem:"){
          shmem = std::stoi(value);
        }
      }
    }
  }

  return double(memTotal - memFree - buffers - cached - sReclaimable + shmem) / double(memTotal);
}

long LinuxParser::UpTime() { 
  // look in uptime: first number is system uptime, second is time in idle process

  string sysUptime, idle;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> sysUptime >> idle;
  }
  
  return std::stol(sysUptime); 
}

vector<int> LinuxParser::CpuUtilization() { 
  string line;
  string key;
  string user, nice, sys, idle, iowait, irq, softirq, steal;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> key >> user >> nice >> sys >> idle >> iowait >> irq >> softirq >> steal;

    return std::vector<int>{std::stoi(user), std::stoi(nice), std::stoi(sys), std::stoi(idle), std::stoi(iowait), std::stoi(irq), std::stoi(softirq), std::stoi(steal)};
  }

  return std::vector<int>();
}

int LinuxParser::TotalProcesses() { 
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "processes") {
          return std::stoi(value);
        }
      }
    }
  }

  return std::stoi(value);
}

int LinuxParser::RunningProcesses() { 
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "procs_running") {
          return std::stoi(value);
        }
      }
    }
  }

  return std::stoi(value);
}

/*
 * Process Methods
*/

string LinuxParser::Command(int pid) { 
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  if (filestream.is_open()) {
    if(std::getline(filestream, line)){
      return line;
    }
  }

  return string(); 
}

string LinuxParser::Ram(int pid) { 
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        // I have been advised to use "VmRSS" over "VmSize" as 
        // VmSize is virtual memory size wheras VmRSS is the exact physical memory
        if (key == "VmRSS:") {
          float temp = std::stod(value);

          temp /= 1024;

          std::ostringstream sbuilder;

          // To set to two decimal places. Otherwise, it is too long and makes the time column almost unreadable. 
          sbuilder.precision(2);

          sbuilder << std::fixed << temp;

          return sbuilder.str();
        }
      }
    }
  }  

  return string(); }

string LinuxParser::Uid(int pid) { 
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "Uid:") {
          return value;
        }
      }
    }
  }

  return string();  
}

string LinuxParser::User(int pid) { 
  string line;
  string username, x, id;
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> username >> x >> id) {
        try{
          if (id == Uid(pid)) {
            return username;
          }
          break;
        } catch(std::invalid_argument const&){
          std::cout << "Erronous Line: " << line << std::endl;
          std::cout << "Erronous username: " << username << std::endl;
          std::cout << "Erronous x: " << x << std::endl;
          std::cout << "Erronous ID: " << id << std::endl;
          return string();
        }
      }
    }
  }
  return string(); 
}

long LinuxParser::UpTime(int pid) { 
  string line;
  string uptime;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    for(int i = 0; i < 22; i++){
      linestream >> uptime;
    }

    return LinuxParser::UpTime() - std::stol(uptime)/sysconf(_SC_CLK_TCK);
  }
  return 0; 
}

vector<string> LinuxParser::GetProcessStat(int pid){
  string line;
  string data;
  vector<string> statData;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    while(linestream >> data){
      statData.push_back(data);
    }
  }

  return statData;
}