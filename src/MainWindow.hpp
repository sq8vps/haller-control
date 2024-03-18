#pragma once

#include "UdpNode.hpp"
#include "Logger.hpp"
#include "Definitions.hpp"
#include "JoystickWorker.hpp"

#include <QMainWindow>
#include <SFML/Window/Window.hpp>
#include <SFML/Window/Event.hpp>
#include <QWidget>
#include <QLineEdit>
#include <QKeyEvent>
#include <QRegularExpressionValidator>
#include <QThread>

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
    void setLogText(QString textToLog, LogType logType);
    void printGamepadDebugMessage(QString message);
    void saveLogsToFile();
private:
    void setIcons();
    void setCameraIcons();
    void connecSignalsToSlots();
    void updateMotorValuesToSend(std::array<float, numOfMotors>& motorValues);
    void setValidators();
    void initMotorButtons();
    void clearMotorTextFields();

    std::vector<QLineEdit *> motorTextFields;
    std::shared_ptr<UdpNode> udpNode;
    QRegularExpressionValidator *validator;
    Logger *logger;
    Ui::MainWindow *ui;
    QTabWidget *tabWidget;
    QThread *thread;
    JoystickWorker *worker;
    sf::Window *joystickWindow;
    sf::Event* event;
};
