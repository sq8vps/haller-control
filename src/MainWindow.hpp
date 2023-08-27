#pragma once

#include "UdpNode.hpp"
#include "Definitions.hpp"

#include <QMainWindow>
#include <QWidget>
#include <QLineEdit>
#include <QKeyEvent>
#include <QRegularExpressionValidator>

#include <vector>

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
    void setCameraIcons();
    void connectButtonSignalsToSlots();
    void updateMotorValuesToSend(std::array<float, numOfMotors>& motorValues);
    void setValidators();
    void initMotorButtons();
    void clearMotorTextFields();

    std::vector<QLineEdit *> motorTextFields;
    std::shared_ptr<UdpNode> udpNode;
    QRegularExpressionValidator *validator;
    Ui::MainWindow *ui;
    QTabWidget *tabWidget;
};
