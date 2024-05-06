#pragma once

#include <array>

#include <QString>

#include "Definitions.hpp"

class UdpPacketMaker
{
public:
    UdpPacketMaker();
    ~UdpPacketMaker();
    static QByteArray makePacket(UserInputType inputType, std::array<float, numOfMotors> motorValues = {});
private:
    static void appendMotorValuesToPacket(QByteArray& data, std::array<float, numOfMotors>& motorValues);
};

