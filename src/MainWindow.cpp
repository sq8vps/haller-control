#include <string>
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

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_W:
        handleUserInput("Gripper going up");
        break;
    case Qt::Key_S:
        handleUserInput("Gripper going down");
        break;
    case Qt::Key_A:
        handleUserInput("Gripper going back");
        break;
    case Qt::Key_D:
        handleUserInput("Gripper going forward");
        break;
    case Qt::Key_I:
        handleUserInput("Haller going up");
        break;
    case Qt::Key_K:
        handleUserInput("Haller going down");
        break;
    case Qt::Key_J:
        handleUserInput("Haller going back");
        break;
    case Qt::Key_L:
        handleUserInput("Haller going forward");
        break;
    default:
        break;
    }
}

void MainWindow::setIcons()
{
    setCameraIcon();
    setButtonIcons();
}

void MainWindow::setCameraIcon()
{
    QPixmap cameraPix(":/resource/turtle.jpg");
    int w = ui->image->width();
    int h = ui->image->height();
    ui->image->setPixmap(cameraPix.scaled(w, h));
}

void MainWindow::setButtonIcons()
{
    QPixmap buttonPix(":/resource/button.png");

    ui->hallerUpButton->setIcon(QIcon(buttonPix));
    ui->hallerDownButton->setIcon(QIcon(buttonPix));
    ui->hallerLeftButton->setIcon(QIcon(buttonPix));
    ui->hallerRightButton->setIcon(QIcon(buttonPix));

    ui->gripperUpButton->setIcon(QIcon(buttonPix));
    ui->gripperDownButton->setIcon(QIcon(buttonPix));
    ui->gripperLeftButton->setIcon(QIcon(buttonPix));
    ui->gripperRightButton->setIcon(QIcon(buttonPix));
}

void MainWindow::connectButtonSignalsToSlots()
{
    connect(ui->hallerUpButton, &QPushButton::released, this, [this]{handleUserInput("Haller going up");});
    connect(ui->hallerDownButton, &QPushButton::released, this, [this]{handleUserInput("Haller going down");});
    connect(ui->hallerLeftButton, &QPushButton::released, this, [this]{handleUserInput("Haller going left");});
    connect(ui->hallerRightButton, &QPushButton::released, this, [this]{handleUserInput("Haller going right");});

    connect(ui->gripperUpButton, &QPushButton::released, this, [this]{handleUserInput("Gripper going up");});
    connect(ui->gripperDownButton, &QPushButton::released, this, [this]{handleUserInput("Gripper going down");});
    connect(ui->gripperLeftButton, &QPushButton::released, this, [this]{handleUserInput("Gripper going left");});
    connect(ui->gripperRightButton, &QPushButton::released, this, [this]{handleUserInput("Gripper going right");});
}

void MainWindow::handleUserInput(QString textToShow)
{
    ui->plainTextEdit->setPlainText(textToShow);
    udpNode->sendMessage(textToShow);
}
