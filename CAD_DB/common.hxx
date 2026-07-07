#pragma once

#include <chrono>
void myerror(std::string_view errmsg);

#define TMDF std::chrono::steady_clock::time_point time_start, time_end
#define TMST time_start = Timer::now()
#define TMED time_end = Timer::now()
#define TMDR Timer::duration(time_start, time_end)

namespace Timer
{
    std::chrono::steady_clock::time_point now();
    double duration(std::chrono::steady_clock::time_point time_start, std::chrono::steady_clock::time_point time_end);
}
