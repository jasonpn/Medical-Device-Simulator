/*
 * This class handles the display elements of the application window.
*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QTime>

class Device;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // Setters to uodate display elements.
    void updateFrequency();
    void updateWaveform();
    void updateTime();
    void updateTimer();
    void updateCurrent();
    void updateBattery();
    void updateScreen(bool);
    void updateRecordingLED(bool);
    void updateCircuitLED(bool);
    void updateBatteryLED(bool);
    void updateApplyToSkin(bool);
    void updateRecordText();
    void batteryWarning();

public slots:
    void on_applyToSkin_stateChanged(); // Handles checkbox value changes.
    void on_batterySpinBox_valueChanged(int);
    void on_faultButton_clicked(); // Handles fault button checkbox.

private:
    Ui::MainWindow *ui;
    Device *model;
};
#endif // MAINWINDOW_H
