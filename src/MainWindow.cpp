#include "MainWindow.hpp"

#include <array>
#include <vector>

#include <QLayout>
#include <QValidator>
#include <QColor>
#include <QMessageBox>
#include <QThread>

#include <SFML/Window/Joystick.hpp>
#include <SFML/Window/Window.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/VideoMode.hpp>

#include "./ui_MainWindow.h"
#include "JoystickWorker.hpp"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    udpNode = std::make_shared<UdpNode>();
    ui->setupUi(this);
    logger = Logger::getLogger();

    // thread that handles joystick input
    joystickInputThread = new QThread;
    joystickWorker = new JoystickWorker();
    joystickWorker->moveToThread(joystickInputThread);

    joystickInputThread->start();

    cameraHandler = new CameraHandler();

    setWindowTitle(tr("Narwhal control panel"));
    connectSignalsToSlots();
    setValidators();
    setIcons();
}

MainWindow::~MainWindow()
{
    joystickInputThread->exit();
    joystickInputThread->terminate();
    delete joystickWorker;
    delete joystickInputThread;
    delete validator;
    delete ui;
}

void MainWindow::setIcons()
{
    setCameraIcons();
}

void MainWindow::setCameraIcons()
{
    QPixmap cameraPix(":/resource/img/turtle.jpg");
    int w = ui->leftCamera->width();
    int h = ui->leftCamera->height();
    ui->leftCamera->setPixmap(cameraPix.scaled(w, h));
}


void MainWindow::setLogText(QString textToLog, LogType logType)
{
    switch(logType)
    {
        case LogType::Warning:
            ui->logTextField->setTextColor("yellow");
            break;
        case LogType::Error:
            ui->logTextField->setTextColor("red");
            break;
        default:
            // TODO in light mode text not visible - fix
            ui->logTextField->setTextColor("white");
            break;
    }
    ui->logTextField->insertPlainText(textToLog);
}

void MainWindow::printGamepadDebugMessage(QString message)
{
    ui->gamePadOutput->insertPlainText(message);
}

void MainWindow::connectSignalsToSlots()
{
    handleUIButtonsSignals();
    handleJoystickSignals();
    handleDataSendSignals();
    handleCameraSignals();

    QObject::connect(logger, &Logger::logSignal, this, &MainWindow::setLogText);
}

void MainWindow::handleJoystickSignals()
{
    connect(joystickWorker, &JoystickWorker::error, this, [this]{printGamepadDebugMessage("Joystick error!\n");});
    connect(joystickInputThread, &QThread::started, joystickWorker, [this]{joystickWorker->process();});
    connect(joystickWorker, SIGNAL(finished()), joystickInputThread, SLOT(quit()));

    connect(joystickWorker, &JoystickWorker::gripperClose, this, [this]{printGamepadDebugMessage("Closing gripper\n");});
    connect(joystickWorker, &JoystickWorker::gripperOpen, this, [this]{printGamepadDebugMessage("Opening gripper\n");});
    connect(joystickWorker, &JoystickWorker::emergencyStop, this, [this]{printGamepadDebugMessage("stop\n");});
    connect(joystickWorker, &JoystickWorker::motorControl, &dataSendTimer, &DataSendTimer::updateForceVector);
    connect(joystickWorker, &JoystickWorker::setZtrim, this,
            [this](float x){printGamepadDebugMessage("Z axis trim set to " + QString::number(x, 'f', 3) + "\n"); ui->leTrim->setText(QString::number(x, 'f', 3));});

    connect(joystickWorker, SIGNAL(finished()), joystickWorker, SLOT(deleteLater()));
    connect(joystickInputThread, SIGNAL(finished()), joystickInputThread, SLOT(deleteLater()));
}

void MainWindow::handleDataSendSignals()
{
    connect(&dataSendTimer, &DataSendTimer::sendForceVector, this, &MainWindow::sendMotorValues);
}

void MainWindow::handleUIButtonsSignals()
{

    connect(ui->saveLogsButton, &QPushButton::released, this, &MainWindow::saveLogsToFile);
}

