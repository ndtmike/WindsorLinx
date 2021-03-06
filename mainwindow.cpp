/****************************************************************************
**
** Copyright (C) 2012 Denis Shienkov <denis.shienkov@gmail.com>
** Copyright (C) 2012 Laszlo Papp <lpapp@kde.org>
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtSerialPort module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL21$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file. Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** As a special exception, The Qt Company gives you certain additional
** rights. These rights are described in The Qt Company LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** $QT_END_LICENSE$
**
****************************************************************************/
/* ---------------------------------------------------------------------------
** This software is furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** File Name: mainwindow.cpp
**
** mainwindow.cpp - windsorlinx software
**
** Author: Michael W. Hoag
** Copyright Michael W. Hoag 2016
** Email: mike@ndtjames.com
** -------------------------------------------------------------------------*/
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    console = new Console;
    setCentralWidget(console);
    serial = new QSerialPort(this);
    serialTimeOut = new QTimer(this);

    initActionsConnections();
    saveFileName = "";

    connect(serial, SIGNAL(error(QSerialPort::SerialPortError)), this,
            SLOT(handleError(QSerialPort::SerialPortError)));
    connect(serial, SIGNAL(readyRead()), this, SLOT(readData()));
    connect(serialTimeOut,SIGNAL(timeout()), this,SLOT(endUpload()));

    DataUpload = false;

#ifndef QT_DEBUG
    QTimer* init_timer = new QTimer(this);
    init_timer->singleShot(100, this, SLOT(showSplash()));
#else
#ifdef TEST_REG
    QTimer* init_timer = new QTimer(this);
    init_timer->singleShot(100, this, SLOT(showSplash()));
#else
    QTimer* init_timer = new QTimer(this);
    init_timer->singleShot(20, this, SLOT(loadExampleFile()));
#endif
#endif
    connectTimer = new QTimer(this);
}

MainWindow::~MainWindow()
{
    closeSerialPort();
    delete connectTimer;
    delete ui;
    delete console;
    delete serial;
}

void MainWindow::about()
{
    QString s;
    QTextStream toabout(&s);

    toabout << tr("The <b>Windsorlinx Software</b> is used with the <br>") <<
               tr("<b><i>James Instruments Inc.</i></b> Windsor Probe HP Z-WP-1000.<br><br>")<<
               tr("USA: +1773.4636565<br>")<<
               tr("Europe: +31.548.659032<br>")<<
               tr("Email: <a href=\"mailto:info@ndtjames.com?Subject=Windsorlinx\" target=\"_top\">info@ndtjames.com</a><br>")<<
               tr("Web: <a href=\"http://www.ndtjames.com\">http://www.ndtjames.com</a><br>")<<
               tr("Copyright 2016<br>")/*<<
               tr("Aggralinx is based in part on the work of the <a href=\"http://qwt.sf.net\"> Qwt project (http://qwt.sf.net)")*/;

    QMessageBox::information(this, tr("About Aggrameter"), s);
}

bool MainWindow::checkSerialPort()
{
    QString description;
    QString manufacturer;
    QString portname;

    CurrentLocale = QLocale::system(); //standardized number strings
    QString systemlocale = CurrentLocale.name();
    CurrentLocale = QLocale(systemlocale);
    QLocale::setDefault(CurrentLocale);
    Translator = new QTranslator(this);

    const QString portmanufacturer = "FTDI";
    const QString noport = tr("No Available Ports"
                              "\nCheck instrument is plugged in"
                              "\nor serial port installed properly"
                              "\n then restart Aggralinx");
    const QString messageTitle = "Check Serial Port";
    const QString connected = tr("Connected to ");
    QList <QSerialPortInfo> availablePorts;
    bool r = false;
    availablePorts = QSerialPortInfo::availablePorts();

    if(!availablePorts.isEmpty()){
        foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
        {
            portname = info.portName();
            description = info.description();
            manufacturer = info.manufacturer();
            if(manufacturer == portmanufacturer){
                serial->setPortName(portname);
                r = true;
            }
        if( r == true )break;
        }
    }
    if(r == false){
        QMessageBox::information(this,messageTitle,noport);
    }else{
        QMessageBox::information(this ,messageTitle , connected + portname );
    }
    return(r);
}



void MainWindow::cleanData()//main function that takes raw data and transforms to usable
{
    QString buffer;
    QTextStream display( &buffer );

    Parser p( this, Data );

    qint64 i = 0;
    for( std::vector<DataSet::Test>::iterator itr = p.Data->GetBeginItr();
         itr != p.Data->GetEndItr();++i,++itr){

        display << tr("Test Number: ")<< i+1 <<'\n'
                << p.ToQDateTime(itr).toString("MM/dd/yyyy hh:mm") <<'\n'
                << tr("Power: ") << p.ToQStrPower(itr) << '\t'
                << tr("Density: ") << p.ToQStrDensity(itr) << '\n'
                << tr("Moh: ") << p.ToQStrMoh( itr ) << '\t'
                << tr("Units: ") << p.ToQStrUnits( itr ) << '\n'
                << tr("Aggregate Size: ") << p.ToQStrAggsize( itr ) << '\n'
                << tr("Concrete Weight: ") << p.ToQStrWeight( itr ) << '\n' << '\n'
                << resultsFormat( p , itr );
    }
    console->setPlainText( buffer );
    DataUpload = true;
}

