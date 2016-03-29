#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ConnectMenu();
    LoadDisplay();
}

MainWindow::~MainWindow()
{
    delete Display;
    delete ui;
}
void MainWindow::LoadDisplay()
{
    Display = new QTextEdit;
    setCentralWidget(Display);

}

void MainWindow::ConnectMenu()
{
    connect(ui->action_Open, SIGNAL(triggered()), this , SLOT(FileOpen()));
    connect(ui->action_Save, SIGNAL(triggered()), this , SLOT(FileSave()));
    connect(ui->action_Save_As, SIGNAL(triggered()), this , SLOT(FileSaveAs()));
    connect(ui->action_Quit, SIGNAL(triggered()), this , SLOT(FileQuit()));
    connect(ui->action_Copy, SIGNAL(triggered()), this , SLOT(EditCopy()));
    connect(ui->action_Connect, SIGNAL(triggered()), this , SLOT(ToolConnect()));
    connect(ui->action_Help, SIGNAL(triggered()), this , SLOT(HelpContents()));
    connect(ui->action_About, SIGNAL(triggered()), this , SLOT(HelpAbout()));
}

void MainWindow::FileOpen()
{
#ifdef MENU_TEST
    QMessageBox::information( this,"FileOpen", "File Open OK!");
#endif

    QString fileName = "";
    fileName = QFileDialog::getOpenFileName(this,
        tr("Open Text File"), "", tr("Text FIles (*.txt)"));
//    saveFileName = fileName;
    QFile file(fileName);
    QTextStream load(&file);
#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
    Display->setPlainText("");
    file.open(QFile::ReadOnly | QFile::Text);
    QTextStream ReadFile(&file);
    Display->setPlainText(load.readAll());
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif
    file.close();
}

void MainWindow::FileSave()
{
#ifdef MENU_TEST
    QMessageBox::information( this,"FileSave", "File Save OK!");
#endif
}

bool MainWindow::FileSaveAs()
{
#ifdef MENU_TEST
    QMessageBox::information( this,"FileSaveAs", "File Save As OK!");
#endif
    QFileDialog dialog(this);
    dialog.setWindowModality(Qt::WindowModal);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    QStringList files;
    if (dialog.exec())
        files = dialog.selectedFiles();
    else
        return false;

    return SaveFile(files.at(0));
}

void MainWindow::FileQuit()
{
#ifdef MENU_TEST
    QMessageBox::information( this,"FileQuit", "File Quit OK!");
#endif
}

void MainWindow::EditCopy()
{
#ifdef MENU_TEST
    QMessageBox::information( this,"EditCopy", "Edit Copy OK!");
#endif
}

void MainWindow::ToolConnect()
{
#ifdef MENU_TEST
    QMessageBox::information( this,"ToolConnect", "Tool Connect OK!");
#endif
}

void MainWindow::HelpContents()
{
#ifdef MENU_TEST
    QMessageBox::information( this,"HelpContents", "Help Contents OK!");
#endif
}

void MainWindow::HelpAbout()
{
#ifdef MENU_TEST
    QMessageBox::information( this,"Help About", "Help About OK!");
#endif
}

bool MainWindow::SaveFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Application"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return false;
    }

    QTextStream out(&file);
#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
    out << Display->toPlainText();
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif

//    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File saved"), 2000);
    return true;
}
