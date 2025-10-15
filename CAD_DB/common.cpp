#include <iostream>
#include <string>
#include <chrono>
#include <QtWidgets/QApplication>
#include "common.hxx"

pqxx::connection* postgresqldb_conn;

void myerror(std::string_view errmsg) {
    qCritical() << "*** Error: " << QString::fromUtf8(errmsg) << Qt::endl;
    exit(3);
}
std::chrono::steady_clock::time_point Timer::now() {
    return std::chrono::steady_clock::now();
}
double Timer::duration(std::chrono::steady_clock::time_point time_start, std::chrono::steady_clock::time_point time_end) {
    return std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(time_end - time_start).count();
}