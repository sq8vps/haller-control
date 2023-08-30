#include "UdpNode.hpp"
#include "UdpPacketMaker.hpp"

#include <QString>

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
    socket->write(data);
    prepareLogMessage(inputType, motorValues);
}

void UdpNode::prepareLogMessage(UserInputType inputType, std::array<float, numOfMotors> motorValues)
{
    // TODO make it less ugly
    QString logMsg = "UDP packet was sent. Packet type: ";
    switch(inputType)
    {
    case UserInputType::GripperOpen:
        logMsg += "servo control (id: " + QString::number(uint8_t(PacketType::ServoControl)) + "), "
                + "packet data length: " + QString::number(uint8_t(DataLength::ServoControl))
                + ", packet data: [" + QString::number(uint8_t(ServoNumber::GripperOpen)) + ", "
                + QString::number(gripperOpen) + "]";
        break;
    case UserInputType::GripperClose:
        logMsg += "servo control (id: " + QString::number(uint8_t(PacketType::ServoControl)) + "), "
                + "packet length: " + QString::number(uint8_t(DataLength::ServoControl))
                + ", packet data: [" + QString::number(uint8_t(ServoNumber::GripperClose)) + ", "
                + QString::number(gripperClose) + "]";
        break;
    case UserInputType::EmergencyStop:
        logMsg += "emergency stop (id: " + QString::number((uint8_t)PacketType::EmergencyStop) + "), "
                + "packet length: " + QString::number(uint8_t(DataLength::EmergencyStop));
        break;
    case UserInputType::MotorControl:
        logMsg += "motor control (id: " + QString::number((uint8_t)PacketType::MotorControl) + "), "
                + "packet length: " + QString::number(uint8_t(DataLength::MotorControl))
                + ", packet data: [";
        for(const auto &value : motorValues)
        {
            logMsg += QString::number(value) + ", ";
        }
        logMsg += "]";
        break;
    default:
        break;
    }
    logger->log(logMsg, LogType::Inf);
}
