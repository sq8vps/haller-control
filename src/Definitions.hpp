#pragma once

#include <cstdint>

static constexpr uint8_t numOfMotors{6};
constexpr float pi = 3.1415927;
constexpr float gripperOpen{2*pi};
constexpr float gripperClose{pi};

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

enum class ServoNumber : uint8_t
{
    GripperOpen,
    GripperClose,
};

enum class UserInputType : uint8_t
{
    MotorControl,
    GripperOpen,
    GripperClose,
    EmergencyStop
};

enum class LogType : uint8_t
{
    Debug,
    Inf,
    Warning,
    Error
};
