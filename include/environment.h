//
// Created by liam on 6/13/21.
//

#ifndef LIBPTENV_ENVIRONMENT_H
#define LIBPTENV_ENVIRONMENT_H

// C/C++
#include <iostream>

// External
#include <torch/torch.h>

// Project
#include "indicators.h"
#include "dataManager.h"
#include "gaf_single.h"

enum verboseID {NONE = 0, RESULT = 1, FULL = 2};

/**
 * @brief This class provides an Forex Environment for Reinforced AI training
 * Functionality like Gym Enves known from python
 */
class Environment
{
public:
    // Constructor/Destructor
    Environment();
    ~Environment();

    // take next step with action from actor
    torch::Tensor step(int action, float *reward, bool *done);

    // Reset Environment
    torch::Tensor reset();


private:
    Settings settings; ///<Settings Object storing all user input from envsettings.json
    CandleStorage data; ///<Storage where the OHLCV data from the File is stored
    Indicators indicators; ///<Indicators object for calculating and storing the Indicators data

    // Environment tracker vars
    size_t us_pos; ///<Current idx in data and indicators. Not starting at 0 but windowSize for computation
    size_t us_step; ///<Current step. Unlike us_pos it starts at 0 for the first step.

    // Verbose
    void setVerbose(int i_mode = 0);
    int verboseMode = 0;


protected:

};


#endif //LIBPTENV_ENVIRONMENT_H
