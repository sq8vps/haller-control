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
protected:
    void keyPressEvent(QKeyEvent *event) override;
private slots:
    void handleButtons(QString textToShow);
private:
    void setIcons();
    void setCameraIcon();
    void setButtonIcons();
    void connectButtonSignalsToSlots();

    UdpNode *udpNode;
    Ui::MainWindow *ui;
};
