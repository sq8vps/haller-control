#pragma once

#include <QObject>
#include <SFML/Window/Joystick.hpp>

class CustomJoystick : public QObject
{
    Q_OBJECT
public:
    CustomJoystick();
signals:
    void buttonPressed();
};

