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

    StrAggSize = QWidget::tr("Medium Aggregate Size");
    StrDensity = QWidget::tr("Standard Density Concrete");
    StrMoh = QWidget::tr("Moh Value 4");
    StrPower = QWidget::tr("Standard Power");
    StrUnits = QWidget::tr("MPA");
    StrWeight = QWidget::tr("High Weight Concrete");
}

DataSet::DataSet(const QByteArray &in){

    ADC.resize(TestSetSize());
    Dist.resize(TestSetSize());//must use re-size as can't declare size in declaration
    Str.resize(TestSetSize());

    QByteArray clean = in;
    QByteArray time, pwr, density, weight,
            moh, units, aggsize;
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
    LoadVectors( clean );
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

    double distance, span, adcscale, adcdistance;

    span = (double)((ADCFullScale - ADCZero));
    adcscale = ADCScaleFactorMetric() / span;
    adcscale = TruncateScale() * adcscale; //scale up for round function
    adcscale = floor (adcscale); //round off the value to match unit whch truncates number
    adcscale = adcscale/TruncateScale(); // scale it back down
    adcdistance = (double)(adc-ADCZero );
    distance = ((adcdistance * adcscale) + DistanceOffsetMetric());

    return(distance);
}

double DataSet::CalcStr( double d ){
    double             b;
    double             m;
    double       y;

    switch (PrivatePower){
    case High_Power:
        y = HighPowerB() * exp( d * HighPowerM());
        break;
    case Std_Power:
        switch (PrivateMoh){
        case Value_3:
           b = StdB3();
           m = StdM3();
           y = d > StdMin3()? (m * d) - b : 0;
           break;
        case Value_4:
            b = StdB4();
            m = StdM4();
            y = d > StdMin4()? (m * d) - b : 0;
           break;
        case Value_5:
            b = StdB5();
            m = StdM5();
            y = d > StdMin5()? (m * d) - b : 0;
           break;
        case Value_6:
            b = StdB6();
            m = StdM6();
            y = d > StdMin6()? (m * d) - b : 0;
            break;
        case Value_7:
            b = StdB7();
            m = StdM7();
            y = d > StdMin7()? (m * d) - b : 0;
           break;
        }
        if(PrivateWeight == Med_Weight){
            b = StdB3();
            m = StdM3();
            y = d > StdMin3()? (m * d) - b : 0;
        }
        break;
    case Low_Power:
        switch (PrivateMoh){
        case Value_3:
           b = LowB3();
           m = LowM3();
           y = d > LowMin3()? (m * d) - b : 0;
           break;
        case Value_4:
            b = LowB4();
            m = LowM4();
            y = d > LowMin3()? (m * d) - b : 0;
           break;
        case Value_5:
            b = LowB5();
            m = LowM5();
            y = d > LowMin3()? (m * d) - b : 0;
           break;
        case Value_6:
            b = LowB6();
            m = LowM6();
            y = d > LowMin3()? (m * d) - b : 0;
            break;
        case Value_7:
            b = LowB7();
            m = LowM7();
            y = d > LowMin3()? (m * d) - b : 0;
           break;
        }
        if(PrivateWeight == Med_Weight){
            b = LowB3();
            m = LowM3();
            y = d > LowMin3()? (m * d) - b : 0;
        }
        break;
    }
//    y = (m * d) - b;
    qDebug()<<"y: "<< y << '\t' <<"m: "<<m<<'\t'<<"d: "<<d<<'\t'<<"b: "<<b<<'\n';
    // Scale for low and super-low weights
    if (PrivateWeight == Low_Weight)
    {
      y *= 0.84;
    }
    else if (PrivateWeight == Very_Low_Weight)
    {
      y *= 0.66;
    }

    if (y < 0)
    {
      y = 0;
    }

    return(y);
}

qint64 DataSet::HexQByteArraytoInt(QByteArray &in){

    QByteArray data = in;
    bool ok;
    qint64 out;

    data = data.toHex(); //convert to hex string
    out = data.toInt( &ok, 16 );
    return(out);
}

void DataSet::LoadStrAggSize(QByteArray &in){

    int value = (int)in[0];     //only one character string
    AggSize aggsize = (AggSize)value;

    switch (aggsize){
        case Med_Agg:
            StrAggSize = QWidget::tr("Medium Aggregate Size");
            break;
        case Small_Agg:
            StrAggSize = QWidget::tr("Small Aggregate Size");
            break;
        case Large_Agg:
            StrAggSize = QWidget::tr("Large Aggreagate Size");
            break;
    }
}

void DataSet::LoadDateTime(QByteArray &in){

    qint64 month, day, year, hour, minute;

    month = (((in[3])>>4) & 0x01)*10;     //BCD for Month tens place
    month += (in[3]) & 0x0f;      //BCD for Month
    day = (((in[2])>>4) & 0x07)*10;     //BCD for Minutes tens place
    day += (in[2]) & 0x0f;      //BCD for Minutes
    year = (((in[4])>>4) & 0x0F)*10;     //BCD for Minutes tens place
    year += (in[4]) & 0x0f;      //BCD for Minutes
    year = year>50 ? year+1900 : year+2000;
    hour =(((in[1])>>4) & 0x01)* 10; // BCD for Hours tens place
    hour += (in[1])& 0x0f ;  //BCD for Hours
    hour = ((in[1]) & 0x20) > 0 ? hour + 12: hour;
    minute = (((in[0])>>4) & 0x07)*10;     //BCD for Minutes tens place
    minute += (in[0]) & 0x0f;      //BCD for Minutes

    QDate testdate(year, month, day) ;
    QTime testtime(hour, minute);
    QDateTime testdatetime( testdate,  testtime );
    TestDateTime = testdatetime;
}

