#include "Logger.hpp"

#include <QDebug>

Logger *Logger::logger = nullptr;

Logger::Logger()
{
}

Logger* Logger::getLogger()
{
    if(logger == nullptr)
    {
       logger = new Logger();
    }
    return logger;
}

void Logger::log(QString textToLog, LogType logType)
{
    switch(logType)
    {
    case LogType::Debug:
       textToLog = "DEBUG: " + textToLog + "\n";
       break;
    case LogType::Inf:
       textToLog = "INF: " + textToLog + "\n";
       break;
    case LogType::Warning:
       textToLog = "WARNING: " + textToLog + "\n";
       break;
    case LogType::Error:
       textToLog = "ERROR: " + textToLog + "\n";
       break;
    default:
       break;
    }
    emit logSignal(textToLog, logType);
}
