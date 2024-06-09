#pragma once

#include <QObject>

constexpr float GripperFullRange{3.14f};
constexpr uint64_t GripperRefreshTime{100};
constexpr uint64_t GripperFullRangeTime{4000};
constexpr uint8_t GripperServoNumber{6};

class GripperTimer : public QObject
{
    Q_OBJECT
public:
    GripperTimer();
    ~GripperTimer();
    enum Direction
    {
        CLOCKWISE,
        COUNTERCLOCKWISE,
    };

public slots:
    void updateRotation(Direction direction);
signals:
    void send(const float angle);
private:
    float angle{0.f};
    uint64_t nextUpdate{0};
};
