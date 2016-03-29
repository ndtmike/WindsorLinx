#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore>
#include <QtGlobal>
#include <QMessageBox>
#include <QtWidgets>
#include <QTextEdit>

#ifdef QT_DEBUG
    #define MENU_TEST
#endif

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void FileOpen();
    void FileSave();
    bool FileSaveAs();
    void FileQuit();
    void EditCopy();
    void ToolConnect();
    void HelpContents();
    void HelpAbout();

private:
    Ui::MainWindow *ui;

    void ConnectMenu();
    void LoadDisplay();
    bool SaveFile(const QString& fileName);
    QTextEdit* Display;
};

#endif // MAINWINDOW_H
