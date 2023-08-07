#pragma once

#include "Definitions.hpp"
#include <array>


class UdpPacketMaker
{
public:
    UdpPacketMaker();
    ~UdpPacketMaker();
    static int makePacket(UserInputType inputType, std::array<int, numOfMotors> motorValues);
};

