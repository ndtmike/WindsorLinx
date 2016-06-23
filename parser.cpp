/* ---------------------------------------------------------------------------
** This software is furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** File Name: parser.cpp
**
** parser.cpp - windsorlinx software
** Divides the input stream up into data packets
**
** Author: Michael W. Hoag
** Copyright Michael W. Hoag 2016
** Email: mike@ndtjames.com
** -------------------------------------------------------------------------*/

#include "parser.h"
#include "datapacket.h"

Parser::Parser( QWidget*, const QByteArray &in ) //no parameter for QWidget Pointer it is not used
{
    QByteArray copy = in;

    qDebug()<<"Number of Sets: " << copy[0] -48 ;
    copy = copy.remove(0,1);  //remove number of sets
    for(qint64 i=0; copy.size() > 0 ; i+=DataSetSize()){
        QByteArray d = copy.left(DataSetSize());
        copy = copy.remove(0,(int)DataSetSize());
        DataSet newdatapoint(d);
        Data.append(newdatapoint);
    }
}

Parser::~Parser()
{
}

DataSet::DataSet(void){

    ADC.resize(3);
    Dist.resize(3);//must use re-size as can't declare size in declaration
    Pres.resize(3);

    ADCZero = 0;
    ADCFullScale = 512;
    AmPm = "AM";
    Day = 0;
    Month = 0;
    Year = 0;
    Hour = 0;
    Minute = 0;
    StrAggSize = QMainWindow::tr("Medium Aggregate Size");
    StrDensity = QMainWindow::tr("Standard Density Concrete");
    StrMoh = QMainWindow::tr("Moh Value 4");
    StrPower = QMainWindow::tr("Standard Power");
    StrUnits = QMainWindow::tr("MPA");
    StrWeight = QMainWindow::tr("High Weight Concrete");
}

DataSet::DataSet(const QByteArray &in){

    ADC.resize(3);
    Dist.resize(3);//must use re-size as can't declare size in declaration
    Pres.resize(3);

    QByteArray clean = in;
    QByteArray time, pwr, density, weight,
            moh, units, aggsize, adczero, adcfull,
            data0, data1, data2;
    clean = RemoveAscii(clean);//clean copy without the ASCII offset

    time = clean.left(TimeLength()); //TimePos == 0
    LoadDateTime(time);
    pwr = clean.mid(PWRPos(),PWRLength());
    LoadStrPower(pwr);
    density = clean.mid(DensityPos(),DensityLength());
    LoadStrDensity(density);
    weight = clean.mid(WeightPos(),WeightLength());
    LoadStrWeight(weight);
    moh = clean.mid(MohPos(),MohLength());
    LoadStrMoh(moh);
    units = clean.mid(UnitsPos(),UnitsLength());
    LoadStrUnits(units);
    aggsize = clean.mid(AggSizePos(),AggSizeLength());
    LoadStrAggSize(aggsize);

    adczero = clean.mid(ADCZeroPos(),ADCZeroLength());
    ADCZero = (int)adczero[0];     //only one character string
    adcfull = clean.mid(ADCFullScalePos(), ADCFullScaleLength());
    ADCFullScale = (int)adcfull[0];
    data0 = clean.mid(ADCData0Pos(), ADCData0Length());
    ADC[0] = (int)data0[0];
    data1 = clean.mid(ADCData1Pos(), ADCData1Length());
    ADC[1] = (int)data0[1];
    data2 = clean.mid(ADCData2Pos(), ADCData2Length());
    ADC[2] = (int)data0[2];
    qDebug()<<Month<<'-'<<Day <<'-'<< Year <<'\n'
           << Hour <<':'<< Minute <<' '<< AmPm <<'\n'
           << "Power: " << StrPower << '\n'
           << "StrDensity: " << StrDensity << '\n'
           << "StrMoh: " << StrMoh << '\n'
           << "StrUnits: " << StrUnits << '\n'
           << "StrAggSize: " << StrAggSize << '\n'
           << "StrWeight: " << StrWeight << '\n'
           << "ADC Zero: "<< ADCZero << '\n'
           << "ADCFullScale: "<< ADCFullScale << '\n'
           << "ADC[0]: "<< ADC[0] << '\n'
           << "ADC[1]: "<< ADC[1] << '\n'
           << "ADC[2]: "<< ADC[2] << '\n';
}

