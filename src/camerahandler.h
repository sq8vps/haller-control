#ifndef CAMERAHANDLER_H
#define CAMERAHANDLER_H

#include <QObject>
#include <QImage>
#include <QThread>
#include "cameraworker.h"

class CameraHandler : public QObject {
    Q_OBJECT
public:

    CameraHandler();
    ~CameraHandler();

    void reconnect();
    void connect();

    CameraWorker* worker;


signals:
    void newFrame(QImage qimg);
    void cameraStatus(CameraWorker::CameraStatus cameraStatus);
    void cameraTemperature(float temp);
    void cameraCpuUsage(float cpuUsage);
    void cameraLaserStatus(int laserStatus);
    void setModeTW(CameraWorker::CameraMode cameraMode);
    void stopCamera();
    void restartCamera();

public slots:
    void setMode(CameraWorker::CameraMode cameraMode);

private slots:
    void getFrameFW(QImage qimg);
    void getCameraStatusFW(CameraWorker::CameraStatus cameraStatus);
    void getCameraTemperatureFW(float temp);
    void getCameraCpuUsageFW(float cpuUsage);
    void getCameraLaserStatusFW(int laserStatus);


private:
    CameraWorker::CameraMode cMode{CameraWorker::CameraMode::LowLatency};
    QThread* workerThread;
    bool stopped{false};

};

#endif // CAMERAHANDLER_H
