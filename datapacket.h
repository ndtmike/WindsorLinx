/* ---------------------------------------------------------------------------
** This software is furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** File Name: DataPacket.h
**
** DataPacket - windsorlinx software
** Divides the DataPacket into reall data that can be used.
**
** Author: Michael W. Hoag
** Copyright Michael W. Hoag 2016
** Email: mike@ndtjames.com
** -------------------------------------------------------------------------*/


#ifndef DATAPACKET_H
#define DATAPACKET_H

#include <QString>
#include <QDateTime>
#include <QTextStream>
#include <QMessageBox>

class DataPacket
{
public:
    DataPacket();
    DataPacket(QByteArray Packet);
    ~DataPacket();

    QString testnumber;
    QString datatype;
    QString units;
    QString discardvalue;
    QList<QString> reading;
    QString average;
    QString median;
    QString minimum;
    QString maximum;
    QString angle;
    QString correlation;
    QDateTime time;

    bool error_flag;

private:
    void readDataPacket(QByteArray& p) ;
    QString readTestNumber( QByteArray& p)const;
    QString readDatatype(QByteArray& p)const;
    QString readUnits(QByteArray& p)const;
    QString readDiscardValue(QByteArray& p)const;
    QString readAverage(QByteArray& p);
    QString readMedian(QByteArray& p);
    QString readMinimum(QByteArray& p);
    QString readMaximum(QByteArray& p);
    QString readAngle(QByteArray& p)const;
    QString readCorrelation(QByteArray& p)const;
    QDateTime readDateTime(QByteArray& p)const;
    bool readReading(QByteArray& p);
    QByteArray formatReadings(int& length, int& index, QByteArray w );
};

#endif // DATAPACKET_H
