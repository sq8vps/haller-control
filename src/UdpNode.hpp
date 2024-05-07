#pragma once

#include <cstdint>
#include <array>

#include <QUdpSocket>

#include "Definitions.hpp"
#include "Logger.hpp"

class UdpNode
{
public:
    UdpNode(QString destinationAddress = "192.168.69.100", uint16_t destinationPort = 25565);
    ~UdpNode();
    void sendMessage(UserInputType inputType, std::array<float, numOfMotors> motorValues = {});
private:
    QString prepareLogMessage(UserInputType inputType, std::array<float, numOfMotors> motorValues);

    std::shared_ptr<QUdpSocket> socket;
    Logger *logger = Logger::getLogger();
    uint16_t destinationPort;
    QString destinationAddress;
};
