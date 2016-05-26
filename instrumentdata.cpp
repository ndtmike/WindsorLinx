/* ---------------------------------------------------------------------------
** This software is furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** File Name: instrumentdata.cpp
**
** plots the data on a graph - windsorlinx software
**
** Author: Michael W. Hoag
** Copyright Michael W. Hoag 2016
** Email: mike@ndtjames.com
** -------------------------------------------------------------------------*/
#include "instrumentdata.h"

InstrumentData::InstrumentData()
{
    rawInput = dummyData();
}

InstrumentData::InstrumentData(QString& dataIn)
{

    rawInput = dataIn;
}

InstrumentData::~InstrumentData()
{

}

QString InstrumentData::rawTime()
{
     QString buffer = rawInput.section(" ",rawTimeIndex(),rawTimeIndex());
     return buffer;
}

QString InstrumentData::rawDate()
{
    QString buffer = rawInput.section(" ",rawDateIndex(),rawDateIndex());
    return buffer;
}

QString InstrumentData::rawReading()
{
    QString buffer = rawInput.section(" ",rawReadingIndex(),rawReadingIndex());
    return buffer;
}

QString InstrumentData::rawMaterial()
{
    QString buffer = rawInput.section(" ",rawMaterialIndex(),rawMaterialIndex());
    return buffer;
}

QString InstrumentData::rawPercentage()
{
    QString buffer = rawInput.section(" ",rawPercentageIndex(),rawPercentageIndex());
    return buffer;
}

void InstrumentData::updatePercentage(QString &percentage)
{
    rawInput = rawInput.left(upDatePercentageIndex())+' '+ percentage;
}

QTime InstrumentData::toQTime()
{
    QTime output;
    QString buffer = this->rawTime();
    output =QTime::fromString(buffer, "HH':'MM");

    return(output);
}

QDate InstrumentData::toQDate()
{
    QDate output;
    QString buffer = this->rawDate();
    output = QDate::fromString( buffer,"MM'/'DD'/'YYYY");
    return(output);
}

QDateTime InstrumentData::toQDateTime()
{
    return(QDateTime(this->toQDate(),this->toQTime()));
}



