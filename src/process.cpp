#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>

#include "process.h"
#include "linux_parser.h"

using namespace std;

Process::Process(int pid_) : pid_(pid_){}

int Process::Pid() { return pid_;}

double Process::CpuUtilization() { 
    vector<string> processStat = LinuxParser::GetProcessStat(pid_);
    double totalTime;

    for(int i{13}; i < 17; i++){
        totalTime += std::stof(processStat[i]);
    }

    double seconds = LinuxParser::UpTime() - std::stod(processStat[21]) / sysconf(_SC_CLK_TCK);

    cpuUsage_ = totalTime / sysconf(_SC_CLK_TCK) / seconds;

    return cpuUsage_; 
}

string Process::Command() { 
    ostringstream formattedCommand; 
    string rawCommand = LinuxParser::Command(pid_);

    if(rawCommand.size() > 50){
        formattedCommand << rawCommand.substr(0, 50) << "...";
    } else {
        formattedCommand << rawCommand;
    }
    
    return formattedCommand.str();
}

string Process::Ram() { return LinuxParser::Ram(pid_); }

string Process::User() { return LinuxParser::User(pid_); }

long int Process::UpTime() { return LinuxParser::UpTime(pid_); }

bool Process::operator<(Process const& a) const { 
    return cpuUsage_ < a.cpuUsage_;
}
