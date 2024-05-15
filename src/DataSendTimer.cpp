#include "DataSendTimer.hpp"

#include "Definitions.hpp"

#include <QDebug>

DataSendTimer::DataSendTimer()
{
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(processAndSendVetctor()));
    timer->start(dataSendIntervalMs);
}

DataSendTimer::~DataSendTimer()
{
    delete timer;
}

void DataSendTimer::processAndSendVetctor()
{
   // qDebug() << "timer is timing";
    std::array<float, numOfMotors> calculatedForceVector{allocator.allocate(currentForceVector)};
    emit sendForceVector(calculatedForceVector);
}

void DataSendTimer::updateForceVector(const std::array<float, numOfAxis>& currentVector)
{
    currentForceVector = currentVector;
}