void MainWindow::copy()
{
    console->selectAll();
    console->copy();
}

void MainWindow::closeSerialPort()
{
    if (serial->isOpen())
        serial->close();
    console->setEnabled(false);
    ui->statusBar->showMessage(tr("Disconnected"));
}

void MainWindow::endUpload()
{
    serialTimeOut->stop();

    if(DataUpload == false){
        QMessageBox::information(this, "endUpload", tr("Upload Complete"));
#ifndef QT_NO_CURSOR
        QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
        cleanData();
#ifndef QT_NO_CURSOR
        QApplication::restoreOverrideCursor();
#endif
    }else{
        QMessageBox::information(this, "endUpload", tr("Restart Program Before Uploading More Data"));
    }

    ui->actionSaveAs->setEnabled(true);
    ui->action_Open->setEnabled(false);
    ui->actionPlot->setEnabled(true);
}

void MainWindow::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError) {
        QMessageBox::critical(this, tr("Critical Error"), serial->errorString());
        closeSerialPort();
    }
}

void MainWindow::help()
{
    QProcess* help = new QProcess(this);
    help->start("hh.exe Windsorlinx.chm");
}
/*
 * sets translator object to German
 */
void MainWindow::lngDeutche()
{
    CurrentLocale = QLocale(QLocale::German);
    QLocale::setDefault(CurrentLocale);
    LNGLoadTranslator();
}
/*
 * sets translator object to English
 */
void MainWindow::LNGEnglish()
{
    CurrentLocale = QLocale(QLocale::English);
    QLocale::setDefault(CurrentLocale);
    LNGLoadTranslator();
}
/*
 * sets translator object to Spanish
 */
void MainWindow::lngEspanol()
{
    CurrentLocale = QLocale(QLocale::Spanish);
    QLocale::setDefault(CurrentLocale);
    LNGLoadTranslator();
}
/*
 *loads translator  object
*/
void MainWindow::LNGLoadTranslator()
{
    if(Translator->isEmpty()){
            Translator->load(CurrentLocale.language(), "Internationalization","_");
            qApp->installTranslator(Translator);
    }else{
        qApp->removeTranslator(Translator);
        Translator->load(CurrentLocale.language(), "Internationalization","_");
        qApp->installTranslator(Translator);
    }
}
void MainWindow::loadExampleFile()
{
    saveFileName = "C:/Users/Mike/Documents/Projects/Windsorlinx/Example File/Example1.txt";
    QFile file(saveFileName);
    file.open(QFile::ReadOnly | QFile::Text);
    QTextStream load(&file);
    QString buffer = load.readAll();
    Data.append( buffer );
    cleanData();
    file.close();
}

void MainWindow::initActionsConnections()
{
    ui->actionQuit->setEnabled(true);
    ui->actionPlot->setEnabled(false);
    ui->actionSaveAs->setEnabled(false);
    ui->action_Open->setEnabled(true);

    connect(ui->actionQuit, SIGNAL(triggered()), this, SLOT(close()));
    connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(about()));
    connect(ui->actionSaveAs, SIGNAL(triggered()), this, SLOT(saveAs()));
    connect(ui->actionCopy, SIGNAL(triggered()), this, SLOT(copy()));
    connect(ui->actionHelp, SIGNAL(triggered()), this, SLOT(help()));
    connect(ui->action_Open, SIGNAL(triggered()), this, SLOT(openFile()));
    connect(ui->actionPlot, SIGNAL(triggered()), this, SLOT(cleanData()));
    connect(ui->actionDeutche, SIGNAL(triggered()), this, SLOT(lngDeutche()));
    connect(ui->actionEnglish, SIGNAL(triggered()), this, SLOT(LNGEnglish()));
    connect(ui->actionEspanol, SIGNAL(triggered()), this, SLOT(lngEspanol()));
}

void MainWindow::openFile()
{
    QString fileName = "";
    fileName = QFileDialog::getOpenFileName(this,
        tr("Open Text File"), "", tr("Text FIles (*.txt)"));

    if(fileName !=""){ //check if cancel pressed
        saveFileName = fileName;
        QFile file(saveFileName);
        file.open(QFile::ReadOnly | QFile::Text);
        QTextStream load(&file);
#ifndef QT_NO_CURSOR
        QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
        console->setPlainText(load.readAll());
#ifndef QT_NO_CURSOR
        QApplication::restoreOverrideCursor();
#endif
        file.close();
        ui->actionSaveAs->setEnabled(true);
        ui->action_Open->setEnabled(false);
        ui->actionPlot->setEnabled(true);
    }
}

