#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <iostream>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using namespace LinuxParser;
using namespace std;

System::System(){
    vector<int> processID = LinuxParser::Pids();

    for(unsigned int i = 0; i < processID.size(); i++){
        if(LinuxParser::Command(processID[i]) != ""){
            processes_.push_back(processID[i]);
        }
    }
}

// TODO: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() { 
    return processes_; 
}

string System::Kernel() { 
    return LinuxParser::Kernel(); 
}

double System::MemoryUtilization() { 
    return LinuxParser::MemoryUtilization();
}

string System::OperatingSystem() { 
    return LinuxParser::OperatingSystem();
}

int System::RunningProcesses() {
    return LinuxParser::RunningProcesses();
}

int System::TotalProcesses() { 
    return LinuxParser::TotalProcesses();
}

long int System::UpTime() { 
    return LinuxParser::UpTime(); 
}