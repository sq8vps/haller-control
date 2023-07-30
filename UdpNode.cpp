#include "UdpNode.hpp"

#include <string>

UdpNode::UdpNode()
    : destinationAddress("1.2.3.4"), destinationPort(9000)
{
    socket = std::make_shared<QUdpSocket>();
    socket->connectToHost(QHostAddress(destinationAddress), destinationPort);
}

UdpNode::UdpNode(QString destinationAddress, uint16_t destinationPort)
    : destinationAddress(destinationAddress), destinationPort(destinationPort)
{
    socket = std::make_shared<QUdpSocket>();
    socket->bind(QHostAddress(destinationAddress), destinationPort);
}

void UdpNode::sendMessage(QString msg)
{
    QByteArray data;
    data.append(msg.toUtf8());
    socket->write(data);
}
