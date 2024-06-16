#ifndef CAMERAWORKER_H
#define CAMERAWORKER_H

#include <QObject>
#include <QImage>
#include <depthai/depthai.hpp>
#include <QThread>

class CameraWorker : public QObject {
    Q_OBJECT
public:
    enum CameraMode {
        Color,
        LowLatency,
        NightVision,
        DepthVision
    };

    enum CameraStatus {
        Up,
        Down,
        Connecting,
        Error
    };

    enum CollisionState {
        Ok,
        Warning,
        Critical,
        Off
    };

    CameraWorker(CameraMode mode);
    void setMode(CameraMode cameraMode);
    void setCollision(bool collision_);

public slots:
    void process();

    void stop();
    void restart();

signals:
    void newFrame(QImage image);
    void cameraStatus(CameraStatus cameraStatus);
    void cameraTemperature(float temp);
    void cameraCpuUsage(float cpuUsage);
    void cameraLaserStatus(int laserStatus);
    void cameraCollision(CollisionState collisionState);


private:
    void setPipeline(CameraMode cameraMode);
    dai::Pipeline pipeline;
    std::shared_ptr<dai::Device> device;
    CameraMode cMode;
    void printSystemInformation(dai::SystemInformation info);
    bool running{true};
    bool collision{false};
};

#endif // CAMERAWORKER_H
