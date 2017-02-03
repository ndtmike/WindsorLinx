#include <vector>

#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

//first test dataset
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

//second test dataset
    testagg = DataSet::Small_Agg;
    testden = DataSet::Std_Density;
    testmoh = DataSet::Value_3;
    testpower = DataSet::High_Power;
    testunits = DataSet::PSI;
    testweight = DataSet::High_Weight;

    testprop = {testagg, testden, testmoh, testpower, testunits, testweight};
    testadc = { 10, 2400, 900, 1050, 1002 };

    test = { testadc, testprop };

    dataset.AddTest( test );

    DisplayData( dataset );
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::DisplayData( DataSet data )
{
    QString output_text = "";
    QTextStream output_stream( &output_text );
    for( auto itr = data.GetBeginItr(); itr !=data.GetEndItr(); ++itr ){

        std::vector<double> length = data.TestLength( itr );
        std::vector<double> strength =data.TestStrength( itr );

        output_stream<<"Aggregate Size: "<< toQStrAggsize(data, itr)<< '\n'
                  <<"Density: "<< toQStrDensity( data, itr)<< '\n'
                  <<"Agregate Hardness: "<< toQStrMoh( data, itr) <<'\n'
                  <<"Power: "<< toQStrPower( data, itr) <<'\n'
                  <<"Units: "<< toQStrUnits( data, itr)<<'\n'
                  <<"Weight: "<< toQStrWeight( data, itr)<<'\n'
                  <<"Length: "<< '\n';
        output_stream.setRealNumberNotation(QTextStream::FixedNotation);
        output_stream.setRealNumberPrecision( 0 );
        for( auto i = length.begin(); i!= length.end(); ++i ){
            double out = *i;

            DataSet::Test current_test = data.GetTest( itr );
            if(current_test.TestProp.PropUnits == DataSet::MPA){
                output_stream.setRealNumberPrecision( 1 );
                output_stream <<'\t'<< out << " mm" <<'\n';
            }else{
                output_stream.setRealNumberPrecision( 3 );
                output_stream <<'\t'<< out << " inches" <<'\n';
            }
        }

        output_stream << "Strength: "<< '\n';
        for( auto i = strength.begin(); i!= strength.end(); ++i ){

            DataSet::Test current_test = data.GetTest( itr );
            if(current_test.TestProp.PropUnits == DataSet::MPA){
                output_stream.setRealNumberPrecision( 1 );
                double out = *i;
                output_stream <<'\t'<< out <<" MPA" << '\n';
            }else{
                output_stream.setRealNumberPrecision( 0 );
                double out = *i;
                output_stream <<'\t'<< out << " PSI" <<'\n';
            }
        }
        output_stream <<'\n';
    }

    ui->Code_Test->setText( output_text );
}

QString MainWindow::toQStrAggsize(DataSet dataset, std::vector<DataSet::Test>::iterator itr_test)
{
    QString return_string = " ";
    DataSet::Test current_test = dataset.GetTest( itr_test);

    switch (current_test.TestProp.PropAggSize){
        case DataSet::Med_Agg:
            return_string = "Medium Aggregate";
            break;
        case DataSet::Small_Agg:
            return_string = "Small Aggregate";
            break;
        case DataSet::Large_Agg:
            return_string = "Large Aggregate";
            break;
    }

    return(return_string);
}

QString MainWindow::toQStrDensity(DataSet dataset, std::vector<DataSet::Test>::iterator itr_test)
{
    QString return_string = " ";
    DataSet::Test current_test = dataset.GetTest( itr_test);

    switch (current_test.TestProp.PropDensity){
        case DataSet::Std_Density:
            return_string = "Standard Density";
            break;
        case DataSet::Light_Density:
            return_string = "Light Density";
            break;
    }

    return(return_string);
}

QString MainWindow::toQStrMoh(DataSet dataset, std::vector<DataSet::Test>::iterator itr_test)
{
    QString return_string = " ";
    DataSet::Test current_test = dataset.GetTest( itr_test);

    switch (current_test.TestProp.PropMoh){
        case DataSet::Value_3:
            return_string = "Moh 3";
            break;
        case DataSet::Value_4:
            return_string = "Moh 4";
            break;
        case DataSet::Value_5:
            return_string = "Moh 5";
            break;
        case DataSet::Value_6:
            return_string = "Moh 6";
            break;
        case DataSet::Value_7:
            return_string = "Moh 7";
            break;
    }
    return(return_string);
}

QString MainWindow::toQStrPower(DataSet dataset, std::vector<DataSet::Test>::iterator itr_test)
{
    QString return_string = " ";
    DataSet::Test current_test = dataset.GetTest( itr_test);

    switch (current_test.TestProp.PropPower){
        case DataSet::Std_Power:
            return_string = "Standard Power";
            break;
        case DataSet::Low_Power:
            return_string = "Low Power";
            break;
        case DataSet::High_Power:
            return_string = "High Performance";
            break;
    }
    return(return_string);
}

QString MainWindow::toQStrUnits(DataSet dataset, std::vector<DataSet::Test>::iterator itr_test)
{
    QString return_string = " ";
    DataSet::Test current_test = dataset.GetTest( itr_test);

    switch (current_test.TestProp.PropUnits){
        case DataSet::MPA:
            return_string = "MPA";
            break;
        case DataSet::PSI:
            return_string = "PSI";
            break;
    }
    return(return_string);
}

QString MainWindow::toQStrWeight(DataSet dataset, std::vector<DataSet::Test>::iterator itr_test)
{
    QString return_string = " ";
    DataSet::Test current_test = dataset.GetTest( itr_test);

    switch (current_test.TestProp.PropWeight){
        case DataSet::High_Weight:
            return_string = "High Density";
            break;
        case DataSet::Med_Weight:
            return_string = "Medium Density";
            break;
        case DataSet::Low_Weight:
            return_string = "Low Density";
            break;
        case DataSet::Very_Low_Weight:
            return_string = "Very Low Density";
            break;
    }

    return(return_string);
}
