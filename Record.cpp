#include "Record.h"

Record::Record( QString timeElapsed, QString waveform, double frequency, int duration, int powerLevel) : waveform(waveform), frequency(frequency), duration(duration), powerLevel(powerLevel), timeElapsed(timeElapsed)   {
}
Record::~Record()   {}

QString Record::getRecordData() {
    QString s = "";

    s += "(" + timeElapsed + ")\n";
    s += "Waveform: " + waveform + "\n";
    s += "Frequency: " + QString::number(frequency) + "Hz\n";
    s += "Duration: " + QString::number(duration) + "s\n";
    s += "Current: " +  QString::number(powerLevel) + "uA\n";
    return s;
}

double Record::getFrequency(){
  return frequency;
}

int Record::getDuration(){
  return duration;
}

int Record::getPower(){
  return powerLevel;
}

QString Record::getWaveform(){
  return waveform;
}

void Record::setPower(int p){
  powerLevel = p;
}

void Record::setDuration(int d){
  duration = d;
}

void Record::setFrequency(double f){
  frequency = f;
}

void Record::setWaveform(QString w){
  waveform = w;
}
