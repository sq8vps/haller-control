#include <array>
#include <QLayout>
#include <QRegularExpressionValidator>

#include "MainWindow.hpp"
#include "Definitions.hpp"
#include "./ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    udpNode = std::make_shared<UdpNode>();
    ui->setupUi(this);

    // TODO logging system
    setWindowTitle(tr("Haller control panel"));
    setValidators();
    setIcons();
    connectButtonSignalsToSlots();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setIcons()
{
    setCameraIcon();
}

void MainWindow::setCameraIcon()
{
    QPixmap cameraPix(":/resource/img/turtle.jpg");
    int w = ui->leftCamera->width();
    int h = ui->leftCamera->height();
    ui->leftCamera->setPixmap(cameraPix.scaled(w, h));
    ui->rightCamera->setPixmap(cameraPix.scaled(w, h));
}

void MainWindow::connectButtonSignalsToSlots()
{
    connect(ui->sendMotorDataButton, &QPushButton::released, this, [this]{handleUserInput(UserInputType::MotorControl);});
    connect(ui->gripperCloseButton, &QPushButton::pressed, this, [this]{handleUserInput(UserInputType::GripperClose);});
    connect(ui->gripperOpenButton, &QPushButton::pressed, this, [this]{handleUserInput(UserInputType::GripperOpen);});
    connect(ui->stopButton, &QPushButton::released, this, [this]{handleUserInput(UserInputType::EmergencyStop);});
}

void MainWindow::handleUserInput(UserInputType inputType)
{
    std::array<float, numOfMotors> motorValues{};
    if(inputType == UserInputType::MotorControl)
    {
        updateMotorValues(motorValues);
    }
    udpNode->sendMessage(inputType, motorValues);
}

void MainWindow::updateMotorValues(std::array<float, numOfMotors>& motorValues)
{
    motorValues.at(0) = ui->motor1->text().toFloat();
    motorValues.at(1) = ui->motor2->text().toFloat();
    motorValues.at(2) = ui->motor3->text().toFloat();
    motorValues.at(3) = ui->motor4->text().toFloat();
    motorValues.at(4) = ui->motor5->text().toFloat();
}

void MainWindow::setValidators()
{
    // numbers from -1 to 1 inclusive with max 6 digits after decimal
    QRegularExpression rx("-1|0|1|^-?0.[0-9]{1,6}$");
    QValidator *validator = new QRegularExpressionValidator(rx, this);

    ui->motor1->setValidator(validator);
    ui->motor2->setValidator(validator);
    ui->motor3->setValidator(validator);
    ui->motor4->setValidator(validator);
    ui->motor5->setValidator(validator);
}
