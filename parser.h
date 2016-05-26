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

#include "mainwindow.h"
#include "datapacket.h"

class Parser : public QMainWindow
{
public:
    Parser();
    Parser( const QString &df ) ;
    Parser( const QString &df, const QString &of);
    ~Parser();

private:
     QString dataFile;
     QString outputFile;
     QVector<QString> inputData;
     bool readInputData();
     bool writeOutputData();
     void eraseTempFile();
     QString cleanLine(QString& line);

    static const QString tFile(void){ return("temp.txt");};
     static qint64 maxLineLength(void){ return(35);};
     static qint64 minLineLength(void){ return(24);};
};

#endif // PARSER_H
