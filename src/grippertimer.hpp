#pragma once

#include <QObject>

constexpr float GripperFullRange{3.14f};
constexpr uint64_t GripperRefreshTime{20};
constexpr uint64_t GripperFullRangeTime{500};
constexpr uint8_t GripperServoNumber{0};

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