qint64 DataSet::avgDist(){

    qint64 a;

    a = (Dist[0]+Dist[1]+Dist[2])/3;

    return(a);
}

qint64 DataSet::avgPres(){

    qint64 a;

    a = (Pres[0]+Pres[1]+Pres[2])/3;

    return(a);
}

void DataSet::LoadStrAggSize(QByteArray &in){

    int value = (int)in[0];     //only one character string
    AggSize aggsize = (AggSize)value;

    switch (aggsize){
        case Med_Agg:
            StrAggSize = QMainWindow::tr("Medium Aggregate Size");
            break;
        case Small_Agg:
            StrAggSize = QMainWindow::tr("Small Aggregate Size");
            break;
        case Large_Agg:
            StrAggSize = QMainWindow::tr("Large Aggreagate Size");
            break;
    }
}

void DataSet::LoadDateTime(QByteArray &in){
    qDebug()<< "in loadDateTime:" << in;

    Month = (((in[3])>>4) & 0x01)*10;     //BCD for Month tens place
    Month += (in[3]) & 0x0f;      //BCD for Month
    Day = (((in[2])>>4) & 0x07)*10;     //BCD for Minutes tens place
    Day += (in[2]) & 0x0f;      //BCD for Minutes
    Year = (((in[4])>>4) & 0x0F)*10;     //BCD for Minutes tens place
    Year += (in[4]) & 0x0f;      //BCD for Minutes
    Hour =(((in[1])>>4) & 0x01)* 10; // BCD for Hours tens place
    Hour += (in[1])& 0x0f ;  //BCD for Hours
    Minute = (((in[0])>>4) & 0x07)*10;     //BCD for Minutes tens place
    Minute += (in[0]) & 0x0f;      //BCD for Minutes
    AmPm = ((in[1]) & 0x20) > 0 ? "PM":"AM";
}
void DataSet::LoadStrDensity(QByteArray &in){

    int value = (int)in[0];     //only one character string
    Density density = (Density)value;

    switch (density){
        case Std_Density:
            StrDensity = QMainWindow::tr("Standard Density Concrete");
            break;
        case Light_Density:
            StrDensity = QMainWindow::tr("Low Density Concrete");
            break;
    }
}

void DataSet::LoadStrMoh(QByteArray &in){

    int value = (int)in[0];     //only one character string
    Moh moh = (Moh)value;

    switch (moh){
        case Value_3:
            StrMoh = QMainWindow::tr("Moh Value 3");
            break;
        case Value_4:
            StrMoh = QMainWindow::tr("Moh Value 4");
            break;
        case Value_5:
            StrMoh = QMainWindow::tr("Moh Value 5");
            break;
        case Value_6:
            StrMoh = QMainWindow::tr("Moh Value 6");
            break;
        case Value_7:
            StrMoh = QMainWindow::tr("Moh Value 7");
            break;
    }
}

void DataSet::LoadStrPower(QByteArray &in){

    int value = (int)in[0];  //only one character string
    Power pwr = (Power)value;

    switch (pwr){
        case Std_Power:
            StrPower = QMainWindow::tr("Standard Power");
            break;
        case Low_Power:
            StrPower = QMainWindow::tr("Low Power");
            break;
        case High_Power:
            StrPower = QMainWindow::tr("High Performance");
            break;
    }
}

void DataSet::LoadStrUnits(QByteArray &in){

    int value = (int)in[0];     //only one character string
    Units units = (Units)value;

    switch (units){
        case MPA:
            StrUnits = QMainWindow::tr("MPA");
            break;
        case PSI:
            StrUnits = QMainWindow::tr("PSI");
            break;
    }
}
void DataSet::LoadStrWeight(QByteArray &in){

    int value = (int)in[0];     //only one character string
    Weight weight = (Weight)value;

    switch (weight){
        case High_Weight:
            StrWeight = QMainWindow::tr("High Weight Concrete");
            break;
        case Med_Weight:
            StrWeight = QMainWindow::tr("Medium Weight");
            break;
        case Low_Weight:
            StrWeight = QMainWindow::tr("Low Weight");
            break;
        case Very_Low_Weight:
            StrWeight = QMainWindow::tr("Very Low Weight");
            break;
    }
}

QByteArray DataSet::RemoveAscii(QByteArray &in){

    QByteArray out;

    for(qint16 i=0; i<in.size();++i){
        out[i]=in[i]-48;//removes ascii offset
    }
    return(out);
}


