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
 * @brief This class provides the environment for every
 */
class Environment
{
public:
    // Constructor/Destructor
    Environment();
    ~Environment();

    torch::Tensor step(int action, float *reward, bool *done);

    torch::Tensor reset();


private:

protected:

};


#endif //LIBPTENV_ENVIRONMENT_H
