#include <array>
#include <QLayout>

#include "MainWindow.hpp"
#include "Definitions.hpp"
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

// TODO make two cameras
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
    // TODO fix button close/open
    connect(ui->gripperCloseButton, &QPushButton::pressed, this, [this]{handleUserInput(UserInputType::GripperClose);});
    connect(ui->gripperOpenButton, &QPushButton::pressed, this, [this]{handleUserInput(UserInputType::GripperOpen);});
    connect(ui->stopButton, &QPushButton::released, this, [this]{handleUserInput(UserInputType::EmergencyStop);});
}

void MainWindow::handleUserInput(UserInputType inputType)
{
    std::array<QString, numOfMotors> motorValues{};
    if(inputType == UserInputType::MotorControl)
    {
        updateMotorValues(motorValues);
    }
    udpNode->sendMessage(inputType, motorValues);
}

void MainWindow::updateMotorValues(std::array<QString, numOfMotors>& motorValues)
{
    motorValues.at(0) = ui->motor1->text();
    motorValues.at(1) = ui->motor2->text();
    motorValues.at(2) = ui->motor3->text();
    motorValues.at(3) = ui->motor4->text();
    motorValues.at(4) = ui->motor5->text();
}
