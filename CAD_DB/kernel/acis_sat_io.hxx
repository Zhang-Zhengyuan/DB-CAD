#pragma once
// Private ACIS I/O: every caller establishes its own file metadata.
#include "acis/include/kernapi.hxx"
#include "acis/include/fileinfo.hxx"
#include "acis/include/vers.hxx"
#include <QDir>
#include <QFile>
#include <QTemporaryFile>
#include <memory>
#include <stdexcept>

namespace dbcad::acis_detail {
inline void checkSat(outcome status) {
    if(!status.ok()) throw std::runtime_error(find_err_mess(status.error_number()));
}
inline FILE* openSat(const QString& path, bool writing) {
#ifdef _WIN32
    return _wfopen(reinterpret_cast<const wchar_t*>(path.utf16()),writing?L"w+b":L"rb");
#else
    return fopen(QFile::encodeName(path).constData(),writing?"w+b":"rb");
#endif
}
inline QByteArray writeSat(const ENTITY_LIST& entities) {
    QTemporaryFile temporary(QDir::tempPath()+"/dbcad_sat_XXXXXX.sat");
    if(!temporary.open()) throw std::runtime_error(temporary.errorString().toStdString());
    const auto path=temporary.fileName(); temporary.close();
    std::unique_ptr<FILE,decltype(&fclose)> file(openSat(path,true),&fclose);
    if(!file) throw std::runtime_error("Cannot open SAT temporary file");
    // FileUnits is mandatory even on the first Push after starting the modeller.
    // Latest native version preserves procedural data; never inherit another caller's version.
    checkSat(api_save_version(get_major_version(),get_minor_version()));
    FileInfo info; info.set_units(1.0); info.set_product_id("DBCAD");
    checkSat(api_set_file_info(FileIdent|FileUnits,info));
    checkSat(api_set_int_option("sequence_save_files",0));
    checkSat(api_save_entity_list(file.get(),TRUE,entities));
    if(fflush(file.get())!=0) throw std::runtime_error("Cannot flush SAT temporary file");
    file.reset();
    QFile input(path);
    if(!input.open(QIODevice::ReadOnly)) throw std::runtime_error(input.errorString().toStdString());
    auto bytes=input.readAll();
    if(input.error()!=QFileDevice::NoError || bytes.isEmpty()) throw std::runtime_error("Empty or unreadable SAT backup");
    return bytes;
}
inline void readSat(const QByteArray& bytes, ENTITY_LIST& entities) {
    QTemporaryFile temporary(QDir::tempPath()+"/dbcad_sat_XXXXXX.sat");
    if(!temporary.open() || temporary.write(bytes)!=bytes.size()) throw std::runtime_error("Cannot stage SAT input");
    const auto path=temporary.fileName(); temporary.close();
    std::unique_ptr<FILE,decltype(&fclose)> file(openSat(path,false),&fclose);
    if(!file) throw std::runtime_error("Cannot open SAT input");
    checkSat(api_restore_entity_list(file.get(),TRUE,entities));
}
}
