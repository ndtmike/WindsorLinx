 /* ---------------------------------------------------------------------------
** This software is furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** File Name: Inst_Data.h
**
** Header file for parser.cpp - windsorlinx software
**
** Author: Michael W. Hoag
** Copyright Michael W. Hoag 2017
** Email: mike@ndtjames.com
** -------------------------------------------------------------------------*/

#ifndef INST_DATA_H
#define INST_DATA_H

#include <math.h>
#include <vector>
#include <string>
#include <ctime>
#include <algorithm>
#include <functional>

/* ---------------------------------------------------------------------------
** Holds the data for each test as well as each tests properties in a vector.
** The class does not 'code/decode' the data to strings that is assumed to happen
** in the QT interface. Thus allowing ease in translation.
** -------------------------------------------------------------------------*/

class DataSet
{
public:
    enum AggSize{ Med_Agg = 13, Small_Agg, Large_Agg };
    enum Density{ Std_Density = 4, Light_Density };
    enum Moh{ Value_3 = 6, Value_4, Value_5, Value_6, Value_7 };
    enum Power{ Std_Power = 1, Low_Power, High_Power };
    enum Units{ MPA = 11, PSI};
    enum Weight{ High_Weight = 16, Med_Weight, Low_Weight, Very_Low_Weight};

    struct Prop{ //material property parameters for Windsor Probe
        AggSize PropAggSize;
        Density PropDensity;
        Moh PropMoh;
        Power PropPower;
        Units PropUnits;
        Weight PropWeight;
    };

    struct Test{ //test for the Windsor Probe raw data
        std::vector<long> ADC;
        Prop TestProp;
        tm TestTime;
    };

    DataSet();
    DataSet(Test Init_Test); //constructor!!

    void AddTest(Test test);
    Test GetTest(std::vector<Test>::iterator itr_current);
    std::vector<Test>::iterator GetBeginItr();
    std::vector<Test>::iterator GetEndItr();

    std::vector<double> TestLength(std::vector<Test>::iterator current_test);
    std::vector<double> TestStrength(std::vector<Test>::iterator current_test);

private:

    std::vector <Test> TestData;

    //return vector constants for length and strenght
    static int LocADCFullScale(void){return(1);}
    static int LocADCZero(void){return(0);}
    static int LocAVG(void){return(3);}
    static int NumTests(void){return(3);}
    static int Vector_Size(void){return(4);}

    //for Constructor
    static int ADCZeroLength(void){return(1);}
    static int ADCZeroPos(void){return(11);}
    static int ADCFullScaleLength(void){return(1);}
    static int ADCFullScalePos(void){return(12);}
    static int ADCData0Length(void){return(1);}
    static int ADCData0Pos(void){return(13);}
    static int ADCData1Length(void){return(1);}
    static int ADCData1Pos(void){return(14);}
    static int ADCData2Length(void){return(1);}
    static int ADCData2Pos(void){return(15);}
    static int AggSizeLength(void){return(1);}
    static int AggSizePos(void){return(10);}
    static int DensityLength(void){return(1);}
    static int DensityPos(void){return(6);}
    static int HexToDec(void){return(16);}
    static int MohLength(void){return(1);}
    static int MohPos(void){return(8);}
    static int PWRLength(void){return(1);}
    static int PWRPos(void){return(5);}
    static int TimeLength(void){return(5);}
    static int WeightLength(void){return(1);}
    static int WeightPos(void){return(7);}
    static int UnitsLength(void){return(1);}
    static int UnitsPos(void){return(9);}

    //for TestLength
    static double ADCScaleFactorMetric(void){return(38.100);}
    static double ADCScaleFactorStandard(void){return(1.5);}
    static double DistanceOffsetMetric(void){ return(25.400);}
    static double DistanceOffsetStandard(void){ return(1.0);}
    static double InchConvFactor(void){ return(25.4);}
    static double PSIConvFactor(void){ return(145.0);}

    // for TestStrength
    static double HighPowerB(void){return(2.8);}
    static double HighPowerM(void){return(0.0602);}
    static double StdMin3(void){return(28.0);}
    static double StdMin4(void){return(29.0);}
    static double StdMin5(void){return(33.0);}
    static double StdMin6(void){return(36.0);}
    static double StdMin7(void){return(39.0);}
    static double LowMin3(void){return(28.0);}
    static double LowMin4(void){return(29.0);}
    static double LowMin5(void){return(33.0);}
    static double LowMin6(void){return(36.0);}
    static double LowMin7(void){return(39.0);}
    static double StdB3(void){return(43.680);}
    static double StdB4(void){return(51.74);}
    static double StdB5(void){return(59.79);}
    static double StdB6(void){return(73.130);}
    static double StdB7(void){return(87.950);}
    static double StdM3(void){return(1.81057);}
    static double StdM4(void){return(1.9013);}
    static double StdM5(void){return(1.9882);}
    static double StdM6(void){return(2.172);}
    static double StdM7(void){return(2.3761);}
    static double LowB3(void){return(22.034);}
    static double LowB4(void){return(25.495);}
    static double LowB5(void){return(30.00);}
    static double LowB6(void){return(36.542);}
    static double LowB7(void){return(43.545);}
    static double LowM3(void){return(0.9079);}
    static double LowM4(void){return(0.9428);}
    static double LowM5(void){return(0.9959);}
    static double LowM6(void){return(1.0858);}
    static double LowM7(void){return(1.1797);}

    static double MMtoInch(void){return(25.4);}
    static double MPAtoPSI(void){return(145.0);}
    static int TestSetSize(void){return(3);}
    static double TruncateScale(void){return(100.0);}

    //Calculate Standard Tables
    double CalStdTable(double b, double m, double length, double min);
};

#endif // INST_DATA_H
