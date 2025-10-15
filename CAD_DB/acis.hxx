#pragma once
#include <string>
void unlock_license();
int initialize_acis();
void terminate_acis(int level);
std::string process(outcome& result);