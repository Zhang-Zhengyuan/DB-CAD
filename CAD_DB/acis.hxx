#pragma once
#include <string>
#include <acis/include/api.hxx>
void unlock_license();
int initialize_acis();
void terminate_acis(int level);
std::string process(outcome& result);
