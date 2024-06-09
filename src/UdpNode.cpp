#include "UdpNode.hpp"

#include <QString>

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
    socket->write(data);
    logger->log(prepareLogMessage(inputType, motorValues), LogType::Inf);
}

void UdpNode::sendMessage(UserInputType inputType, uint8_t servoNumber, float servoValue)
{
    QByteArray data{UdpPacketMaker::makePacket(inputType, servoNumber, servoValue)};
    socket->write(data);
}

void UdpNode::sendMessage(UserInputType inputType)
{
    QByteArray data{UdpPacketMaker::makePacket(inputType)};
    socket->write(data);
}

QString UdpNode::prepareLogMessage(UserInputType inputType, std::array<float, numOfMotors> motorValues)
{
    // TODO make it less ugly
    QString logMsg = "UDP packet was sent. Packet type: ";
    switch(inputType)
    {
    // case UserInputType::GripperOpen:
    //     logMsg += "servo control (id: " + QString::number(uint8_t(PacketType::ServoControl)) + "), "
    //             + "packet data length: " + QString::number(uint8_t(DataLength::ServoControl))
    //             + ", packet data: [" + QString::number(uint8_t(ServoNumber::GripperOpen)) + ", "
    //             + QString::number(gripperOpen) + "]";
    //     break;
    // case UserInputType::GripperClose:
    //     logMsg += "servo control (id: " + QString::number(uint8_t(PacketType::ServoControl)) + "), "
    //             + "packet length: " + QString::number(uint8_t(DataLength::ServoControl))
    //             + ", packet data: [" + QString::number(uint8_t(ServoNumber::GripperClose)) + ", "
    //             + QString::number(gripperClose) + "]";
    //     break;
    case UserInputType::EmergencyStop:
        logMsg += "emergency stop (id: " + QString::number((uint8_t)PacketType::EmergencyStop) + "), "
                + "packet length: " + QString::number(uint8_t(DataLength::EmergencyStop));
        break;
    case UserInputType::MotorControl:
        logMsg += "motor control (id: " + QString::number((uint8_t)PacketType::MotorControl) + "), "
                + "packet length: " + QString::number(uint8_t(DataLength::MotorControl))
                + ", packet data: [";
        for(int i=0; i<numOfMotors; ++i)
        {
            logMsg += QString::number(motorValues[i]);
            if(i!=numOfMotors-1)
            {
                logMsg += ", ";
            }
        }
        logMsg += "]";
        break;
    default:
        break;
    }
    return logMsg;
}
