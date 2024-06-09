#pragma once

#include <array>

#include <QString>

#include "Definitions.hpp"

class UdpPacketMaker
{
public:
    UdpPacketMaker();
    ~UdpPacketMaker();
    static QByteArray makePacket(UserInputType inputType, std::array<float, numOfMotors> motorValues);
    static QByteArray makePacket(UserInputType inputType, uint8_t servoNumber, float servoValue);
    static QByteArray makePacket(UserInputType inputType);
private:
    static void appendMotorValuesToPacket(QByteArray& data, std::array<float, numOfMotors>& motorValues);
};

