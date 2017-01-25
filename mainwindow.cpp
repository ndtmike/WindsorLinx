#include <vector>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Inst_Data.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    DataSet::AggSize testagg = DataSet::Large_Agg;
    DataSet::Density testden = DataSet::Std_Density;
    DataSet::Moh testmoh = DataSet::Value_5;
    DataSet::Power testpower = DataSet::Std_Power;
    DataSet::Units testunits = DataSet::MPA;
    DataSet::Weight testweight = DataSet::Med_Weight;

    DataSet::Prop testprop = {testagg, testden, testmoh, testpower, testunits, testweight};
    std::vector<long> testadc = { 10, 2400, 1000, 1010, 1005 };

    DataSet::Test test = { testadc, testprop };

    DataSet dataset(test);
}

MainWindow::~MainWindow()
{
    delete ui;
}
