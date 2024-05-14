#include "JoystickWorker.hpp"

#include <QDebug>

#include <SFML/Window/Joystick.hpp>
#include <SFML/Window/Window.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/VideoMode.hpp>

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
    currentAxisPositions.at(0) = sf::Joystick::getAxisPosition(joystickNum, sf::Joystick::Axis::X) / 100.f * gain;
    // left and right
    currentAxisPositions.at(1) = sf::Joystick::getAxisPosition(joystickNum, sf::Joystick::Axis::Y) / 100.f * gain;
    // up and down
    currentAxisPositions.at(2) = sf::Joystick::getAxisPosition(joystickNum, sf::Joystick::Axis::V) / 100.f * gain;
    // roll
    currentAxisPositions.at(3) = sf::Joystick::getAxisPosition(joystickNum, sf::Joystick::Axis::PovX) / 100.f * gain;
    // yaw
    currentAxisPositions.at(4) = sf::Joystick::getAxisPosition(joystickNum, sf::Joystick::Axis::PovY) / 100.f * gain;

    return currentAxisPositions;
}

void JoystickWorker::setJoystickGain(float gain)
{
    this->gain = gain;
}