void MainWindow::openSerialPort()
{
    serial->setBaudRate(9600);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);
    if (serial->open(QIODevice::ReadWrite
                     )) {
            console->setEnabled(true);
            console->setLocalEchoEnabled( true);
            ui->statusBar->showMessage(tr("Connected"));
    } else {
        QMessageBox::critical(this, tr("Error"), serial->errorString());

        ui->statusBar->showMessage(tr("Open error"));
    }
}

void MainWindow::processSerialPort()
{

    foundSerialPort = checkSerialPort();
    if(foundSerialPort){
        openSerialPort();
        connectTimer->start(500);
    }
}

void MainWindow::readData()
{
    connectTimer->stop();
    serialTimeOut->start(500);
    Data += serial->readAll();
    console->putData(serial->readAll());
}

QString MainWindow::resultsFormat( Parser &r,/*qint64*/ std::vector<DataSet::Test>::iterator itr ){
    QString buffer;
    QTextStream display( &buffer );

    std::vector<double> length = r.Data->TestLength( itr );
    std::vector<double> strength = r.Data->TestStrength( itr );

    if(itr->TestProp.PropUnits == DataSet::MPA){
        display << tr("Exposed Probe Length First Reading: ")<< QString::number( length[0],'f',1 )
                << tr( " mm" )<<'\n'
                << tr("Concrete Strength First Reading: ")<< QString::number( strength[0],'f',1 )
                << tr( " MPA" )<<'\n'
                << tr("Exposed Probe Length Second Reading: ")<< QString::number( length[1],'f',1 )
                << tr( " mm" )<<'\n'
                << tr("Concrete Strength Second Reading: ")<< QString::number( strength[1],'f',1 )
                << tr( " MPA" )<<'\n'
                << tr("Exposed Probe Length Third Reading: ")<< QString::number( length[2],'f',1 )
                << tr( " mm" )<<'\n'
                << tr("Concrete Strength Second Reading: ")<< QString::number( strength[2],'f',1 )
                << tr( " MPA" )<<'\n'<<'\n'
                << tr("Average Exposed Probe Length: ")<< QString::number( length[3], 'f', 1 )
                << tr( " mm" )<<'\n'
                << tr("Average Compressive Strength: ")<< QString::number( strength[3], 'f', 1 )
                << tr( " MPA" )<<'\n'<<'\n';
    }else{
        display << tr("Exposed Probe Length First Reading: ")<< QString::number( length[0],'f',2)
                << tr( " inches" )<<'\n'
                << tr("Concrete Strength First Reading: ")<< QString::number( strength[0],'f',0)
                <<tr(" PSI")<<'\n'
                << tr("Exposed Probe Length Second Reading: ")<< QString::number( length[1],'f',2)
                << tr( " inches" )<<'\n'
                << tr("Concrete Strength Second Reading: ")<< QString::number( strength[1],'f',0)
                << tr( " PSI" )<<'\n'
                << tr("Exposed Probe Length Third Reading: ")<< QString::number( length[2],'f',2)
                << tr( " inches" )<<'\n'
                << tr("Concrete Strength Second Reading: ")<< QString::number( strength[2],'f',0)
                << tr( " PSI" )<<'\n'<<'\n'
                << tr("Average Exposed Probe Length: ")<< QString::number( length[3],'f',2)
                << tr( " inches" )<<'\n'
                << tr("Average Compressive Strength: ")<< QString::number( strength[0],'f',0)
                << tr( " PSI" )<<'\n'<<'\n';
    }
    return (buffer);
}

/*void MainWindow::save()
{
    if(saveFileName != ""){
        saveFile(saveFileName);
        QMessageBox::information(this,"Aggralinx",tr("File: ")+
                                 '\n' + saveFileName +
                                 '\n' + tr("Saved Successfully"));
    }else{
        saveAs();
    }
}
*/
bool MainWindow::saveAs()
{
    QFileDialog dialog(this);
    dialog.setWindowModality(Qt::WindowModal);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setNameFilter(tr("Text (*.txt)"));
    QStringList files;
    if (dialog.exec())
        files = dialog.selectedFiles();
    else
        return false;
    saveFileName = files.at(0);
    return saveFile(files.at(0));
}

bool MainWindow::saveFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, "MainWindow",
                             tr("Cannot write file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return false;
    }
    QTextStream out(&file);
#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
    out << console->toPlainText();
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif
    return true;
}

void MainWindow::showSplash()
{
    const int five_sec = 5000;

    SplashDialog* splash = new SplashDialog();
    splash->setModal( true );
    splash->show();

    QTimer* splash_timer = new QTimer(this);
    splash_timer->singleShot(five_sec, this, SLOT(processSerialPort()));
}
