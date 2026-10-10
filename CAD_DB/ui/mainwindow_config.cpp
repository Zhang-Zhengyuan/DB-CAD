#include "mainwindow_internal.h"

std::string trim_copy(const std::string &input) {
    const size_t first = input.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) {
        return "";
    }
    const size_t last = input.find_last_not_of(" \t\r\n");
    return input.substr(first, last - first + 1);
}

// Read up to maxLines from a config file. Tries, in order:
//   1. filePath as-is (absolute, or relative to current working dir)
//   2. <exe dir>/filePath
//   3. <exe dir>/../filePath
//   4. <exe dir>/../../filePath
std::vector<std::string> read_config_lines(const std::string &filePath, int maxLines) {
    std::vector<std::string> values;
    auto tryOpen = [&](const std::string &p) -> bool {
        std::ifstream fs(p, std::ios_base::in);
        if (!fs.is_open())
            return false;
        std::string line;
        // Strip UTF-8 BOM (\xEF\xBB\xBF) that PowerShell's Set-Content -Encoding UTF8 writes.
        bool firstLine = true;
        while (static_cast<int>(values.size()) < maxLines && std::getline(fs, line)) {
            if (firstLine && line.size() >= 3 && static_cast<unsigned char>(line[0]) == 0xEF &&
                static_cast<unsigned char>(line[1]) == 0xBB && static_cast<unsigned char>(line[2]) == 0xBF) {
                line.erase(0, 3);
            }
            firstLine = false;
            values.push_back(trim_copy(line));
        }
        return true;
    };

    QStringList candidates;
    candidates << QString::fromStdString(filePath);
    const QString appDir = QApplication::applicationDirPath();
    candidates << QDir(appDir).filePath(QString::fromStdString(filePath));
    candidates << QDir(appDir).filePath(QString("..") + QDir::separator() + QString::fromStdString(filePath));
    candidates << QDir(appDir).filePath(QString("../..") + QDir::separator() + QString::fromStdString(filePath));

    for (const QString &c : candidates) {
        values.clear();
        if (tryOpen(c.toStdString()))
            break;
    }

    // pad to maxLines if the file existed but had fewer entries
    while (static_cast<int>(values.size()) < maxLines) {
        values.emplace_back("");
    }
    return values;
}

bool write_config_lines(const std::string &filePath, const std::vector<std::string> &lines, QString *error) {
    QStringList candidates;
    candidates << QString::fromStdString(filePath);
    const QString appDir = QApplication::applicationDirPath();
    candidates << QDir(appDir).filePath(QString::fromStdString(filePath));
    candidates << QDir(appDir).filePath(QString("..") + QDir::separator() + QString::fromStdString(filePath));
    candidates << QDir(appDir).filePath(QString("../..") + QDir::separator() + QString::fromStdString(filePath));

    for (const QString &c : candidates) {
        std::ofstream fs(c.toStdString(), std::ios_base::out | std::ios_base::trunc);
        if (!fs.is_open())
            continue;
        for (const auto &line : lines) {
            fs << line << '\n';
        }
        return true;
    }
    if (error != nullptr) {
        *error = QObject::tr("无法写入配置文件：%1").arg(QString::fromStdString(filePath));
    }
    return false;
}
