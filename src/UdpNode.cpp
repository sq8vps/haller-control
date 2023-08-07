#include "UdpNode.hpp"
#include "UdpPacketMaker.hpp"

UdpNode::UdpNode(QString destinationAddress, uint16_t destinationPort)
    : destinationAddress(destinationAddress), destinationPort(destinationPort)
{
    socket = std::make_shared<QUdpSocket>();
    socket->bind(QHostAddress(destinationAddress), destinationPort);
    // TODO check if need to connect
}

UdpNode::~UdpNode()
{
    socket->close();
}

void UdpNode::sendMessage(UserInputType inputType, std::array<QString, numOfMotors> motorValues)
{
    QByteArray data{};
    socket->write(data);
}
