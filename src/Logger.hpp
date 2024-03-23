#pragma once

#include <QObject>
#include <QString>

#include "Definitions.hpp"

class Logger : public QObject
{
    Q_OBJECT

public:

    Logger(const Logger& obj) = delete;
    static Logger* getLogger();
    void log(QString textToLog, LogType logType);
    void saveLogs(QString logs);

signals:
    void logSignal(QString textToLog, LogType logType);

private:
    Logger();
    std::string getCurrentDateAndTime();

    static Logger* logger;
};

