#pragma once

#include "Definitions.hpp"

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
    std::shared_ptr<QUdpSocket> socket;
    uint16_t destinationPort;
    QString destinationAddress;
};
