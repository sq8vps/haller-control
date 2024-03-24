#pragma once

#include <QObject>
#include <SFML/Window/Window.hpp>

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
    void gripperClose();
    void gripperOpen();
    void emergencyStop();
    //void motorControl(uint8_t direction, float value);
private:

    constexpr static int joystickNum{0};
    enum class Button : int
    {
        X, A, B, Y, LB, RB, LT, RT, Back, Start, LeftJ, RightJ
    };
};

