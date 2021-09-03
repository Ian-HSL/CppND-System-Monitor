#include "processor.h"
#include <iostream>

double Processor::Utilization() { 
    previousProcessorData_ = currentProcessorData_;

    currentProcessorData_ = LinuxParser::CpuUtilization();

    int PrevTotal{0}, Total{0};

    for(int i{0}; i < 8; i++){
        PrevTotal += previousProcessorData_[i];
        Total += currentProcessorData_[i];
    }

    int totald = Total - PrevTotal;
    int idled = currentProcessorData_[3] + currentProcessorData_[4]
              - previousProcessorData_[3] - previousProcessorData_[4];

    return (totald - idled)/double(totald);
}