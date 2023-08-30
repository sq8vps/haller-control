#include <array>
#include <vector>
#include <QLayout>
#include <QValidator>
#include <QColor>

#include "MainWindow.hpp"
#include "./ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    udpNode = std::make_shared<UdpNode>();
    ui->setupUi(this);
    logger = Logger::getLogger();

    setWindowTitle(tr("Haller control panel"));
    initMotorButtons();
    connecSignalsToSlots();
    logger->log("Jakies info 1\n", LogType::Inf);
    logger->log("Jakies info 2\n", LogType::Inf);
    logger->log("Jakies info 3\n", LogType::Inf);
    logger->log("Dupsko\n", LogType::Warning);
    logger->log("Dupsko znowu\n", LogType::Error);
    logger->log("A teraz info\n", LogType::Inf);

    setValidators();
    setIcons();
}

MainWindow::~MainWindow()
{
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
    ui->rightCamera->setPixmap(cameraPix.scaled(w, h));
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
            ui->logTextField->setTextColor("white");
            break;
    }

    ui->logTextField->insertPlainText(textToLog);
}

void MainWindow::connecSignalsToSlots()
{
    connect(ui->sendMotorDataButton, &QPushButton::released, this, [this]{handleUserInput(UserInputType::MotorControl);});
    connect(ui->gripperCloseButton, &QPushButton::pressed, this, [this]{handleUserInput(UserInputType::GripperClose);});
    connect(ui->gripperOpenButton, &QPushButton::pressed, this, [this]{handleUserInput(UserInputType::GripperOpen);});
    connect(ui->stopButton, &QPushButton::released, this, [this]{handleUserInput(UserInputType::EmergencyStop);});

    QObject::connect(logger, &Logger::logSignal, this, &MainWindow::setLogText);
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
}
