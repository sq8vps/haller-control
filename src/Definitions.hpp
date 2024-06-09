#pragma once

#include <cstdint>

static constexpr uint8_t numOfMotors{6};
static constexpr uint8_t numOfAxis{5};
constexpr float pi = 3.1415927;
constexpr uint8_t gripperOpen{0};
constexpr uint8_t gripperClose{1};
constexpr uint32_t dataSendIntervalMs = 50; // change to 50 when done

enum class PacketType : uint8_t
{
    MotorControl = 1,
    EmergencyStop = 2,
    ServoControl = 3,
    GripperControl = 4,
};

enum class DataLength : uint8_t
{
    MotorControl = 4 * numOfMotors,
    EmergencyStop = 0,
    ServoControl = 5,
    GripperControl = 1,
    GripperOpenClose = 1,
};

enum class ServoNumber : uint8_t
{
    Gripper = 6,
};

enum class UserInputType : uint8_t
{
    MotorControl,
    GripperOpen,
    GripperClose,
    EmergencyStop,
    GripperControl,
};

enum class LogType : uint8_t
{
    Debug,
    Inf,
    Warning,
    Error
};
