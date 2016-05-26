/* ---------------------------------------------------------------------------
** This software is furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** File Name: regression.h
** header file for regression analysis
**
** Author: Michael W. Hoag
** Email: mike@ndtjames.com
** Copyright Michael W. Hoag 2015
** -------------------------------------------------------------------------*/

//nice reference...http://onlinestatbook.com/2/regression/intro.html

#ifndef REGRESSION_H
#define REGRESSION_H

#include <QList>
#include <QVector>


typedef int treg_data_size;
typedef double treg_data_data;

struct exp_data{
    treg_data_data x;
    treg_data_data y;
};

class Regression
{
public:

    QList <exp_data> reg_data;

    Regression();
    ~Regression();
    void addData( exp_data data );

    treg_data_data num_data_points;
    treg_data_data stand_dev_x();
    treg_data_data stand_dev_y();
    treg_data_data mean_x();
    treg_data_data mean_y();
    treg_data_data r();
    treg_data_data rsqu();
    treg_data_data slope();
    treg_data_data offset();

private:

    bool bad_init_data;

    treg_data_data sumX();
    treg_data_data sumY();
    treg_data_data sumXY();
    treg_data_data sumXX();
    treg_data_data sumYY();
};

#endif // REGRESSION_H