void MainWindow::handleCameraSignals()
{
    connect(cameraHandler, &CameraHandler::newFrame, this, &MainWindow::updateFrame);
    connect(ui->camReconnectButton, &QPushButton::released, cameraHandler, &CameraHandler::reconnect);
    connect(this, &MainWindow::setCamMode, cameraHandler, &CameraHandler::setMode);
    connect(ui->camColorMode, &QRadioButton::toggled, this, &MainWindow::on_camColorMode_clicked);
    connect(ui->camLowLatencyMode, &QRadioButton::toggled, this, &MainWindow::on_camLowLatencyMode_clicked);
    connect(ui->camNightVision, &QRadioButton::toggled, this, &MainWindow::on_camNightVision_clicked);
    connect(ui->camDepthVisionMode, &QRadioButton::toggled, this, &MainWindow::on_camDepthVision_clicked);
    connect(ui->camCollisionCheckbox, &QCheckBox::checkStateChanged, this, &MainWindow::on_camColision_switched);
    connect(this, &MainWindow::setCollision, cameraHandler->worker, &CameraWorker::setCollision);
    connect(cameraHandler, &CameraHandler::cameraStatus, this, &MainWindow::updateCameraStatus);
    connect(cameraHandler, &CameraHandler::cameraTemperature, this, &MainWindow::updateCameraTemperature);
    connect(cameraHandler, &CameraHandler::cameraCpuUsage, this, &MainWindow::updateCameraCpuUsage);
    connect(cameraHandler, &CameraHandler::cameraLaserStatus, this, &MainWindow::updateCameraLaserStatus);
    connect(cameraHandler->worker, &CameraWorker::cameraCollision, this, &MainWindow::updateCameraCollision);

    connect(cameraHandler, &CameraHandler::stopCamera, cameraHandler->worker, &CameraWorker::stop);
    connect(cameraHandler, &CameraHandler::restartCamera, cameraHandler->worker, &CameraWorker::restart);
}

void MainWindow::sendMotorValues(const std::array<float, numOfMotors>& motorValues)
{
    udpNode->sendMessage(UserInputType::MotorControl, motorValues);
    ui->leM1->setText(QString::number(motorValues[0], 'f', 3));
    ui->leM2->setText(QString::number(motorValues[1], 'f', 3));
    ui->leM3->setText(QString::number(motorValues[2], 'f', 3));
    ui->leM4->setText(QString::number(motorValues[3], 'f', 3));
    ui->leM5->setText(QString::number(motorValues[4], 'f', 3));
    ui->leM6->setText(QString::number(motorValues[5], 'f', 3));
}

void MainWindow::updateFrame(QImage image)
{
    int w = ui->leftCamera->width();
    int h = ui->leftCamera->height();
    ui->leftCamera->setPixmap(QPixmap::fromImage(image).scaled(w, h, Qt::KeepAspectRatio));
}

void MainWindow::saveLogsToFile()
{
    QString logs{ui->logTextField->toPlainText()};
    logger->saveLogs(logs);
    ui->logTextField->clear();
    QMessageBox::information(this, tr("Log info"), tr("Logs saved") );
}

void MainWindow::handleUserInput(UserInputType inputType)
{
    std::array<float, numOfMotors> motorValues{};
    if(inputType == UserInputType::MotorControl)
    {
        updateMotorValuesToSend(motorValues);
    }
    udpNode->sendMessage(inputType, motorValues);
}

void MainWindow::updateMotorValuesToSend(std::array<float, numOfMotors>& motorValues)
{
    for(int i = 0; i < numOfMotors; ++i)
    {
        motorValues.at(i) = motorTextFields.at(i)->text().toFloat();
    }
}

void MainWindow::setValidators()
{
    // numbers from -1 to 1 inclusive with max 6 digits after decimal
    QRegularExpression rx("-1|0|1|^-?0.[0-9]{1,6}$");
    validator = new QRegularExpressionValidator(rx, this);
    for(const auto& field : motorTextFields)
    {
        field->setValidator(validator);
    }
}


void MainWindow::on_hsMotorGain_valueChanged(int value)
{
    float x = static_cast<float>(value) / 100.f;
    joystickWorker->setJoystickGain(x);
    ui->laMotorGain->setText(QString::number(x, 'f', 1));
}


void MainWindow::on_rbLinear_clicked()
{
    joystickWorker->setEqualization(JoystickWorker::LINEAR);
}


