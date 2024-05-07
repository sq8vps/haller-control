#pragma once

#include <QObject>
#include <QTimer>

#include "Definitions.hpp"
#include "Allocator.hpp"

class DataSendTimer : public QObject
{
    Q_OBJECT
public:
    DataSendTimer();
    ~DataSendTimer();
public slots:
    void updateForceVector(const std::array<float, numOfAxis>& currentVector);
    void processAndSendVetctor();
signals:
    void sendForceVector(const std::array<float, numOfMotors>& calculatedForceVector);
private:
    std::array<float, numOfAxis> currentForceVector;
    QTimer *timer;
    Allocator allocator;
};

