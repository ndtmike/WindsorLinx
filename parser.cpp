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
    QByteArray time, qbapwr, qbadensity, qbaweight,
            qbamoh, qbaunits, qbaaggsize;
    clean = RemoveAscii(clean);//clean copy without the ASCII offset

//    time = clean.left(TimeLength()); //TimePos == 0
//    LoadDateTime(time);
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
    //    LoadVectors( clean );
}

QByteArray Parser::RemoveAscii(QByteArray &in){

    QByteArray out;

    for(qint16 i=0; i<in.size();++i){
        out[i]=in[i]-48;//removes ascii offset
    }
    return(out);
}

DataSet::AggSize Parser::QBAtoAggSize(QByteArray &in)
{
    int value = (int)in[0];     //only one character string
    DataSet::AggSize aggsize = (DataSet::AggSize)value;

    return(aggsize);
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

DataSet::Weight Parser::QBAtoWeight(QByteArray &in)
{
    int value = (int)in[0];     //only one character string
    DataSet::Weight weight = (DataSet::Weight)value;

    return(weight);
}

