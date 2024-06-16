#include "UdpPacketMaker.hpp"

#include <QByteArray>
#include <QDebug>

UdpPacketMaker::UdpPacketMaker() {}

UdpPacketMaker::~UdpPacketMaker() {}

QByteArray UdpPacketMaker::makePacket(UserInputType inputType)
{
    QByteArray data{};

    switch (inputType) {
    case UserInputType::GripperClose:
        data.append(static_cast<char>(PacketType::GripperControl));
        data.append(static_cast<char>(DataLength::GripperOpenClose));
        data.append(reinterpret_cast<const char*>(&gripperClose), sizeof(gripperClose));
        break;
    case UserInputType::GripperOpen:
        data.append(static_cast<char>(PacketType::GripperControl));
        data.append(static_cast<char>(DataLength::GripperOpenClose));
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

QByteArray UdpPacketMaker::makePacket(UserInputType inputType, std::array<float, numOfMotors> motorValues)
{
    QByteArray data{};

    switch (inputType) {
    case UserInputType::MotorControl:
        data.append(static_cast<char>(PacketType::MotorControl));
        data.append(static_cast<char>(DataLength::MotorControl));
        appendMotorValuesToPacket(data, motorValues);
        break;
    default:
        break;
    }

    return data;
}

QByteArray UdpPacketMaker::makePacket(UserInputType inputType, uint8_t servoNumber, float servoValue)
{
    QByteArray data{};

    switch (inputType) {
    case UserInputType::GripperControl:
        data.append(static_cast<char>(PacketType::ServoControl));
        data.append(static_cast<char>(DataLength::ServoControl));
        data.append(static_cast<char>(servoNumber));
        data.append(reinterpret_cast<const char*>(&servoValue), sizeof(servoValue));
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
