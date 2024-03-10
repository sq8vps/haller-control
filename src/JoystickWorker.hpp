#pragma once

#include <QObject>

class JoystickWorker : public QObject
{
    Q_OBJECT
public:
    JoystickWorker();
    ~JoystickWorker();
public slots:
    void process();

signals:
    void finished();
    void error(QString err);
};

