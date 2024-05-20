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

