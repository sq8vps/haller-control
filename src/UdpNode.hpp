#pragma once

#include "Definitions.hpp"
#include "Logger.hpp"

#include <cstdint>
#include <array>
#include <QUdpSocket>

class UdpNode
{
public:
    UdpNode(QString destinationAddress = "2.3.4.5", uint16_t destinationPort = 9000);
    ~UdpNode();
    void sendMessage(UserInputType inputType, std::array<float, numOfMotors> motorValues = {});
private:
    QString prepareLogMessage(UserInputType inputType, std::array<float, numOfMotors> motorValues);

    std::shared_ptr<QUdpSocket> socket;
    Logger *logger = Logger::getLogger();
    uint16_t destinationPort;
    QString destinationAddress;
};
