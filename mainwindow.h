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
** File Name: mainwindow.h
**
** Header file for mainwindow.h - windsorlinx software
**
** Author: Michael W. Hoag
** Copyright Michael W. Hoag 2016
** Email: mike@ndtjames.com
** -------------------------------------------------------------------------*/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtCore/QtGlobal>
#include <QIODevice>
#include <QMainWindow>
#include <QTextStream>
#include <QMessageBox>
#include <QFileDialog>
#include <QtSerialPort/QSerialPort>
#include <QSerialPortInfo>
#include <QTimer>
#include <QList>
#include <QProcess>

#include "console.h"
#include "splash.h"
#include "parser.h"

#define TEST_REG


QT_BEGIN_NAMESPACE

namespace Ui {
class MainWindow;
}

QT_END_NAMESPACE

class Console;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QString saveFileName;

protected:

private slots:
    void about();
    void cleanData();
    void closeSerialPort();
    void copy();
    void endUpload();
    void handleError(QSerialPort::SerialPortError error);
    void help();
    void openFile();
    void openSerialPort();
    void processSerialPort();
    void readData();
    bool saveAs();
    void showSplash();
    void save();

private:

    QTimer* connectTimer;
    QByteArray Data;
    bool DataUpload;
    Console *console;
    bool foundSerialPort;
    QSerialPort *serial;
    QTimer *serialTimeOut;
    Ui::MainWindow *ui;

    bool checkSerialPort();
    void initActionsConnections();
    QString resultsFormat( Parser &r, qint64 i);
    bool saveFile(const QString &fileName);

    static const QString helpString(void){ return("hh.exe aggralinx.chm");};
};
#endif // MAINWINDOW_H
