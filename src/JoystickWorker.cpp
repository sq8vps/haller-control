#include "JoystickWorker.hpp"

#include <QDebug>

#include <SFML/Window/Joystick.hpp>
#include <SFML/Window/Window.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <cmath>

JoystickWorker::JoystickWorker(){}

JoystickWorker::~JoystickWorker(){}

void JoystickWorker::process(){

    sf::Joystick::update();
    sf::Window window(sf::VideoMode({800, 600}), "Joystick window");
    window.setVisible(false);
    bool isWindowContextActivated = window.setActive(false);

    while (window.isOpen() and isWindowContextActivated)
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed or not sf::Joystick::isConnected(joystickNum))
                window.close();

            if (sf::Joystick::isButtonPressed(joystickNum, int(Button::A)))
            {
                emit gripperOpen();
            }
            if (sf::Joystick::isButtonPressed(joystickNum, int(Button::X)))
            {
                emit gripperClose();
            }

            if(sf::Joystick::isButtonPressed(joystickNum, int(Button::RB)) or
                sf::Joystick::isButtonPressed(joystickNum, int(Button::LB)))
            {
                emit emergencyStop();
            }
            ForceVector currentForceVector = getCurrentForceVector();
            // TODO test if this comparison works good
            if(currentForceVector != previousForceVector)
            {
                emit motorControl(currentForceVector);
                previousForceVector = currentForceVector;
            }
        }
        window.display();
    }
    emit finished();
}

ForceVector JoystickWorker::getCurrentForceVector()
{
    ForceVector currentAxisPositions;
    // back and forth
    currentAxisPositions.at(0) = -sf::Joystick::getAxisPosition(joystickNum, sf::Joystick::Axis::V) / 100.f;
    // left and right
    currentAxisPositions.at(1) = sf::Joystick::getAxisPosition(joystickNum, sf::Joystick::Axis::U) / 100.f;
    // up and down
    currentAxisPositions.at(2) = sf::Joystick::getAxisPosition(joystickNum, sf::Joystick::Axis::Y) / 100.f;
    // roll
    currentAxisPositions.at(3) = -sf::Joystick::getAxisPosition(joystickNum, sf::Joystick::Axis::Z) / 100.f;
    // yaw
    currentAxisPositions.at(4) = sf::Joystick::getAxisPosition(joystickNum, sf::Joystick::Axis::X) / 100.f;

    for(int i = 0; i < currentAxisPositions.size(); i++)
    {
        float x{currentAxisPositions.at(i)};
        float absx{};
        switch(equalization)
        {
            case SQUARE:
                currentAxisPositions.at(i) = ((x < 0.f) ? -1.f : 1.f) * x * x * gain;
                break;
            case INVERSE_SQUARE:
                absx = abs(x);
                currentAxisPositions.at(i) = ((x < 0.f) ? -1.f : 1.f) * (1.f - pow(absx - 1.f, 2.f)) * gain;
                break;
            case CUBE:
                currentAxisPositions.at(i) = x * x * x * gain;
                break;
            case INVERSE_CUBE:
                absx = abs(x);
                currentAxisPositions.at(i) = ((x < 0.f) ? -1.f : 1.f) * (1.f + pow(absx - 1.f, 3.f)) * gain;
                break;
            case LINEAR:
            default:
                currentAxisPositions.at(i) = x * gain;
                break;
        }
    }

    if (sf::Joystick::isButtonPressed(joystickNum, int(Button::Y)))
    {
        zOffset = currentAxisPositions.at(2);
        emit setZtrim(zOffset * 0.5f);
    }
    currentAxisPositions.at(2) += zOffset;

    return currentAxisPositions;
}

void JoystickWorker::setJoystickGain(float gain)
{
    this->gain = gain;
}

void JoystickWorker::setEqualization(JoystickWorker::Equalization e)
{
    equalization = e;
}
