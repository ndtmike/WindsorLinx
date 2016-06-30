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
    Str.resize(3);

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
    Str.resize(3);

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

    bool ok;

    adczero = clean.mid(ADCZeroPos(),ADCZeroLength());
    adczero = adczero.toHex();  //convert to hex sring
    ADCZero = adczero.toInt(&ok,HexToDec()); //convert to value
    adcfull = clean.mid(ADCFullScalePos(), ADCFullScaleLength());
    adcfull = adcfull.toHex();
    ADCFullScale = adcfull.toInt(&ok,HexToDec());

    data0 = clean.mid(ADCData0Pos(), ADCData0Length());
    data0 = data0.toHex();
    ADC[0] = data0.toInt( &ok , HexToDec());
    Dist[0] = CalcDistance(ADC[0]);
    Str[0] = CalcStr(Dist[0]);

    data1 = clean.mid(ADCData1Pos(), ADCData1Length());
    data1 = data1.toHex();
    ADC[1] = data1.toInt( &ok, HexToDec() );
    Dist[1] = CalcDistance(ADC[1]);
    Str[1] = CalcStr(Dist[1]);

    data2 = clean.mid(ADCData2Pos(), ADCData2Length());
    data2 = data2.toHex();
    ADC[2] = data2.toInt( &ok, HexToDec());
    Dist[2] = CalcDistance(ADC[2]);
    Str[2] = CalcStr(Dist[2]);

    qDebug()<< Month<<'-'<<Day <<'-'<< Year <<'\n'
           << Hour <<':'<< Minute <<' '<< AmPm <<'\n'
           << "Power: " << StrPower << '\n'
           << "StrDensity: " << StrDensity << '\n'
           << "StrMoh: " << StrMoh << '\n'
           << "StrUnits: " << StrUnits << '\n'
           << "StrAggSize: " << StrAggSize << '\n'
           << "StrWeight: " << StrWeight << '\n'<< '\n'
           << "ADC Zero: "<< ADCZero << '\n'
           << "ADCFullScale: "<< ADCFullScale << '\n'
           << "ADC[0]: "<< ADC[0] << '\n'
           << "Dist[0]: "<< QString::number( Dist[0],'f',1)<<'\n'
           << "Str[0]: "<< QString::number( Str[0],'f',1)<<'\n'
           << "ADC[1]: "<< ADC[1] << '\n'
           << "Dist[1]: "<< QString::number( Dist[1],'f',1)<<'\n'
           << "Str[2]: "<< QString::number( Str[1],'f',1)<<'\n'
           << "ADC[2]: "<< ADC[2] << '\n'
           << "Dist[2]"<< QString::number( Dist[2],'f',1)<<'\n'
           << "Str[2]: "<< QString::number( Str[2],'f',1)<<'\n';                                                           ;
}

qint64 DataSet::avgDist(){

    qint64 a;

    a = (Dist[0]+Dist[1]+Dist[2])/3;

    return(a);
}

qint64 DataSet::avgPres(){

    qint64 a;

    a = (Str[0]+Str[1]+Str[2])/3;

    return(a);
}

double DataSet::CalcDistance( qint64 adc  ){
//doesn't quite match unit, rounding error?
    double distance, span, adcscale, adcdistance;

    span = (double)((ADCFullScale - ADCZero) - 1.0);
    adcscale = ADCScaleFactorMetric() / span;
    adcdistance = (double)(adc-ADCZero - 1.0);

    switch(PrivateUnits){
    case MPA:
            distance = ((adcdistance * adcscale) + DistanceOffsetMetric());
            break;
    case PSI:
            distance = (DistanceOffsetMetric() + (adcdistance * adcscale)) * InchConvFactor();
            break;
    }
/*
   qDebug()<< "adc: " << adc <<'\n'
          <<"ADCFullScale: "<< ADCFullScale << '\n'
          <<"ADCZero: "<< ADCZero <<'\n'
          <<"adcdistance"<< QString::number(adcdistance,'f',1) << '\n'
         <<"span: "<< QString::number( span, 'f', 1) << '\n'
         <<"adcscale: " << QString::number(adcscale,'f',1) << '\n'
         <<"distance: " << QString::number(distance,'f',1)<<'\n'<<'\n';
*/
    return(distance);
}

double DataSet::CalcStr( double d ){
    double             b;
    double             m;
    double       y;

    d = PrivateUnits == PSI ? d / InchConvFactor() : d;

    if (PrivatePower == High_Power)
    {
      y = HighPowerB() * exp( d * HighPowerM());
    }
    else
    {
      if (PrivatePower == Std_Power)
      {
        switch (PrivateMoh){
        case Value_3:
           b = StdB3();
           m = StdM3();
           break;
        case Value_4:
            b = StdB4();
            m = StdM4();
           break;
        case Value_5:
            b = StdB5();
            m = StdM5();
           break;
        case Value_6:
            b = StdB6();
            m = StdM6();
            break;
        case Value_7:
            b = StdB7();
            m = StdM7();
           break;
        }
      }
      if (PrivatePower == Low_Power)
      {
        switch (PrivateMoh){
        case Value_3:
           b = LowB3();
           m = LowM3();
           break;
        case Value_4:
            b = LowB4();
            m = LowM4();
           break;
        case Value_5:
            b = LowB5();
            m = LowM5();
           break;
        case Value_6:
            b = LowB6();
            m = LowM6();
            break;
        case Value_7:
            b = LowB7();
            m = LowM7();
           break;
        }
      }
      y = (m * d) - b;
    }
/*
    // Scale for low and super-low weights
    if (submenuWeight == SUBMENU_WEIGHT_LOW)
    {
      y *= 0.84;
    }
    else if (submenuWeight == SUBMENU_WEIGHT_SUPER_LOW)
    {
      y *= 0.66;
    }

    if (y < 0)
    {
      y = 0;
    }
*/
    y = PrivateUnits == PSI ? y * MPAToPSI() : y;
    return(y);
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
            PrivateMoh = Value_3;
            break;
        case Value_4:
            StrMoh = QMainWindow::tr("Moh Value 4");
            PrivateMoh = Value_4;
            break;
        case Value_5:
            StrMoh = QMainWindow::tr("Moh Value 5");
            PrivateMoh = Value_5;
            break;
        case Value_6:
            StrMoh = QMainWindow::tr("Moh Value 6");
            PrivateMoh = Value_6;
            break;
        case Value_7:
            StrMoh = QMainWindow::tr("Moh Value 7");
            PrivateMoh = Value_7;
            break;
    }
}

void DataSet::LoadStrPower(QByteArray &in){

    int value = (int)in[0];  //only one character string
    Power pwr = (Power)value;

    switch (pwr){
        case Std_Power:
            StrPower = QMainWindow::tr("Standard Power");
            PrivatePower = Std_Power;
            break;
        case Low_Power:
            StrPower = QMainWindow::tr("Low Power");
            PrivatePower = Low_Power;
            break;
        case High_Power:
            StrPower = QMainWindow::tr("High Performance");
            PrivatePower = High_Power;
            break;
    }
}

void DataSet::LoadStrUnits(QByteArray &in){

    int value = (int)in[0];     //only one character string
    Units units = (Units)value;

    switch (units){
        case MPA:
            StrUnits = QMainWindow::tr("MPA");
            PrivateUnits = MPA;
            break;
        case PSI:
            StrUnits = QMainWindow::tr("PSI");
            PrivateUnits = PSI;
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


