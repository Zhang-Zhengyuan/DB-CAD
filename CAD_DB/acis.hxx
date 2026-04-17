#pragma once
#include <string>
class outcome;
void unlock_license();
int initialize_acis();
void terminate_acis(int level);
std::string process(outcome& result);