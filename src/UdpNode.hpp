#pragma once

#include "Definitions.hpp"
#include <cstdint>
#include <array>
#include <QUdpSocket>

class UdpNode
{
public:
    UdpNode(QString destinationAddress = "1.2.3.4", uint16_t destinationPort=9000);
    ~UdpNode();
    void sendMessage(UserInputType inputType, std::array<QString, numOfMotors> motorValues = {});
private:
    std::shared_ptr<QUdpSocket> socket;
    uint16_t destinationPort;
    QString destinationAddress;
};
