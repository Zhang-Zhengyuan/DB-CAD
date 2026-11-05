#pragma once

#include <cstdio>
#include <filesystem>

namespace dbcad::worker {

// Windows paths use UTF-16; POSIX filesystem paths use native UTF-8 bytes.
inline FILE* openFile(const std::filesystem::path& path, bool write) {
#ifdef _WIN32
    return _wfopen(path.c_str(), write ? L"w+b" : L"rb");
#else
    return std::fopen(path.c_str(), write ? "w+b" : "rb");
#endif
}

}  // namespace dbcad::worker
