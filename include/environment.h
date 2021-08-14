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


protected:

};


#endif //LIBPTENV_ENVIRONMENT_H
