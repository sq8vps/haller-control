#include "Logger.hpp"

#include <chrono>
#include <sstream>

#include <QDebug>
#include <QFile>

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

void Logger::saveLogs(QString logs)
{
    QString filename = QString::fromUtf8(std::string("logs-" + getCurrentDateAndTime() + ".txt"));
    // files in windows should not contain ":"
    filename.replace(":", "-");
    // file is created in build directory
    QFile file(filename);

    if(file.open(QIODevice::WriteOnly))
    {
       QTextStream stream(&file);
       stream << logs;
    }
}

std::string Logger::getCurrentDateAndTime()
{
    auto now = std::chrono::system_clock::now();
    auto nowTime = std::chrono::system_clock::to_time_t(now);

    std::stringstream stream;
    stream << std::put_time(std::localtime(&nowTime), "%Y-%m-%d-%X");
    return stream.str();
}

