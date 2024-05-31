#include "camerahandler.h"
#include "cameraworker.h"

CameraHandler::CameraHandler() {
    workerThread = new QThread(this);
    worker = new CameraWorker(cMode);
    worker->moveToThread(workerThread);

    QObject::connect(workerThread, &QThread::started, worker, &CameraWorker::process);
    QObject::connect(worker, &CameraWorker::newFrame, this, &CameraHandler::newFrame);
    QObject::connect(worker, &CameraWorker::cameraTemperature, this, &CameraHandler::getCameraTemperatureFW);
    QObject::connect(worker, &CameraWorker::cameraCpuUsage, this, &CameraHandler::getCameraCpuUsageFW);
    QObject::connect(worker, &CameraWorker::cameraLaserStatus, this, &CameraHandler::getCameraLaserStatusFW);
    QObject::connect(worker, &CameraWorker::cameraStatus, this, &CameraHandler::getCameraStatusFW);
}

CameraHandler::~CameraHandler() {
    workerThread->quit();
    workerThread->wait();
}

void CameraHandler::reconnect() {
    if (workerThread->isRunning() && !stopped) {
        emit stopCamera();
        emit setModeTW(cMode);
        stopped = true;
    } else if (stopped){
        stopped = false;
        emit restartCamera();
    } else {
        connect();
    }

}

void CameraHandler::connect() {
    if (!workerThread->isRunning()) {
        emit setModeTW(cMode);
        workerThread->start();
    }
}

void CameraHandler::setMode(CameraWorker::CameraMode cameraMode) {
    cMode = cameraMode;
    worker->setMode(cameraMode);
}

void CameraHandler::getFrameFW(QImage qimg) {
    emit newFrame(qimg);
}

void CameraHandler::getCameraStatusFW(CameraWorker::CameraStatus cameraStatus) {
    emit CameraHandler::cameraStatus(cameraStatus);
}

void CameraHandler::getCameraTemperatureFW(float temp) {
    emit CameraHandler::cameraTemperature(temp);
}

void CameraHandler::getCameraCpuUsageFW(float cpuUsage) {
    emit CameraHandler::cameraCpuUsage(cpuUsage);
}

void CameraHandler::getCameraLaserStatusFW(int laserStatus) {
    emit CameraHandler::cameraLaserStatus(laserStatus);
}

