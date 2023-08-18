#include "UdpNode.hpp"
#include "UdpPacketMaker.hpp"

UdpNode::UdpNode(QString destinationAddress, uint16_t destinationPort)
    : destinationAddress(destinationAddress), destinationPort(destinationPort)
{
    socket = std::make_shared<QUdpSocket>();
    socket->connectToHost(QHostAddress(destinationAddress), destinationPort);
}

UdpNode::~UdpNode()
{
    socket->close();
}

void UdpNode::sendMessage(UserInputType inputType, std::array<float, numOfMotors> motorValues)
{
    QByteArray data{UdpPacketMaker::makePacket(inputType, motorValues)};
    qDebug() << data;
    socket->write(data);
}
