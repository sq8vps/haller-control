#pragma once

#include <vector>

#include <QMainWindow>
#include <QWidget>
#include <QLineEdit>
#include <QKeyEvent>
#include <QRegularExpressionValidator>
#include <QThread>

#include "UdpNode.hpp"
#include "Logger.hpp"
#include "Definitions.hpp"
#include "JoystickWorker.hpp"
#include "DataSendTimer.hpp"

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
    void setLogText(QString textToLog, LogType logType);
    void printGamepadDebugMessage(QString message);
    void saveLogsToFile();
    void sendMotorValues(const std::array<float, numOfMotors>& motorValues);
private:
    void setIcons();
    void setCameraIcons();
    void connectSignalsToSlots();
    void updateMotorValuesToSend(std::array<float, numOfMotors>& motorValues);
    void setValidators();
    void initMotorButtons();
    void clearMotorTextFields();
    void handleJoystickSignals();
    void handleDataSendSignals();
    void handleUIButtonsSignals();

    std::vector<QLineEdit *> motorTextFields;
    std::shared_ptr<UdpNode> udpNode;
    QRegularExpressionValidator *validator;
    Logger *logger;
    Ui::MainWindow *ui;
    QTabWidget *tabWidget;
    QThread *joystickInputThread;
    JoystickWorker *joystickWorker;
    DataSendTimer dataSendTimer;
};
