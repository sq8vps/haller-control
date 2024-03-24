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
    thread = new QThread;
    worker = new JoystickWorker();
    worker->moveToThread(thread);

    setWindowTitle(tr("Narwhal control panel"));
    initMotorButtons();
    connecSignalsToSlots();
    setValidators();
    setIcons();
    thread->start();
}

MainWindow::~MainWindow()
{
    delete thread;
    delete worker;
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
    ui->gamepadOutput->insertPlainText(message);
}

void MainWindow::connecSignalsToSlots()
{
    handleUIButtonsSignals();
    handleJoystickSignals();

    QObject::connect(logger, &Logger::logSignal, this, &MainWindow::setLogText);
}

void MainWindow::handleJoystickSignals()
{
    connect(worker, SIGNAL(error(QString)), this, SLOT(errorString(QString)));
    connect(thread, &QThread::started, worker, [this]{worker->process();});
    connect(worker, SIGNAL(finished()), thread, SLOT(quit()));
    connect(worker, &JoystickWorker::gripperClose, this, [this]{printGamepadDebugMessage("close\n");});
    connect(worker, &JoystickWorker::gripperOpen, this, [this]{printGamepadDebugMessage("open\n");});
    connect(worker, &JoystickWorker::emergencyStop, this, [this]{printGamepadDebugMessage("stop\n");});
    connect(worker, SIGNAL(finished()), worker, SLOT(deleteLater()));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
}

void MainWindow::handleUIButtonsSignals()
{
    connect(ui->sendMotorDataButton, &QPushButton::released, this, [this]{handleUserInput(UserInputType::MotorControl);});
    connect(ui->gripperCloseButton, &QPushButton::pressed, this, [this]{handleUserInput(UserInputType::GripperClose);});
    connect(ui->gripperOpenButton, &QPushButton::pressed, this, [this]{handleUserInput(UserInputType::GripperOpen);});
    connect(ui->stopButton, &QPushButton::released, this, [this]{handleUserInput(UserInputType::EmergencyStop);});

    connect(ui->saveLogsButton, &QPushButton::released, this, &MainWindow::saveLogsToFile);
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
        clearMotorTextFields();
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

void MainWindow::clearMotorTextFields()
{
    for(const auto & field : motorTextFields)
    {
        field->clear();
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

void MainWindow::initMotorButtons()
{
    motorTextFields.push_back(ui->motor1);
    motorTextFields.push_back(ui->motor2);
    motorTextFields.push_back(ui->motor3);
    motorTextFields.push_back(ui->motor4);
    motorTextFields.push_back(ui->motor5);
    motorTextFields.push_back(ui->motor6);
}
