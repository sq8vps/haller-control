#pragma once

#include <QObject>

#include "Definitions.hpp"

using ForceVector = std::array<float, numOfAxis>;

class DataSendWorker : public QObject
{
    Q_OBJECT
public:
    DataSendWorker();
public slots:
    void process();
    void updateForceVector(const ForceVector& currentVector);

signals:
    void finished();
    void error(QString err);
private:
    ForceVector forceVector{};
};

