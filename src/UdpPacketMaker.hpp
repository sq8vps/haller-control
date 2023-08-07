#pragma once

#include "Definitions.hpp"

#include <QString>
#include <array>

class UdpPacketMaker
{
public:
    UdpPacketMaker();
    ~UdpPacketMaker();
    static QByteArray makePacket(UserInputType inputType, std::array<QString, numOfMotors> motorValues = {});
private:
    static void appendMotorValuesToPacket(QByteArray& data, std::array<QString, numOfMotors>& motorValues);
};

