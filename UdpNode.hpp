#pragma once

#include <string>
#include <cstdint>
#include <QUdpSocket>


class UdpNode
{
public:
    UdpNode();
    UdpNode(QString destinationAddress, uint16_t destinationPort);
    void sendMessage(QString msg);
private:
    std::shared_ptr<QUdpSocket> socket;
    uint16_t destinationPort;
    QString destinationAddress;
};
