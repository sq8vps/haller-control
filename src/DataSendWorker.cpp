#include "DataSendWorker.hpp"

#include <chrono>
#include <thread>

#include <QDebug>

DataSendWorker::DataSendWorker() {}

void DataSendWorker::process()
{
    while(true)
    {
        auto x = std::chrono::steady_clock::now() + std::chrono::milliseconds(dataSendIntervalMs);
        QString msg{"Force vector:\n ["};
        for(const auto& val : forceVector)
        {
            msg += std::to_string(val) + "; ";
        }
        msg += "]\n";
        qDebug() << msg;
        std::this_thread::sleep_until(x);
    }
    emit finished();
}

void DataSendWorker::updateForceVector(const std::array<float, numOfAxis>& currentVector)
{
    forceVector = currentVector;
}
