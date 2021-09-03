#include <string>
#include <iostream>
#include "format.h"

using std::string;

// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) { 
    int hours, minutes, secs;
    string time;

    hours = seconds / 3600;
    if(hours < 10){
        time = time + std::to_string(0);
    }
    time = time + std::to_string(hours) + ":";

    minutes = (seconds % 3600) / 60;
    if(minutes < 10){
        time = time + std::to_string(0);
    }    
    time = time + std::to_string(minutes) + ":";

    secs = (seconds % 3600) % 60;
    if(secs < 10){
        time = time + std::to_string(0);
    }
    time = time + std::to_string(secs);

    return time; 
}