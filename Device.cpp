#include "Device.h"
#include <QTimer>

Device::Device(MainWindow* mainWindow)
{
    on = true;
    recording = true;
    isTouchingSkin = false;
    gracePeriod = false;
    batteryPercentage = 100;
    current = 100;
    frequency = 0.5;
    waveform = "Alpha";
    timer = new QTimer(this);
    time = 20;
    countDown = -1;
    timeIdle = 0;
    numRecords = 0;
    display = mainWindow;

    connect(timer, SIGNAL(timeout()), this, SLOT(timerUpdate()));
    timer->start(1000);
}

Device::~Device(){
  for(int i = 0; i<numRecords; i++){
    delete records[i];
  }
  qInfo("deconstructor");
}

//Setters
void Device::setBatteryPercentage(int per){
    batteryPercentage = per;
}

void Device::setFrequency(double freq){
    frequency = freq;
}

void Device::setCurrent(double cur){
    current = cur;
}

void Device::setWaveform(QString const wave){
    waveform = wave;
}

void Device::setTime(int timer){
    time = timer;
}

//Getters
bool Device::getOn(){
    return on;
}

double Device::getBattery(){
    return batteryPercentage;
}

double Device::getFrequency(){
    return frequency;
}

double Device::getCurrent(){
    return current;
}

QString Device::getWaveform(){
    return waveform;
}

int Device::getTime(){
    return time;
}

int Device::getCountDown(){
    return countDown;
}

bool Device::getSkin(){
  return isTouchingSkin;
}

bool Device::getRecording(){
  return recording;
}

QString Device::getTimeElapsed()    {
    return QDateTime::fromTime_t(timeElapsed).toString("hh:mm:ss");
}

QString Device::getRecordsAsText()  {
    QString s = "";
    QString line = "-------------------------\n";
    for(int i=numRecords-1; i>=0; --i) {
        s += line;
        s += "RECORD " + QString::number(i) + "\n";
        s += line;
        s += records[i]->getRecordData();
    }
    return s;
}

//functions

void Device::updateRecords()    {
    records[numRecords-1]->setPower(current);
    display->updateRecordText();
}

void Device::updateTimes()  {
    if(isTouchingSkin == false) {
        if (timeIdle >= 30) { //shutdown cause idle
            toggle();
            timer->stop();
            return;
        }
        else if (timeIdle >= 5 && gracePeriod == true) { //stop treatment
            countDown = time;
            display->updateTimer();

        }
        timeIdle++;
    }
    timeElapsed++;
    timer->start(1000);
}

void Device::toggleTouchingSkin(){
    resetTimeIdle();

    isTouchingSkin = !isTouchingSkin;
    if(isTouchingSkin == true){ //touching skin
        qInfo("touching skin");
        display->updateCircuitLED(isTouchingSkin);
        if (gracePeriod == false) {
            countDown = time;
        }
    }
    else{ //not touching skin (for pausing and finished treatment)
        qInfo("NOT touching skin");
        gracePeriod = true;
        display->updateCircuitLED(isTouchingSkin);
    }
    resetTimeIdle();
}


void Device::checkSession(){
    if (countDown <= 0) { //end of treatment
        display->updateApplyToSkin(false);
        gracePeriod = false;
        display->updateCircuitLED(false);
        if(on && recording) {
            addRecord(new Record(getTimeElapsed(),waveform,frequency,time,current));
        }
        display->updateRecordText();
    }
}


void Device::checkBattery(){
    if (batteryPercentage <= 5) {
        display->updateBatteryLED(true);
        if(batteryPercentage == 5){
          display->batteryWarning();
        }
        else if(batteryPercentage == 2){
          display->batteryWarning();
        }
    }
    else {
        display->updateBatteryLED(false);
    }
}


void Device::addRecord(Record* r){
  records[numRecords++] = r;
  qInfo("added to records");
}

void Device::timerUpdate() {
    if(isTouchingSkin){
        display->updateTimer();

        if(batteryPercentage == 2){
            display->updateScreen(false);
            toggle();
            timer->stop();
            return;
        }
        else{
            countDown -= 1;
            checkSession();
            display->updateTimer();
            batteryPercentage -= 1;
            display->updateBattery();
            checkBattery();
        }
        timer->start(1000);
    }
    else { //idle timer
        updateTimes();
    }
}

//slots
void Device::toggle(){
    if(on == false)  { //off to on
        timeElapsed = 0;
        resetTimeIdle();
        recording = true;
        timer->start(1000);
        recording = true;
        setFrequency(0.5);
        setCurrent(100);
        setWaveform("Alpha");
        setTime(20);
        checkBattery();
        countDown = time;
    }
    else if (on == true){ //on to off
        //isTouchingSkin = false;
        gracePeriod = false;
        timer->stop();
        display->updateCircuitLED(false);
        display->updateBatteryLED(false);
    }
    on = !on;
    display->updateScreen(on);
}


void Device::toggleRecording(){
    resetTimeIdle();
    /*if(isTouchingSkin)  {
        qInfo("Cannot start/stop recording within a session.");
        return;
    }*/
    recording = !recording;
    display->updateRecordingLED(recording);
}


void Device::changeFrequency(){
    resetTimeIdle();
    if(gracePeriod || isTouchingSkin) return;

    if (frequency == 0.5) {
        setFrequency(77);
        display->updateFrequency();
    }
    else if (frequency == 77) {
        setFrequency(100);
        display->updateFrequency();
    }
    else if (frequency == 100) {
        setFrequency(0.5);
        display->updateFrequency();
    }
}


void Device::changeWaveform(){
    resetTimeIdle();
    if(gracePeriod || isTouchingSkin) return;

    if (waveform == "Alpha") {
        setWaveform("Betta");
        display->updateWaveform();
    }
    else if (waveform == "Betta") {
        setWaveform("Gamma");
        display->updateWaveform();
    }
    else if (waveform == "Gamma") {
        setWaveform("Alpha");
        display->updateWaveform();
    }
}


void Device::changeTime(){
    resetTimeIdle();
    if(gracePeriod || isTouchingSkin) return;

    if (time == 20) {
        setTime(40);
        display->updateTime();
    }
    else if (time == 40) {
        setTime(60);
        display->updateTime();
    }
    else if (time == 60) {
        setTime(20);
        display->updateTime();
    }
}


void Device::changeCurrentUp(){
    resetTimeIdle();

    if(current < 500){
        setCurrent(current += 50);
        display->updateCurrent();
    }
}


void Device::changeCurrentDown(){
    resetTimeIdle();

    if(current > 100){
        setCurrent(current -= 100);
        display->updateCurrent();
    }
    else{
        setCurrent(0);
        display->updateCurrent();
    }
}

void Device::resetTimeIdle(){
    timeIdle = 0;
    qInfo("reset idle time");
}
