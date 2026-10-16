#include <QtWidgets/QApplication>
#include <QtCore/QCoreApplication>
#include <QtCore/QCommandLineParser>
#include <QtCore/QDir>
#include <QtCore/QFileInfo>
#include <QtCore/QLibraryInfo>
#include <QtGui/QScreen>
#include <QtCore/QTranslator>

#ifdef _WIN32
#include <windows.h>
#include <cstdio>
#include <string>
#include <io.h>
#endif

#include "glwidget.h"
#include "mainwindow.h"

namespace {
void configureQtPluginPath(const char* argv0) {
    QString appDir = QFileInfo(QString::fromLocal8Bit(argv0)).absolutePath();

#ifdef _WIN32
    wchar_t modulePath[MAX_PATH] = {};
    const DWORD moduleLen = GetModuleFileNameW(nullptr, modulePath, MAX_PATH);
    if (moduleLen > 0) {
        appDir = QFileInfo(QString::fromWCharArray(modulePath, static_cast<int>(moduleLen))).absolutePath();
    }
#endif

    const QStringList candidateAppDirs = {
        appDir,
        QDir(appDir).filePath("."),
        QDir(appDir).filePath(".."),
        QDir(appDir).filePath("..\\..")
    };

    for (const QString& dir : candidateAppDirs) {
        const QString localPlatforms = QDir(dir).filePath("platforms");
        if (QDir(localPlatforms).exists()) {
            qputenv("QT_QPA_PLATFORM_PLUGIN_PATH", localPlatforms.toUtf8());
            QCoreApplication::addLibraryPath(dir);
            return;
        }
    }

    const QString qtPlugins = QLibraryInfo::path(QLibraryInfo::PluginsPath);
    const QString qtPlatforms = QDir(qtPlugins).filePath("platforms");
    if (QDir(qtPlatforms).exists()) {
        qputenv("QT_QPA_PLATFORM_PLUGIN_PATH", qtPlatforms.toUtf8());
        QCoreApplication::addLibraryPath(qtPlugins);
    }
}
}

int main(int argc, char** argv) {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    // 调试期：先把 stderr 改成无缓冲，否则下面的 fprintf 重定向提示可能被缓冲在旧 stderr 上
    setvbuf(stderr, nullptr, _IONBF, 0);
    setvbuf(stdout, nullptr, _IONBF, 0);

    // 调试期：把 stdout/stderr 重定向到按 PID 命名的日志文件，
    // 避免 A、B 两个 CAD_DB.exe 互相覆盖同一个 log/log_*.txt。
    // —— 真正部署时请把这段包到 #ifndef 后面，或者改成读取环境变量开关。
    {
        const char* dbcadLogEnv = std::getenv("DBCAD_REDIRECT_LOG");
        if (dbcadLogEnv && std::string(dbcadLogEnv) != "0") {
            DWORD pid = GetCurrentProcessId();
            // 把 CAD_DB.exe 路径拆出目录，再加 "log\\CAD_DB_<pid>.log"
            char modulePath[MAX_PATH] = {0};
            DWORD moduleLen = GetModuleFileNameA(nullptr, modulePath, MAX_PATH);
            if (moduleLen > 0) {
                std::string dir(modulePath, moduleLen);
                size_t pos = dir.find_last_of("\\/");
                std::string dirOnly = (pos == std::string::npos) ? "." : dir.substr(0, pos);
                char outPath[MAX_PATH] = {0};
                _snprintf_s(outPath, sizeof(outPath), _TRUNCATE,
                            "%s\\log\\CAD_DB_%lu.log", dirOnly.c_str(), (unsigned long)pid);
                FILE* out = std::fopen(outPath, "w");
                if (out) {
                    std::fprintf(stderr,
                                 "[DEBUG] redirecting stdout/stderr to %s (pid=%lu)\n",
                                 outPath, (unsigned long)pid);
                    int fd = _fileno(out);
                    _dup2(fd, _fileno(stdout));
                    _dup2(fd, _fileno(stderr));
                    // dup2 之后，新 fd 也需要 unbuffered，否则 Qt 的 qDebug 默认走 stderr
                    // 会等到 4KB 才 flush。
                    setvbuf(stdout, nullptr, _IONBF, 0);
                    setvbuf(stderr, nullptr, _IONBF, 0);
                } else {
                    std::fprintf(stderr,
                                 "[DEBUG] failed to open %s, log will go to console\n", outPath);
                }
            }
        }
    }
#else
    // 调试：把 stderr 改成无缓冲，保证崩溃前 fprintf 的内容能立即落盘
    setvbuf(stderr, nullptr, _IONBF, 0);
    setvbuf(stdout, nullptr, _IONBF, 0);
#endif

    QCoreApplication::setApplicationName("DBCAD");
    configureQtPluginPath(argv[0]);

    qputenv("QT_ENABLE_HIGHDPI_SCALING", "0");

    QApplication app(argc, argv);
    QCoreApplication::setOrganizationName("DBCAD");
    QCoreApplication::setApplicationVersion("0.0.1");
    QCommandLineParser parser;
    parser.setApplicationDescription(QCoreApplication::applicationName());
    parser.addHelpOption();
    parser.addVersionOption();
    QCommandLineOption multipleSampleOption("multisample", "Multisampling");
    parser.addOption(multipleSampleOption);
    QCommandLineOption coreProfileOption("coreprofile", "Use core profile");
    parser.addOption(coreProfileOption);
    QCommandLineOption transparentOption("transparent", "Transparent window");
    parser.addOption(transparentOption);

    parser.process(app);

    QSurfaceFormat fmt;
    fmt.setDepthBufferSize(24);
    if (parser.isSet(multipleSampleOption)) fmt.setSamples(4);
    if (parser.isSet(coreProfileOption)) {
        fmt.setVersion(3, 2);
        fmt.setProfile(QSurfaceFormat::CoreProfile);
    }
    QSurfaceFormat::setDefaultFormat(fmt);

    MainWindow mainWindow;

    GLWidget::setTransparent(parser.isSet(transparentOption));
    if (GLWidget::getTransparent()) {
        mainWindow.setAttribute(Qt::WA_TranslucentBackground);
        mainWindow.setAttribute(Qt::WA_NoSystemBackground, false);
    }
    mainWindow.resize(QSize(1200, 800));
    int desktopArea = QGuiApplication::primaryScreen()->size().width() * QGuiApplication::primaryScreen()->size().height();
    int widgetArea = mainWindow.width() * mainWindow.height();
    if (((float)widgetArea / (float)desktopArea) < 0.75f)
        mainWindow.show();
    else
        mainWindow.showMaximized();

    return app.exec();
}
