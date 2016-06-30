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
#include <math.h>

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
    QVector <double> Dist;
    QVector <double> Str;

    qint64 avgDist(void);
    qint64 avgPres(void);

private:
    enum AggSize{ Med_Agg = 13, Small_Agg, Large_Agg };
    enum Density{ Std_Density = 4, Light_Density };
    enum Moh{ Value_3 = 6, Value_4, Value_5, Value_6, Value_7 };
    enum Power{ Std_Power = 1, Low_Power, High_Power };
    enum Units{ MPA = 11, PSI};
    enum Weight{ High_Weight = 16, Med_Weight, Low_Weight, Very_Low_Weight};

    Units PrivateUnits; // for calculations benefit
    Power PrivatePower; // for power calculations
    Moh PrivateMoh;
    void LoadStrAggSize(QByteArray &in);
    void LoadDateTime(QByteArray &in);
    void LoadStrDensity(QByteArray &in);
    void LoadStrMoh(QByteArray &in);
    void LoadStrPower(QByteArray &in);
    void LoadStrUnits(QByteArray &in);
    void LoadStrWeight(QByteArray &in);
    QByteArray RemoveAscii(QByteArray &in);

    double CalcDistance( qint64 adc );
    double CalcStr( double d);
    //for Constructor
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
    static int HexToDec(void){return(16);}
    //for CalcDistance
    static double ADCScaleFactorMetric(void){return(38.100);}
    static double InchConvFactor(void){ return(0.0393701);}
    static double DistanceOffsetMetric(void){ return( 25.400);}
    // for CalcPower
    static double HighPowerB(void){return(142.0);}
    static double HighPowerM(void){return(0.000760);}
    static double StdB3(void){return(4366.0);}
    static double StdB4(void){return(5093.0);}
    static double StdB5(void){return(6060.0);}
    static double StdB6(void){return(7311.0);}
    static double StdB7(void){return(8791.0);}
    static double LowB3(void){return(2372.0);}
    static double LowB4(void){return(2471.0);}
    static double LowB5(void){return(2958.0);}
    static double LowB6(void){return(3653.0);}
    static double LowB7(void){return(4331.0);}
    static double StdM3(void){return(1.810);}
    static double StdM4(void){return(1.888);}
    static double StdM5(void){return(2.000);}
    static double StdM6(void){return(2.172);}
    static double StdM7(void){return(2.375);}
    static double LowM3(void){return(0.9351);}
    static double LowM4(void){return(0.9321);}
    static double LowM5(void){return(0.9898);}
    static double LowM6(void){return(1.0860);}
    static double LowM7(void){return(1.1790);}
    static double MPAToPSI(void){return(145.0);}
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
