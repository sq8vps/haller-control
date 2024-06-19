#include "grippertimer.hpp"
#include <QDateTime>

GripperTimer::GripperTimer() {}
GripperTimer::~GripperTimer() {}

void GripperTimer::updateRotation(GripperTimer::Direction direction)
{
    if(QDateTime::currentMSecsSinceEpoch() > nextUpdate)
    {
        nextUpdate = QDateTime::currentMSecsSinceEpoch() + GripperRefreshTime;
        switch(direction)
        {
            case CLOCKWISE:
                angle += (GripperFullRange / static_cast<float>(2 * GripperFullRangeTime / GripperRefreshTime));
                break;
            case COUNTERCLOCKWISE:
                angle -= (GripperFullRange / static_cast<float>(2 * GripperFullRangeTime / GripperRefreshTime));
                break;
        }

        if(angle > (GripperFullRange / 2.f))
            angle = GripperFullRange / 2.f;
        if(angle < (-GripperFullRange / 2.f))
            angle = -GripperFullRange / 2.f;

        emit send(angle);
    }
}
