/*
 * Record class keeps track of the Device variables so they can later be printed.
*/

#ifndef RECORD_H
#define RECORD_H
#include <QString>
#include <QList>

class Record
{
    public:
        Record(QString timeElapsed="", QString waveform = "alpha", double frequency = 0.5, int duration = 0, int powerLevel = 0);
        ~Record();

        // Getters/Setters.
        double getFrequency();
        int getDuration();
        int getPower();
        QString getWaveform();
        QString getRecordData();
        void setFrequency(double);
        void setDuration(int);
        void setPower(int);
        void setWaveform(QString);

    private:
        // Record variables.
        QString waveform;
        double frequency;
        int duration, powerLevel;
        QString timeElapsed;

};

#endif // RECORD_H
