#pragma once

#include "UdpNode.hpp"

#include <QMainWindow>
#include <QWidget>
#include <QTextEdit>
#include <QKeyEvent>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void handleUserInput(UserInputType inputType);
private:
    void setIcons();
    void setCameraIcon();
    void connectButtonSignalsToSlots();
    void updateMotorValues(std::array<float, numOfMotors>& motorValues);
    void setValidators();

    std::shared_ptr<UdpNode> udpNode;
    Ui::MainWindow *ui;
};
