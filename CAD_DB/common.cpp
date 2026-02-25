#include <iostream>
#include <string>
#include <chrono>
#include "common.hxx"

void myerror(std::string_view errmsg)
{
    std::string err(errmsg);
    std::cerr << "*** Error: " << err << std::endl;
    // 抛出异常，让外部调用者（比如新的GUI或后端服务）去捕获并处理
    throw std::runtime_error(err);
}

std::chrono::steady_clock::time_point Timer::now()
{
    return std::chrono::steady_clock::now();
}

double Timer::duration(std::chrono::steady_clock::time_point time_start, std::chrono::steady_clock::time_point time_end)
{
    return std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(time_end - time_start).count();
}
