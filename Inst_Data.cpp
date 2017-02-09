/* ---------------------------------------------------------------------------
** This software is furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** File Name: Inst_Data.cpp
**
** parser.cpp - windsorlinx software
** Divides the input stream up into data packets
**
** Author: Michael W. Hoag
** Copyright Michael W. Hoag 2017
** Email: mike@ndtjames.com
** -------------------------------------------------------------------------*/

#include "Inst_Data.h"


DataSet::DataSet()
{

}

DataSet::DataSet(DataSet::Test Init_Test)
{
    TestData.push_back( Init_Test );
}

void DataSet::AddTest(DataSet::Test test)
{
    TestData.push_back( test );
}

DataSet::Test DataSet::GetTest(std::vector<Test>::iterator itr_current)
{
    Test returntest = *itr_current;

    return(returntest);
}

std::vector<DataSet::Test>::iterator DataSet::GetBeginItr()
{
    return( TestData.begin() );
}

std::vector<DataSet::Test>::iterator DataSet::GetEndItr()
{
    return( TestData.end() );
}

/*
 * Calculates the Exposed Probe Length returns a four
 * unit vector
 */
std::vector<double> DataSet::TestLength( std::vector<Test>::iterator current_test){

    double span, adcscale, adcdistance;

    std::vector<double>return_length(Vector_Size());//three for the data last one is average

    span = (double)((current_test->ADC[LocADCFullScale()]- //location of ADCFullScale
                     current_test->ADC[LocADCZero()])); //location of ADCZero

    if(current_test->TestProp.PropUnits == MPA){
        adcscale = ADCScaleFactorMetric()/span;
        for(int i=0;i<(NumTests());++i){
            adcdistance = current_test->ADC[2+i]-current_test->ADC[LocADCZero()];
            return_length[i] = (double)(adcdistance*adcscale) + DistanceOffsetMetric();
            return_length[LocAVG()] += (double)return_length[i]/(double)NumTests();
        }
    }else{
        adcscale = ADCScaleFactorMetric()/span;
        for(int i=0;i<(NumTests());++i){
            //offset skips elems needed for span
            adcdistance = current_test->ADC[2+i]-current_test->ADC[LocADCZero()];
            return_length[i] = ((double)(adcdistance*adcscale) + DistanceOffsetMetric())
                    / MMtoInch();
            return_length[LocAVG()] += return_length[i]/(double)NumTests();
        }
    }
    return(return_length);
}

/*
 * Calculates the Concrete Compressive Strength
 */

std::vector<double> DataSet::TestStrength(std::vector<Test>::iterator current_test){

    double       b = 0;
    double       m = 0;
    std::vector<double> length = TestLength(current_test); //vector exposed probe lengths
    std::vector<double> return_vector(Vector_Size()); //return vector

    Power private_power = current_test->TestProp.PropPower;
    Weight private_weight = current_test->TestProp.PropWeight;
    Moh private_moh;
    if(private_power == Std_Power || private_power == Low_Power ){
        private_moh = current_test->TestProp.PropMoh;
    }
    length = TestLength( current_test );

    if(current_test->TestProp.PropUnits == DataSet::PSI){ //converts to mm
            std::transform(length.begin(), length.end(), length.begin(),
                std::bind1st(std::multiplies<double>(),MMtoInch()));
    }
    switch (private_power){
    case High_Power:
        for(auto d = length.begin(), y = return_vector.begin();
            d != length.end(); ++d, ++y){
            *y = HighPowerB() * exp( *d * HighPowerM());
        }
        break;
    case Std_Power:
        switch (private_moh){
        case Value_3:
             b = StdB3();
             m = StdM3();
           for(auto d = length.begin(), y = return_vector.begin();
               d != length.end(); ++d, ++y){
               *y = *d > StdMin3()? (m * *d) - b : 0;
           }
           break;
        case Value_4:
            b = StdB4();
            m = StdM4();
            for(auto d = length.begin(), y = return_vector.begin();
                d != length.end(); ++d, ++y){
                *y = *d > StdMin4()? (m * *d) - b : 0;
            }
           break;
        case Value_5:
            b = StdB5();
            m = StdM5();
            for(auto d = length.begin(), y = return_vector.begin();
                d != length.end(); ++d, ++y){
                *y = *d > StdMin5()? (m * *d) - b : 0;
            }
           break;
        case Value_6:
            b = StdB6();
            m = StdM6();
            for(auto d = length.begin(), y = return_vector.begin();
                d != length.end(); ++d, ++y){
                *y = *d > StdMin6()? (m * *d) - b : 0;
            }
            break;
        case Value_7:
            b = StdB7();
            m = StdM7();
            for(auto d = length.begin(), y = return_vector.begin();
                d != length.end(); ++d, ++y){
                *y = *d > StdMin7()? (m * *d) - b : 0;
            }
            break;
        }
        if(private_weight == Med_Weight){
            b = StdB3();
            m = StdM3();
            for(auto d = length.begin(), y = return_vector.begin();
                d != length.end(); ++d, ++y){
                *y = *d > StdMin3()? (m * *d) - b : 0;
            }
        }
        break;
    case Low_Power:
        switch (private_moh){
        case Value_3:
           b = LowB3();
           m = LowM3();
           for(auto d = length.begin(), y = return_vector.begin();
               d != length.end(); ++d, ++y){
                *y = *d > LowMin3()? (m * *d) - b : 0;
           }
           break;
        case Value_4:
            b = LowB4();
            m = LowM4();
            for(auto d = length.begin(), y = return_vector.begin();
                d != length.end(); ++d, ++y){
                *y = *d > LowMin4()? (m * *d) - b : 0;
            }
           break;
        case Value_5:
            b = LowB5();
            m = LowM5();
            for(auto d = length.begin(), y = return_vector.begin();
                d != length.end(); ++d, ++y){
                *y = *d > LowMin5()? (m * *d) - b : 0;
            }
           break;
        case Value_6:
            b = LowB6();
            m = LowM6();
            for(auto d = length.begin(), y = return_vector.begin();
                d != length.end(); ++d, ++y){
                *y = *d > LowMin6()? (m * *d) - b : 0;
            }
            break;
        case Value_7:
            b = LowB7();
            m = LowM7();
            for(auto d = length.begin(), y = return_vector.begin();
                d != length.end(); ++d, ++y){
                *y = *d > LowMin7()? (m * *d) - b : 0;
            }
           break;
        }
        if(private_weight == Med_Weight){
            b = LowB3();
            m = LowM3();
            for(auto d = length.begin(), y = return_vector.begin();
                d != length.end(); ++d, ++y){
                *y = *d > LowMin3()? (m * *d) - b : 0;
            }
        }
        break;
    }

    // Scale for low and super-low weights
    if (private_weight == Low_Weight)
    {
        for(auto y = return_vector.begin();
            y != length.end(); ++y){
             *y *= 0.84;
        }
    }
    else if (private_weight == Very_Low_Weight)
    {
        for(auto y = return_vector.begin();
            y != length.end(); ++y){
             *y *= 0.66;
        }
    }

    if(current_test->TestProp.PropUnits == DataSet::PSI){ //converts to psi
            std::transform(return_vector.begin(), return_vector.end(), return_vector.begin(),
                std::bind1st(std::multiplies<double>(),MPAtoPSI()));
    }

    return(return_vector);

}


