#pragma once

#include <cstdint>

static constexpr uint8_t numOfMotors{5};

enum class PacketType : uint8_t
{
    MotorControl = 1,
    EmergencyStop = 2,
    ServoControl = 3
};

enum class DataLength : uint8_t
{
    MotorControl = 4 * numOfMotors,
    EmergencyStop = 0,
    ServoControl = 5
};

enum class UserInputType : uint8_t
{
    MotorControl,
    GripperOpen,
    GripperClose,
    EmergencyStop
};
