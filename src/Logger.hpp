#pragma once

#include "Definitions.hpp"

#include <QObject>
#include <QString>

class Logger : public QObject
{
    Q_OBJECT

public:

    Logger(const Logger& obj) = delete;
    static Logger* getLogger();
    void log(QString textToLog, LogType logType);

signals:
    void logSignal(QString textToLog, LogType logType);

private:
    Logger();
    static Logger* logger;
};

