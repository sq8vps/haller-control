#pragma once

#include <mutex>

#include <QObject>
#include <QString>

#include "Definitions.hpp"

// Logger is a singleton (insted of class with only static functions)
// becaouse QT needs an object to connect signals with slots
class Logger : public QObject
{
    Q_OBJECT

public:

    Logger(const Logger& obj) = delete;
    void operator=(const Logger &) = delete;

    static Logger* getLogger();
    void log(QString textToLog, LogType logType);
    void saveLogs(QString logs);

signals:
    void logSignal(QString textToLog, LogType logType);

private:
    Logger();
    ~Logger();
    std::string getCurrentDateAndTime();

    static Logger* logger;
    static std::mutex mutex;
};

