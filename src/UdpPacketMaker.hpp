#pragma once

#include "Definitions.hpp"

#include <QString>
#include <array>

class UdpPacketMaker
{
public:
    UdpPacketMaker();
    ~UdpPacketMaker();
    static QByteArray makePacket(UserInputType inputType, std::array<float, numOfMotors> motorValues = {});
private:
    static void appendMotorValuesToPacket(QByteArray& data, std::array<float, numOfMotors>& motorValues);
};

