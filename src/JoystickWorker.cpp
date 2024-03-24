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
            if (event.type == sf::Event::Closed)
                window.close();

            // sfml window is only needed for handling joystick input
            if (not sf::Joystick::isConnected(0))
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
        }
        window.display();
    }
    emit finished();
}
