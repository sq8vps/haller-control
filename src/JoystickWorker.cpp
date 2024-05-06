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
    currentAxisPositions.at(0) = sf::Joystick::getAxisPosition(joystickNum, sf::Joystick::Axis::X);
    // left and right
    currentAxisPositions.at(1) = sf::Joystick::getAxisPosition(joystickNum, sf::Joystick::Axis::Y);
    // up and down
    currentAxisPositions.at(2) = sf::Joystick::getAxisPosition(joystickNum, sf::Joystick::Axis::V);
    // roll
    currentAxisPositions.at(3) = sf::Joystick::getAxisPosition(joystickNum, sf::Joystick::Axis::PovX);
    // yawn
    currentAxisPositions.at(4) = sf::Joystick::getAxisPosition(joystickNum, sf::Joystick::Axis::PovY);

    return currentAxisPositions;
}
