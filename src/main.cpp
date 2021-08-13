/*! \mainpage FxDQN
 *
 * \section introduction_sec Description of the project
 *
 * This projects tries to implement an DQN algorithm that
 * learns pattern from price and indicator timeseries
 * via an ANN that uses Convolutional networks to analyze
 * GAF encoded images and an 3 layer linear ANN to output
 * Buy|Sell|Hold|Close actions
 *
 * Indicators used can be configured in a JSON settings file
 *
 * \section install_sec Installation
 *
 * This project is build using CMAKE so u can install it as follows
 *
 * \code{.c}
 * mkdir build
 * cd build
 * cmake ..
 * make -j4
 * \endcode
 *
 * \section depend_sec Dependencies
 *
 * This project depends on the ta-lib library to work. It needs to be
 * installed rom the ta-lib.org website.
 *
 */

// C/C++
#include <iostream>

// External

// Project
#include "dataManager.h"


/**
 * Main function executed at start
 *
 * @return status code
 */
int main()
{
    return 0;
}
