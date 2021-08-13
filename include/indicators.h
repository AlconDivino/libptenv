//
// Created by liam on 5/28/21.
//

#ifndef LIBPTENV_INDICATORS_H
#define LIBPTENV_INDICATORS_H

// C/C++
#include <iostream>

// External
#include <ta-lib/ta_func.h>

// Project
#include "dataManager.h"
#include "color.h"



/**
 * @brief Class containing needed indicators as single static function
 * also contains functions to calculate entire arrays of indicators
 * over OHLC data
 */
class Indicators
{

public:
    // Constructor
    explicit Indicators(CandleStorage *in, Settings s);
    Indicators();

    // Setter
    void set_ohlcvPtr(CandleStorage *in, Settings s);

    // Calculate Indicators
    void calculateIndicators();


private:
    CandleStorage *inData = nullptr; ///<Object filled with OHLCV data
    std::vector<std::vector<double>> vf64_indicators; ///<Vector with pointer to double array for indicator storage
    Settings settings; ///<Settings Object containing user settings

protected:

};

#endif //LIBPTENV_INDICATORS_H
