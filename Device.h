/*
 * Device class stores the state of the Device. The main window displays based on the Device data.
*/

#ifndef DEVICE_H
#define DEVICE_H
#include "mainwindow.h"
#include "Record.h"
#include <QObject>
#include <QString>

const int MAX_RECORDS = 256;

class Records;
class MainWindow;


class Device : public QObject
{
Q_OBJECT

private:
    bool on; // Stores whether the device is on.
    bool recording; // Stores whether the device is currently recording.
    bool isTouchingSkin; // Stores whether the device is touching the patient's skin.
    bool gracePeriod; //Stores whether the device is currently in the grace period after it has lost skin connection.
    double batteryPercentage;
    double current; //in milliamps
    double frequency; //0.5, 77, 100 hz
    QString waveform; //Alpha, Betta, Gamma
    QTimer* timer; //Timer that counts down the time during a session and idle timer when not touching skin
    int time; //20, 40, 60
    int countDown; //counts down the time in a session
    int timeIdle; // In seconds. QTimer will increment this. Represents the time the device has been idle.
    int timeElapsed;  // In seconds. QTimer will increment this. Represents the time the device has been on.
    Record* records[MAX_RECORDS];
    int numRecords;
    MainWindow* display;

public:
    Device(MainWindow*);
    ~Device();

    //Setters
    void setBatteryPercentage(int);
    void setFrequency(double);
    void setCurrent(double);
    void setWaveform(QString const);
    void setTime(int);

    //Getters
    bool getOn();
    double getBattery();
    double getFrequency();
    double getCurrent();
    QString getWaveform();
    int getTime();
    int getCountDown();
    bool getSkin();
    bool getRecording();
    QString getRecordsAsText();
    QString getTimeElapsed();

    // Functions
    void toggleTouchingSkin();
    void checkSession();
    void checkBattery();
    void addRecord(Record* r);



public slots:
    // Setters and state changes.
    void toggle(); // on-off toggle.
    void toggleRecording(); // Negates recording boolean value.
    void changeFrequency();
    void changeWaveform();
    void changeTime();
    void changeCurrentUp(); // 59 incrementing.
    void changeCurrentDown(); // 100 decrementing.
    void resetTimeIdle(); // Resets idle timer variable.
    void updateTimes(); // Increments time keeping variables.
    void updateRecords();

private slots:
    void timerUpdate();
};




#endif // DEVICE_H
