#include <QDebug>

#include <SFML/Window/Joystick.hpp>
#include <SFML/Window/Window.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/VideoMode.hpp>

#include "JoystickWorker.hpp"

JoystickWorker::JoystickWorker(){}

JoystickWorker::~JoystickWorker(){}

void JoystickWorker::process(){

    sf::Joystick::update();
    sf::Window window(sf::VideoMode({800, 600}), "My window");
    window.setVisible(false);
    window.setActive(false);

    while (window.isOpen())
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

        }
        window.display();
    }

    emit finished();
}
