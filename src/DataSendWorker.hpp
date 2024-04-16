#pragma once

#include <QObject>

#include "Definitions.hpp"

class DataSendWorker : public QObject
{
    Q_OBJECT
public:
    DataSendWorker();
public slots:
    void process();
    void updateForceVector(const std::array<float, numOfAxis>& currentVector);

signals:
    void finished();
    void error(QString err);
private:
    std::array<float, numOfAxis> forceVector{};
};

