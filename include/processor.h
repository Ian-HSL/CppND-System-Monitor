#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <iostream>
#include <vector>
#include "linux_parser.h"

class Processor {
 public:
    double Utilization();  

 private:
    std::vector<int> previousProcessorData_{std::vector<int>(8)};
    std::vector<int> currentProcessorData_;
};

#endif