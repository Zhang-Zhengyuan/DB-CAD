#pragma once
// Private dependencies shared by MainWindow implementation units.
#include "mainwindow.h"

#include <QActionGroup>
#include <QApplication>
#include <QDateTime>
#include <QDir>
#include <QFileDialog>
#include <QFile>
#include <QFileInfo>
#include <QGuiApplication>
#include <QThread>

#include <cstdio>
#include <exception>
#include <stdexcept>
#include <QInputDialog>
#include <QLineEdit>
#include <QMenuBar>
#include <QMessageBox>
#include <QMouseEvent>
#include <QProcess>
#include <QStringConverter>
#include <QSettings>
#include <QScopedValueRollback>
#include <QSet>
#include <QSignalBlocker>
#include <QStatusBar>
#include <QTemporaryFile>
#include <QTextStream>
#include <QToolBar>
#include <QDebug>
#include <QDialog>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QPushButton>
#include <QSpinBox>
#include <QComboBox>
#include <QLabel>
#include <QListWidget>
#include <QCheckBox>
#include <QDockWidget>
#include <QTimer>
#include <QUuid>
#include <QUrl>
#include <fstream>
#include <algorithm>
#include <functional>
#include <sstream>

#include "window.h"
#include "kernel/geometry_kernel.h"
#include <filesystem>
#include <format>

std::vector<std::string> read_config_lines(const std::string &filePath, int maxLines);
bool write_config_lines(const std::string &filePath, const std::vector<std::string> &lines, QString *error);
