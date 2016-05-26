/* ---------------------------------------------------------------------------
** This software is furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** File Name: regression.cpp
** CPP file for regression analysis
**
** Author: Michael W. Hoag
** Email: mike@ndtjames.com
** Copyright Michael W. Hoag 2015
** -------------------------------------------------------------------------*/

//error checking is currently assumed to be from calling function.


#include <math.h>
#include "regression.h"

Regression::Regression()
{
    exp_data init_point;
    init_point.x = 0.0; //initializes reg_data
    init_point.y = 0.0;
    reg_data<<init_point;
    bad_init_data = true; //says first element just crap data
}

Regression::~Regression(){

}

void Regression::addData( exp_data data ){

    if( bad_init_data == false){ //check if class just initialized?
        reg_data.append( data ); //no great!
        num_data_points = reg_data.size();
    }else{
        reg_data[0] = data; //yes replace first element
        bad_init_data = false;
    }
}

treg_data_data Regression::sumX(){

    treg_data_data total_sum_x = 0.0;

    for (treg_data_size i = 0; i < reg_data.size(); ++i) {
        total_sum_x += reg_data[i].x;
    }
    return(total_sum_x);
}

treg_data_data Regression::sumY(){

    treg_data_data total_sum_y = 0.0;

    for (treg_data_size i = 0; i < reg_data.size(); ++i) {
        total_sum_y += reg_data[i].y;
    }
    return(total_sum_y);
}

treg_data_data Regression::sumXY(){

    treg_data_data total_sum_xy = 0.0;

    for (treg_data_size i = 0; i < reg_data.size(); ++i) {
        total_sum_xy += ((reg_data[i].y) * (reg_data[i].x));
    }
    return(total_sum_xy);
}

treg_data_data Regression::sumXX(){

    treg_data_data total_sum_xx = 0.0;

    for (treg_data_size i = 0; i < reg_data.size(); ++i) {
        total_sum_xx += ((reg_data[i].x) * (reg_data[i].x));
    }
    return(total_sum_xx);
}

treg_data_data Regression::sumYY(){

    treg_data_data total_sum_yy = 0.0;

    for (treg_data_size i = 0; i < reg_data.size(); ++i) {
        total_sum_yy += ((reg_data[i].y) * (reg_data[i].y));
    }
    return(total_sum_yy);
}

treg_data_data Regression::mean_x(){

    treg_data_data mean = 0.0;

    mean = sumX() / num_data_points;

    return( mean );
}

treg_data_data Regression::mean_y(){

    treg_data_data mean = 0.0;

    mean = sumY() / num_data_points;

    return( mean );
}

treg_data_data Regression::stand_dev_x(){ // Calculates the standard deviation of all x values
//http://onlinestatbook.com/2/summarizing_distributions/variability.html


    treg_data_data stand_dev = 0.0;
    treg_data_data variance = 0.0;
    treg_data_data sumxsq = 0.0;
    treg_data_data sumx_sqn = 0.0;

    sumxsq = sumXX();
    sumx_sqn = pow( sumX(), 2) / num_data_points;

    variance = (sumxsq-sumx_sqn)/( num_data_points - 1 );

    stand_dev = sqrt(variance);

    return(stand_dev);
}

treg_data_data Regression::stand_dev_y(){ // Calculates the standard deviation of all y values
//http://onlinestatbook.com/2/summarizing_distributions/variability.html
    treg_data_data stand_dev = 0.0;
    treg_data_data variance = 0.0;
    treg_data_data sumysq = 0.0;
    treg_data_data sumy_sqn = 0.0;

    sumysq = sumYY();
    sumy_sqn = pow( sumY(), 2) / num_data_points ;

    variance = (sumysq-sumy_sqn)/( num_data_points - 1 );

    stand_dev = sqrt(variance);

    return(stand_dev);
}

treg_data_data Regression::r(){
// http://onlinestatbook.com/2/regression/intro.html
    treg_data_data correlation = 0.0;
    treg_data_data numerator = 0.0;
    treg_data_data denominator = 0.0;
    treg_data_data sumXSQ = 0.0;
    treg_data_data sumYSQ = 0.0;

    for(treg_data_size i = 0; i < reg_data.size(); ++i){
        numerator += ( reg_data[i].x - mean_x()) *
                     ( reg_data[i].y - mean_y());
        sumXSQ += pow( reg_data[i].x - mean_x(), 2);
        sumYSQ += pow( reg_data[i].y - mean_y(), 2);
    }
    denominator = sqrt( sumYSQ * sumXSQ );
    correlation = numerator / denominator;

    return( correlation );
}

treg_data_data Regression::rsqu(){
//
    treg_data_data correlation = 0.0;
    treg_data_data numerator = 0.0;
    treg_data_data denominator = 0.0;
    treg_data_data sumXSQ = 0.0;
    treg_data_data sumYSQ = 0.0;

    for(treg_data_size i = 0; i < reg_data.size(); ++i){
        numerator += ( reg_data[i].x - mean_x()) *
                     ( reg_data[i].y - mean_y());
        sumXSQ += pow( reg_data[i].x - mean_x(), 2);
        sumYSQ += pow( reg_data[i].y - mean_y(), 2);
    }
    denominator = sqrt( sumYSQ * sumXSQ );
    correlation = numerator / denominator;

    return( correlation * correlation );
}

treg_data_data Regression::slope(){

    treg_data_data m = 0.0;

    m = r()*stand_dev_y()/stand_dev_x();

    return(m);
}

treg_data_data Regression::offset(){

    treg_data_data b = 0.0;

    b = mean_y() - slope()* mean_x();

    return(b);
}
