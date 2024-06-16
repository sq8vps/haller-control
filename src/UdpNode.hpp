#pragma once

#include <cstdint>
#include <array>

#include <QUdpSocket>

#include "Definitions.hpp"
#include "Logger.hpp"

class UdpNode
{
public:
    UdpNode(QString destinationAddress = "169.254.1.69", uint16_t destinationPort = 25565);
    ~UdpNode();
    void sendMessage(UserInputType inputType, std::array<float, numOfMotors> motorValues);
    void sendMessage(UserInputType inputType, uint8_t servoNumber, float servoValue);
    void sendMessage(UserInputType inputType);
private:
    QString prepareLogMessage(UserInputType inputType, std::array<float, numOfMotors> motorValues);

    std::shared_ptr<QUdpSocket> socket;
    Logger *logger = Logger::getLogger();
    uint16_t destinationPort;
    QString destinationAddress;
};
