#pragma once

#include <array>

#include <QObject>
#include <SFML/Window/Window.hpp>

#include "Definitions.hpp"

using ForceVector = std::array<float, numOfAxis>;

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
    // TODO change names
    void motorControl(const ForceVector& forceVector);
private:
    ForceVector getCurrentForceVector();

    ForceVector previousForceVector{};
    constexpr static int joystickNum{0};
    enum class Button : int
    {
        X, A, B, Y, LB, RB, LT, RT, Back, Start, LeftJ, RightJ
    };
};

