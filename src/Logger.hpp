#pragma once

#include <QObject>

class Logger : public QObject
{
    Q_OBJECT

public:

    Logger(const Logger& obj) = delete;
    static Logger* getLogger();
    void log();

signals:
    void logSignal();

private:
    Logger();
    static Logger* logger;
};

