#include <QDebug>

#include <SFML/Window/Joystick.hpp>
#include <SFML/Window/Window.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/VideoMode.hpp>

#include "JoystickWorker.hpp"

JoystickWorker::JoystickWorker(){}

JoystickWorker::~JoystickWorker(){}

void JoystickWorker::process(){
    qDebug("siemka siemka");

    sf::Window window(sf::VideoMode(), "SFML window");
    sf::Joystick::update();
    while (window.isOpen())
    {
        // Event processing
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Request for closing the window
            if (event.type == sf::Event::Closed)
                window.close();
            if (sf::Joystick::isConnected(0))
            {
                qDebug() << "dupsko";
            }
        }
        window.setActive();
        window.display();
    }

    emit finished();
}
