#include <string>
#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
    char format_time[100];
    int hour, min, sec;
    hour = static_cast<int>(seconds / 3600);
    min = static_cast<int>((seconds - (hour * 3600)) / 60);
    sec = seconds - (hour * 3600) - (min * 60);
    std::sprintf(format_time, "%.2i::%.2i::%.2i", hour, min, sec);
    return format_time; 
}