#include <array>
#include <QLayout>

#include "MainWindow.hpp"
#include "./ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    udpNode = std::make_shared<UdpNode>();
    ui->setupUi(this);

    setWindowTitle(tr("Haller control panel"));
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
    int w = ui->image->width();
    int h = ui->image->height();
    ui->image->setPixmap(cameraPix.scaled(w, h));
}

void MainWindow::connectButtonSignalsToSlots()
{
    connect(ui->sendMotorDataButton, &QPushButton::released, this, [this]{handleUserInput(UserInputType::MotorControl);});
    connect(ui->gripperCloseButton, &QPushButton::toggled, this, [this]{handleUserInput(UserInputType::GripperClose);});
    connect(ui->gripperOpenButton, &QPushButton::toggled, this, [this]{handleUserInput(UserInputType::GripperOpen);});
    connect(ui->stopButton, &QPushButton::released, this, [this]{handleUserInput(UserInputType::EmergencyStop);});
}

void MainWindow::handleUserInput(UserInputType inputType)
{  
    if(inputType == UserInputType::MotorControl)
    {
        std::array<QString, numOfMotors> motorValues{getMotorValues()};
        udpNode->sendMessage(inputType, motorValues);
    }
    else
    {
        udpNode->sendMessage(inputType);
    }
}

std::array<QString, numOfMotors> MainWindow::getMotorValues()
{
    std::array<QString, numOfMotors> motorValues{};
    motorValues.at(0) = ui->motor1->text();
    motorValues.at(1) = ui->motor2->text();
    motorValues.at(2) = ui->motor3->text();
    motorValues.at(3) = ui->motor4->text();
    motorValues.at(4) = ui->motor5->text();
    return motorValues;
}
