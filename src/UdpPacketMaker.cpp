#include "UdpPacketMaker.hpp"

#include <QByteArray>
#include <Qdebug>

UdpPacketMaker::UdpPacketMaker() {}

UdpPacketMaker::~UdpPacketMaker() {}

QByteArray UdpPacketMaker::makePacket(UserInputType inputType, std::array<float, numOfMotors> motorValues)
{
    QByteArray data{};

    switch (inputType) {
    case UserInputType::MotorControl:
        data.append(static_cast<char>(PacketType::MotorControl));
        data.append(static_cast<char>(DataLength::MotorControl));
        appendMotorValuesToPacket(data, motorValues);
        break;
    case UserInputType::GripperClose:
        data.append(static_cast<char>(PacketType::ServoControl));
        data.append(static_cast<char>(DataLength::ServoControl));
        data.append(static_cast<char>(ServoNumber::GripperClose));
        data.append(reinterpret_cast<const char*>(&gripperClose), sizeof(gripperClose));
        break;
    case UserInputType::GripperOpen:
        data.append(static_cast<char>(PacketType::ServoControl));
        data.append(static_cast<char>(DataLength::ServoControl));
        data.append(static_cast<char>(ServoNumber::GripperOpen));
        data.append(reinterpret_cast<const char*>(&gripperOpen), sizeof(gripperOpen));
        break;
    case UserInputType::EmergencyStop:
        data.append(static_cast<char>(PacketType::EmergencyStop));
        data.append(static_cast<char>(DataLength::EmergencyStop));
        break;
    default:
        break;
    }

    return data;
}

void UdpPacketMaker::appendMotorValuesToPacket(QByteArray& data, std::array<float, numOfMotors>& motorValues)
{
    for(const auto& value : motorValues)
    {
        data.append(reinterpret_cast<const char*>(&value), sizeof(value));
    }
}
