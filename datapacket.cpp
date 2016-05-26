/* ---------------------------------------------------------------------------
** This software is furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** File Name: DataPacket.cpp
**
** DataPacket - windsorlinx software
** Divides the DataPacket into reall data that can be used.
**
** Author: Michael W. Hoag
** Copyright Michael W. Hoag 2016
** Email: mike@ndtjames.com
** -------------------------------------------------------------------------*/


#include "datapacket.h"

DataPacket::DataPacket()
{
    QByteArray p = "";
}

DataPacket::DataPacket(QByteArray Packet)
{
    readDataPacket( Packet );

}

DataPacket::~DataPacket()
{

}

void DataPacket::readDataPacket(QByteArray& p)
{
    if( p != "" ){
        testnumber = readTestNumber( p );
        datatype = readDatatype( p );
        units = readUnits( p );
        discardvalue = readDiscardValue( p );
        error_flag = readReading( p );
        average = readAverage( p );
        median = readMedian( p );
        minimum = readMinimum( p );
        maximum = readMaximum( p );
        angle = readAngle( p );
        correlation = readCorrelation( p );
        time = readDateTime( p );
    }else{
        error_flag = false;
    }
}

QString DataPacket::readTestNumber( QByteArray& p )const
{
    QByteArray working;
    const int index = 1; //index postiion Test Number
    const int length = 3; // length of test number

    working = p.mid( index, length);
    return(working);
}

QString DataPacket::readDatatype( QByteArray& p )const
{
    QByteArray working;
    const int index = 4; //index postiion Discard Value
    const int length = 1; // length of Discard Value
    QString r = "";

    working = p.mid( index, length);
    r = (working == "R") ? "Rebound Number":"Concrete Strength";

    return(r);
}

QString DataPacket::readUnits( QByteArray& p )const
{
    QByteArray working;
    const QByteArray psi = "PSI";
    const QByteArray mpa = "MPA";
    const int index = 5; //index postiion Discard Value
    const int length = 1; // length of Discard Value

    working = p.mid( index, length);
    working = working == "P" ? psi : mpa;
    return(working);
}

QString DataPacket::readDiscardValue( QByteArray& p )const
{
    QByteArray working;
    const int index = 6; //index postiion Discard Value
    const int length = 2; // length of Discard Value

    working = p.mid( index, length);
    return(working);
}

QString DataPacket::readAverage( QByteArray& p )
{
    QByteArray working;
    int length = 0; // length of Average
    int index = 0; // index of Average

    if(datatype == "Rebound Number"){
        length = 2;
        index = 48;
    }else{
        length = 7;
        index = 148;
    }
    working = formatReadings( length, index, p );
    return(working);
}

QString DataPacket::readMedian( QByteArray& p )
{
    QByteArray working;
    int length = 0; // length of Median
    int index = 0; // index of Median

    if(datatype == "Rebound Number"){
        length = 2;
        index = 50;
    }else{
        length = 7;
        index = 155;
    }
    working = formatReadings( length, index, p );
    return(working);
}

QString DataPacket::readMinimum( QByteArray& p )
{
    QByteArray working;
    int length = 0; // length of Minimum
    int index = 0; // index of Minimum

    if(datatype == "Rebound Number"){
        length = 2;
        index = 52;
    }else{
        length = 7;
        index = 162;
    }
    working = formatReadings( length, index, p );
    return(working);
}

QString DataPacket::readMaximum( QByteArray& p )
{
    QByteArray working;
    int length = 0; // length of Maximum
    int index = 0; // index of Maximum

    if(datatype == "Rebound Number"){
        length = 2;
        index = 54;
    }else{
        length = 7;
        index = 169;
    }
    working = formatReadings( length, index, p );
    return(working);
}

QString DataPacket::readAngle( QByteArray& p )const
{
    QByteArray working;
    int length = 0; // length of Angle
    int index = 0; // index of Angle
    if(datatype == "Rebound Number"){
        length = 1;
        index = 57;
    }else{
        length = 1;
        index = 176;
    }
    working = p.mid( index, length);
    if(working == "C") working = "Ceiling";
    if(working == "F") working = "Floor";
    if(working == "W") working ="Wall";
    return(working);
}

QString DataPacket::readCorrelation( QByteArray& p )const
{
    QByteArray working;
    int length = 0;
    int index = 0;

    if(datatype == "Rebound Number"){
        length = 1;
        index = 58;
    }else{
        length = 1;
        index = 177;
    }
    working = p.mid( index, length);
    if(working == "U") working = "Cube";
    if(working == "Y") working = "Cylinder";
    return(working);
}

QDateTime DataPacket::readDateTime( QByteArray& p )const
{
    QDateTime r;
    QByteArray working;
    const QString formatin = "hhmmMMddyy";
    const int length = 10; // length of Discard Value
    int index = 0;

    if(datatype == "Rebound Number"){
        index = 58; //index postiion Discard Value

    }else{
        index = 178; //index postiion Discard Value
    }

    working = p.mid(index, length);
    r = QDateTime::fromString(working,formatin);
    return(r);
}

bool DataPacket::readReading( QByteArray& p )
{
    const int begin = 8; //datastarts here
    const int numreadings = 20; //number of readings
    const QByteArray emptystrength = "  xxxxx";
    const QByteArray emptystring = "  ";
    const QByteArray tab = "\t";
    QByteArray t;
    bool ok = true;

    int step = (datatype == "Rebound Number") ? 2 : 7; //rebound value or strength
    int end = begin + step * numreadings;

    for(int i = begin; i < end; i = i + step){
        t = p.mid(i,step);
        if((t != emptystrength && t !=emptystring)){
            t = formatReadings(step, i , p);
            reading <<t;
        }
    }
    return(ok);
}

QByteArray DataPacket::formatReadings(int &length, int &index, QByteArray w)
{
    QByteArray working;
    if(datatype == "Rebound Number"){
        working = w.mid( index, length);
        working.prepend(" R#: ");
    }else{
        working = w.mid( index, length);
        working.prepend(" R#: ");
        if(units == "PSI"){
            working.insert( 7, "\tPSI: ");
        }else{
            working.truncate(10);  //removes trailing xx used as filler
            working.insert( 7, "\tMPA: ");
        }
    }
    return(working);
}