void DataSet::LoadStrDensity(QByteArray &in){

    int value = (int)in[0];     //only one character string
    Density density = (Density)value;

    switch (density){
        case Std_Density:
            StrDensity = QWidget::tr("Standard Density Concrete");
            break;
        case Light_Density:
            StrDensity = QWidget::tr("Low Density Concrete");
            break;
    }
}

void DataSet::LoadStrMoh(QByteArray &in){

    int value = (int)in[0];     //only one character string
    Moh moh = (Moh)value;

    switch (moh){
        case Value_3:
            StrMoh = QWidget::tr("Moh Value 3");
            PrivateMoh = Value_3;
            break;
        case Value_4:
            StrMoh = QWidget::tr("Moh Value 4");
            PrivateMoh = Value_4;
            break;
        case Value_5:
            StrMoh = QWidget::tr("Moh Value 5");
            PrivateMoh = Value_5;
            break;
        case Value_6:
            StrMoh = QWidget::tr("Moh Value 6");
            PrivateMoh = Value_6;
            break;
        case Value_7:
            StrMoh = QWidget::tr("Moh Value 7");
            PrivateMoh = Value_7;
            break;
    }
}

void DataSet::LoadStrPower(QByteArray &in){

    int value = (int)in[0];  //only one character string
    Power pwr = (Power)value;

    switch (pwr){
        case Std_Power:
            StrPower = QWidget::tr("Standard Power");
            PrivatePower = Std_Power;
            break;
        case Low_Power:
            StrPower = QWidget::tr("Low Power");
            PrivatePower = Low_Power;
            break;
        case High_Power:
            StrPower = QWidget::tr("High Performance");
            PrivatePower = High_Power;
            break;
    }
}

void DataSet::LoadStrUnits(QByteArray &in){

    int value = (int)in[0];     //only one character string
    Units units = (Units)value;

    switch (units){
        case MPA:
            StrUnits = QWidget::tr("MPA");
            PrivateUnits = MPA;
            break;
        case PSI:
            StrUnits = QWidget::tr("PSI");
            PrivateUnits = PSI;
            break;
    }
}

void DataSet::LoadStrWeight(QByteArray &in){

    int value = (int)in[0];     //only one character string
    Weight weight = (Weight)value;

    switch (weight){
        case High_Weight:
            StrWeight = QWidget::tr("High Weight Concrete");
            PrivateWeight = High_Weight;
            break;
        case Med_Weight:
            StrWeight = QWidget::tr("Medium Weight");
            PrivateWeight = Med_Weight;
            break;
        case Low_Weight:
            StrWeight = QWidget::tr("Low Weight");
            PrivateWeight = Low_Weight;
            break;
        case Very_Low_Weight:
            StrWeight = QWidget::tr("Very Low Weight");
            PrivateWeight = Very_Low_Weight;
            break;
    }
}

void DataSet::LoadVectors( QByteArray &in ){

    QByteArray adczero, adcfull,
            data0, data1, data2;

    adczero = in.mid(ADCZeroPos(),ADCZeroLength());
    ADCZero = HexQByteArraytoInt( adczero);

    adcfull = in.mid(ADCFullScalePos(), ADCFullScaleLength());
    ADCFullScale = HexQByteArraytoInt(adcfull);

    data0 = in.mid(ADCData0Pos(), ADCData0Length());
    ADC[0] = HexQByteArraytoInt(data0);
    Dist[0] = PrivateUnits == MPA ? CalcDistance(ADC[0]) : CalcDistance(ADC[0]) / InchConvFactor();
    Str[0] =  PrivateUnits == MPA ? CalcStr(Dist[0]) : CalcStr(Dist[0] * InchConvFactor() ) * PSIConvFactor();
    qDebug()<< "Dist[0]: "<< Dist[0]<<'\t'<<"Str[0]" << Str[0] <<'\n';

    data1 = in.mid(ADCData1Pos(), ADCData1Length());
    ADC[1] = HexQByteArraytoInt(data1);
    Dist[1] = PrivateUnits == MPA ? CalcDistance(ADC[1]) : CalcDistance(ADC[1]) / InchConvFactor();
    Str[1] =  PrivateUnits == MPA ? CalcStr(Dist[1]) : CalcStr(Dist[1] * InchConvFactor() ) * PSIConvFactor();
    qDebug()<< "Dist[1]: "<< Dist[1]<<'\t'<<"Str[1]" << Str[1] <<'\n';

    data2 = in.mid(ADCData2Pos(), ADCData2Length());
    ADC[2] = HexQByteArraytoInt(data1);
    Dist[2] = PrivateUnits == MPA ? CalcDistance(ADC[2]) : CalcDistance(ADC[2]) / InchConvFactor();
    Str[2] =  PrivateUnits == MPA ? CalcStr(Dist[2]) : CalcStr(Dist[2]  * InchConvFactor() ) * PSIConvFactor();
    qDebug()<< "Dist[2]: "<< Dist[2]<<'\t'<<"Str[2]" << Str[2] <<'\n';
}

QByteArray DataSet::RemoveAscii(QByteArray &in){

    QByteArray out;

    for(qint16 i=0; i<in.size();++i){
        out[i]=in[i]-48;//removes ascii offset
    }
    return(out);
}


