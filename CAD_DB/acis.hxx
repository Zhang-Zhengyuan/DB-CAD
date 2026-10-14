#pragma once
#include <string>
class outcome;
// ACIS 环境初始化与许可解锁（许可密钥见 spatial_license.h，由 Spatial 官方生成）。
void unlock_license();
int initialize_acis();
void terminate_acis(int level);
std::string process(outcome& result);

