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

void Logger::log()
{
    qDebug() << "dupa";
    emit logSignal();
}
