//
// Created by liam on 5/28/21.
//

#ifndef LIBPTENV_SETTINGS_H
#define LIBPTENV_SETTINGS_H

// C/C++
#include <iostream>
#include <vector>

// External

// Project


/**
 * @brief Class for loading and storing the settings
 * Settings are configured using an json file
 */
class Settings
{
public:
    // General Settings
    std::string s_ohlcvPath; ///<Path to the file with OHLCV data
    float f64_learningRate; ///<Learningrate for the ANN optimizer
    int i_windowSize; ///<Window sizeor the GAF

    // Indicators
    int i_nIndicator; ///<Numbers of indicators

    bool b_SMA; ///<True if SMA enabled
    std::vector<int> vi_smaPeriods; ///<Vector containing the periods of the SMAs

    bool b_EMA; ///<True if EMA is enabled
    std::vector<int> vi_emaPeriods; ///<Vector containing the periods of the EMAs

    bool b_VWAP; ///<True if VWAP is enabled
};


#endif //LIBPTENV_SETTINGS_H
