#include "UdpPacketMaker.hpp"

UdpPacketMaker::UdpPacketMaker()
{

}

UdpPacketMaker::~UdpPacketMaker()
{

}


int UdpPacketMaker::makePacket(UserInputType inputType, std::array<int, numOfMotors> motorValues)
{
    int data{3};

//    switch (inputType) {
//    case UserInputType::MotorControl:
//        data.append(static_cast<char>(PacketType::MotorControl));
//        data.append(static_cast<char>(DataLength::MotorControl));
//        break;
//    case UserInputType::GripperClose:
//        data.append(PacketType::ServoControl);
//        data.apend(DataLength::ServoControl);
//        break;
//    case UserInputType::GripperOpen:
//        data.append(PacketType::ServoControl);
//        data.apend(DataLength::ServoControl);
//        break;
//    case UserInputType::EmergencyStop:
//        data.append(PacketType::EmergencyStop);
//        data.apend(DataLength::EmergencyStop);
//        break;
//    default:
//        break;
//    }

    return data;
}
