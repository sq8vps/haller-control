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
    enum Equalization
    {
        LINEAR,
        SQUARE,
        CUBE,
        INVERSE_SQUARE,
        INVERSE_CUBE,
    };
    JoystickWorker();
    ~JoystickWorker();
    void setJoystickGain(float gain);
    void setEqualization(Equalization e);
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

    ForceVector previousForceVector{0.f, 0.f, 0.f, 0.f, 0.f};
    constexpr static int joystickNum{0};
    enum class Button : int
    {
        X, A, B, Y, LB, RB, LT, RT, Back, Start, LeftJ, RightJ
    };
    float gain{1.f};
    float zOffset{0.f};
    Equalization equalization{LINEAR};
};

