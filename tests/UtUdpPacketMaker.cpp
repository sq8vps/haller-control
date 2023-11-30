#include <gtest/gtest.h>
#include "../src/UdpPacketMaker.hpp"

#include <QDebug>

#include <array>


class UdpPacketMakerTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        sut = std::make_shared<UdpPacketMaker>();
    }

    void assertActualPacketSizeMatchesExpected(const QByteArray& packet, DataLength dataLength);
    void assertActualPacketTypeMatchesExpected(const QByteArray& packet, PacketType packetType);

    static constexpr uint8_t basePacketSize{2};
    std::array<float, numOfMotors> motorValues;
    std::shared_ptr<UdpPacketMaker> sut;
};

void UdpPacketMakerTest::assertActualPacketTypeMatchesExpected(const QByteArray& packet, PacketType packetType)
{
    // first byte of packet contains info of packet type
    ASSERT_EQ(static_cast<uint8_t>(packetType), packet.at(0));
}

void UdpPacketMakerTest::assertActualPacketSizeMatchesExpected(const QByteArray& packet, DataLength dataLength)
{
    uint8_t dataLengthUint = static_cast<uint8_t>(dataLength);
    uint8_t packetSize = basePacketSize + dataLengthUint;

    // second byte of packet contains info of data length
    ASSERT_EQ(dataLengthUint, packet.at(1));
    ASSERT_EQ(packetSize, packet.size());
}

TEST_F(UdpPacketMakerTest, makeMotorControlPacket)
{
    const auto & packet = sut->makePacket(UserInputType::MotorControl, motorValues);

    assertActualPacketTypeMatchesExpected(packet, PacketType::MotorControl);
    assertActualPacketSizeMatchesExpected(packet, DataLength::MotorControl);
}

TEST_F(UdpPacketMakerTest, makeGripperOpenPacket)
{
    const auto & packet = sut->makePacket(UserInputType::GripperOpen, motorValues);

    assertActualPacketTypeMatchesExpected(packet, PacketType::ServoControl);
    assertActualPacketSizeMatchesExpected(packet, DataLength::ServoControl);
}

TEST_F(UdpPacketMakerTest, makeGripperClosePacket)
{
    const auto & packet = sut->makePacket(UserInputType::GripperClose, motorValues);

    assertActualPacketTypeMatchesExpected(packet, PacketType::ServoControl);
    assertActualPacketSizeMatchesExpected(packet, DataLength::ServoControl);
}

TEST_F(UdpPacketMakerTest, makeStopPacket)
{
    const auto & packet = sut->makePacket(UserInputType::EmergencyStop, motorValues);

    assertActualPacketTypeMatchesExpected(packet, PacketType::EmergencyStop);
    assertActualPacketSizeMatchesExpected(packet, DataLength::EmergencyStop);
}