void MainWindow::on_rbSquare_clicked()
{
    joystickWorker->setEqualization(JoystickWorker::SQUARE);
}


void MainWindow::on_rbCube_clicked()
{
    joystickWorker->setEqualization(JoystickWorker::CUBE);
}


void MainWindow::on_rbInverseSquare_clicked()
{
    joystickWorker->setEqualization(JoystickWorker::INVERSE_SQUARE);
}


void MainWindow::on_rbInverseCube_clicked()
{
    joystickWorker->setEqualization(JoystickWorker::INVERSE_CUBE);
}

void MainWindow::on_camColorMode_clicked()
{
    emit setCamMode(CameraWorker::CameraMode::Color);
    ui->camCollisionCheckbox->setEnabled(true);
}

void MainWindow::on_camLowLatencyMode_clicked()
{
    emit setCamMode(CameraWorker::CameraMode::LowLatency);
    ui->camCollisionCheckbox->setEnabled(false);
    emit setCollision(false);
}

void MainWindow::on_camNightVision_clicked() {
    emit setCamMode(CameraWorker::CameraMode::NightVision);
    ui->camCollisionCheckbox->setEnabled(true);
    emit setCollision(false);
}

void MainWindow::on_camDepthVision_clicked() {
    emit setCamMode(CameraWorker::CameraMode::DepthVision);
    ui->camCollisionCheckbox->setEnabled(true);
    emit setCollision(false);
}

void MainWindow::on_camColision_switched(Qt::CheckState checkState){
    std::cout<<"MAIN "<<checkState<<std::endl;
    if (checkState == Qt::Checked) {
        emit setCollision(true);
    } else {
        emit setCollision(false);
    }
}

void MainWindow::updateCameraStatus(CameraWorker::CameraStatus cameraStatus) {
    int w;
    int h;
    QString path;

    switch (cameraStatus) {
    case CameraWorker::CameraStatus::Up:
        path = ":/resource/img/green-led.png";
        break;
    case CameraWorker::CameraStatus::Down:
        path = ":/resource/img/blank-led.png";
        setCameraIcons();
        break;
    case CameraWorker::CameraStatus::Connecting:
        path = ":/resource/img/yellow-led.png";
        break;
    case CameraWorker::CameraStatus::Error:
        path = ":/resource/img/red-led.png";
        setCameraIcons();
        break;
    default:
        path = ":/resource/img/red-led.png";
        setCameraIcons();
        break;
    }
    QPixmap img(path);
    w = ui->camStatus->width();
    h = ui->camStatus->height();
    ui->camStatus->setPixmap(img.scaled(w, h));
}

void MainWindow::updateCameraTemperature(float temp) {
    ui->camTemp->display((int) temp);
}

void MainWindow::updateCameraCpuUsage(float cpuUsage) {
    ui->camCpu->display((int) cpuUsage);
}

void MainWindow::updateCameraLaserStatus(int laserStatus) {
    int w;
    int h;
    QString path;
    switch(laserStatus) {
    case 1:
        path = ":/resource/img/red-led.png";
        break;
    case 0:
        path = ":/resource/img/blank-led.png";
        break;
    default:
        path = ":/resource/img/blank-led.png";
    }

    w = ui->camCollisionLabel->width();
    h = ui->camCollisionLabel->height();
    QPixmap img(path);
    ui->camCollisionLabel->setPixmap(img.scaled(w, h));
}

void MainWindow::updateCameraCollision(CameraWorker::CollisionState collisionState) {
    int w;
    int h;
    QString path;
    switch(collisionState) {
    case CameraWorker::CollisionState::Off:
        path = ":/resource/img/collision-blank.png";
        break;
    case CameraWorker::CollisionState::Ok:
        path = ":/resource/img/collision-green.png";
        break;
    case CameraWorker::CollisionState::Warning:
        path = ":/resource/img/collision-orange.png";
        break;
    case CameraWorker::CollisionState::Critical:
        path = ":/resource/img/collision-red.png";
        break;
    default:
        path = ":/resource/img/collision-blank.png";
        break;
    }

    w = ui->camCollisionLabel->width();
    h = ui->camCollisionLabel->height();
    QPixmap img(path);
    ui->camCollisionLabel->setPixmap(img.scaled(w, h));
}
