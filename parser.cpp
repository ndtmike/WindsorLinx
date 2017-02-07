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
//#include "datapacket.h"

Parser::Parser( QWidget*, const QByteArray &in ) //no parameter for QWidget Pointer it is not used
{
    QByteArray copy = in;
    Data = new DataSet;

    copy = copy.remove(0,1);  //remove number of sets
    for(qint64 i=0; copy.size() > 0 ; i+=DataSetSize()){
        QByteArray d = copy.left(DataSetSize());
        copy = copy.remove(0,(int)DataSetSize());
        DataSet::Test newtest = CreateTest(d);
        Data->AddTest(newtest);
    }
}

Parser::~Parser()
{
}

DataSet::Test Parser::CreateTest(QByteArray array)
{
    QByteArray clean = array;
    QByteArray qbatime, qbapwr, qbadensity, qbaweight,
            qbamoh, qbaunits, qbaaggsize;
    tm test_time;

    DataSet::Test return_test;

    clean = RemoveAscii(clean);//clean copy without the ASCII offset

    qbatime = clean.left(TimeLength()); //TimePos == 0
    test_time = QBAtoDateTime(qbatime);
    return_test.TestTime = test_time;

    qbapwr = clean.mid(PWRPos(),PWRLength());
    DataSet::Power power = QBAtoPower(qbapwr);
    qbadensity = clean.mid(DensityPos(),DensityLength());
    DataSet::Density density = QBAtoDensity(qbadensity);
    qbaweight = clean.mid(WeightPos(),WeightLength());
    DataSet::Weight weight = QBAtoWeight(qbaweight);
    qbamoh = clean.mid(MohPos(),MohLength());
    DataSet::Moh moh = QBAtoMoh(qbamoh);
    qbaunits = clean.mid(UnitsPos(),UnitsLength());
    DataSet::Units units = QBAtoUnits(qbaunits);
    qbaaggsize = clean.mid(AggSizePos(),AggSizeLength());
    DataSet::AggSize aggsize = QBAtoAggSize(qbaaggsize);

    DataSet::Prop return_prop;
    return_prop.PropAggSize = aggsize;
    return_prop.PropDensity = density;
    return_prop.PropMoh = moh;
    return_prop.PropPower = power;
    return_prop.PropUnits = units;
    return_prop.PropWeight = weight;
    return_test.TestProp = return_prop;

    return_test.ADC = QBAtoVectorADC( clean );

    return(return_test);
}

QByteArray Parser::RemoveAscii(QByteArray &in){

    QByteArray out;

    for(qint16 i=0; i<in.size();++i){
        out[i]=in[i]-48;//removes ascii offset
    }
    return(out);
}

qint64 Parser::HexQByteArraytoInt(QByteArray &in)
{
    QByteArray data = in;
    bool ok;
    qint64 out;

    data = data.toHex(); //convert to hex string
    out = data.toInt( &ok, 16 );
    return(out);
}

DataSet::AggSize Parser::QBAtoAggSize(QByteArray &in)
{
    int value = (int)in[0];     //only one character string
    DataSet::AggSize aggsize = (DataSet::AggSize)value;

    return(aggsize);
}

tm Parser::QBAtoDateTime(QByteArray &in)
{
    qint64 month, day, year, hour, minute;
    tm return_tm;

    month = (((in[3])>>4) & 0x01)*10;     //BCD for Month tens place
    month += (in[3]) & 0x0f;
    return_tm.tm_mon = month;
    day = (((in[2])>>4) & 0x07)*10;
    day += (in[2]) & 0x0f;      //BCD for Minutes
    return_tm.tm_mday;
    year = (((in[4])>>4) & 0x0F)*10;     //BCD for Minutes tens place
    year += (in[4]) & 0x0f;      //BCD for Minutes
    year = year>50 ? year+1900 : year+2000;
    return_tm.tm_year = year - 1900;
    hour =(((in[1])>>4) & 0x01)* 10; // BCD for Hours tens place
    hour += (in[1])& 0x0f ;  //BCD for Hours
    hour = ((in[1]) & 0x20) > 0 ? hour + 12: hour;
    return_tm.tm_hour = hour;
    minute = (((in[0])>>4) & 0x07)*10;     //BCD for Minutes tens place
    minute += (in[0]) & 0x0f;      //BCD for Minutes
    return_tm.tm_min = minute;

    return(return_tm);
}

DataSet::Density Parser::QBAtoDensity(QByteArray &in)
{
    int value = (int)in[0];     //only one character string
    DataSet::Density density = (DataSet::Density)value;

    return(density);
}

DataSet::Moh Parser::QBAtoMoh(QByteArray &in)
{
    int value = (int)in[0];     //only one character string
    DataSet::Moh moh = (DataSet::Moh)value;

    return(moh);
}

DataSet::Power Parser::QBAtoPower(QByteArray &in)
{
    int value = (int)in[0];  //only one character string
    DataSet::Power pwr = (DataSet::Power)value;

    return(pwr);
}

DataSet::Units Parser::QBAtoUnits(QByteArray &in)
{
    int value = (int)in[0];     //only one character string
    DataSet::Units units = (DataSet::Units)value;

    return(units);
}

std::vector<long> Parser::QBAtoVectorADC(QByteArray &in)
{
    QByteArray /*qba_adczero, qba_adcfull,
            qba_data0, qba_data1, qba_data2,*/
            qba_working;
    long adc_working;
    std::vector<long> return_vector;

    qba_working = in.mid(ADCZeroPos(),ADCZeroLength()); //adc zero
    adc_working = HexQByteArraytoInt( qba_working);
    return_vector.push_back(adc_working);

    qba_working = in.mid(ADCFullScalePos(), ADCFullScaleLength()); //adc full scale
    adc_working = HexQByteArraytoInt(qba_working);
    return_vector.push_back( adc_working );

    qba_working = in.mid(ADCData0Pos(), ADCData0Length());
    adc_working = HexQByteArraytoInt(qba_working);
    return_vector.push_back(adc_working);

    qba_working = in.mid(ADCData1Pos(), ADCData1Length());
    adc_working = HexQByteArraytoInt(qba_working);
    return_vector.push_back(adc_working);

    qba_working = in.mid(ADCData2Pos(), ADCData2Length());
    adc_working = HexQByteArraytoInt(qba_working);
    return_vector.push_back(adc_working);

    return(return_vector);
}

DataSet::Weight Parser::QBAtoWeight(QByteArray &in)
{
    int value = (int)in[0];     //only one character string
    DataSet::Weight weight = (DataSet::Weight)value;

    return(weight);
}

