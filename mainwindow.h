#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextStream>
#include "Inst_Data.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    void DisplayData(DataSet data);

    QString toQStrAggsize(DataSet dataset, std::vector<DataSet::Test>::iterator itr_test);
    QString toQStrDensity(DataSet dataset, std::vector<DataSet::Test>::iterator itr_test);
    QString toQStrMoh(DataSet dataset, std::vector<DataSet::Test>::iterator itr_test);
    QString toQStrPower(DataSet dataset, std::vector<DataSet::Test>::iterator itr_test);
    QString toQStrUnits(DataSet dataset, std::vector<DataSet::Test>::iterator itr_test);
    QString toQStrWeight(DataSet dataset, std::vector<DataSet::Test>::iterator itr_test);
};

#endif // MAINWINDOW_H
