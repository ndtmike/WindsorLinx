/* ---------------------------------------------------------------------------
** This software is furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** File Name: parser.h
**
** Header file for parser.cpp - windsorlinx software
**
** Author: Michael W. Hoag
** Copyright Michael W. Hoag 2016
** Email: mike@ndtjames.com
** -------------------------------------------------------------------------*/

#ifndef PARSER_H
#define PARSER_H

#include <QString>
#include <QFile>
#include <QList>
#include <QTextStream>
#include <QMessageBox>
#include <QFileDialog>
#include <QDateTime>
#include <QVector>
#include <QtCore>
#include <QDebug>

#include "mainwindow.h"
#include "datapacket.h"
class DataSet
{
public:
    DataSet(); //constructor!!
    DataSet(const QByteArray &in);

    qint64 ADCZero;
    qint64 ADCFullScale;
    QString AmPm;
    qint64 Day;
    qint64 Hour;
    qint64 Minute;
    qint64 Month;
    QString StrAggSize;
    QString StrDensity;
    QString StrMoh;
    QString StrPower;
    QString StrUnits;
    QString StrWeight;
    qint64 Year;

    QVector <qint64> ADC;
    QVector <qint64> Dist;
    QVector <qint64> Pres;

    qint64 avgDist(void);
    qint64 avgPres(void);

private:
    enum AggSize{ Med_Agg = 13, Small_Agg, Large_Agg };
    enum Density{ Std_Density = 4, Light_Density };
    enum Moh{ Value_3 = 6, Value_4, Value_5, Value_6, Value_7 };
    enum Power{ Std_Power = 1, Low_Power, High_Power };
    enum Units{ MPA = 11, PSI};
    enum Weight{ High_Weight = 16, Med_Weight, Low_Weight, Very_Low_Weight};

    void LoadStrAggSize(QByteArray &in);
    void LoadDateTime(QByteArray &in);
    void LoadStrDensity(QByteArray &in);
    void LoadStrMoh(QByteArray &in);
    void LoadStrPower(QByteArray &in);
    void LoadStrUnits(QByteArray &in);
    void LoadStrWeight(QByteArray &in);
    QByteArray RemoveAscii(QByteArray &in);

    static int ADCZeroLength(void){return(1);}
    static int ADCZeroPos(void){return(11);}
    static int ADCFullScaleLength(void){return(1);}
    static int ADCFullScalePos(void){return(12);}
    static int ADCData0Length(void){return(1);}
    static int ADCData0Pos(void){return(13);}
    static int ADCData1Length(void){return(1);}
    static int ADCData1Pos(void){return(14);}
    static int ADCData2Length(void){return(1);}
    static int ADCData2Pos(void){return(15);}
    static int AggSizeLength(void){return(1);}
    static int AggSizePos(void){return(10);}
    static int DensityLength(void){return(1);}
    static int DensityPos(void){return(6);}
    static int MohLength(void){return(1);}
    static int MohPos(void){return(8);}
    static int PWRLength(void){return(1);}
    static int PWRPos(void){return(5);}
    static int TimeLength(void){return(5);}
    static int WeightLength(void){return(1);}
    static int WeightPos(void){return(7);}
    static int UnitsLength(void){return(1);}
    static int UnitsPos(void){return(9);}


};

class Parser:QWidget
{
public:
    Parser( QWidget* parent = 0 , const QByteArray &in = "");
    ~Parser();

private:
    QVector<DataSet> Data;

    static qint64 DataSetSize(void){return(16);}
};



#endif // PARSER_H
