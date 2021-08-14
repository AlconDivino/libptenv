//
// Created by liam on 6/13/21.
//

#include "environment.h"

/**
 * @brief Constructor
 * Loads data and settings
 */
Environment::Environment()
{
    // Load settings
    DataManager::readSettings(this->settings);

    // Load all candle Data
    DataManager::readOHLCV_csv(&data, settings.s_ohlcvPath);

    // Calculate indicators
    indicators.set_ohlcvPtr(&data, settings);
    indicators.calculateIndicators();

    // Cut ohlcv data to indicator length
    data.cut(indicators.getSize());
}

Environment::~Environment()
{

}

/**
 * @brief Takes a step in the environment with provided action.
 * Returns reward and if the environment is done as parameters
 * Returns the next state tensor
 *
 * @param action Integer action either [Buy, Sell, Hold, Close]
 * @param reward
 * @param done
 * @return
 */
torch::Tensor Environment::step(int action, float *reward, bool *done)
{
    return torch::Tensor();
}

/**
 *
 * @return
 */
torch::Tensor Environment::reset()
{
    return torch::Tensor();
}

/**
 * @brief Sets the verbose mode.
 * NONE does not print anything
 * RESULT prints the result when done is true
 * FULL prints the state of the environment at every step
 *
 * @param s_mode Mode from enum verboseID
 */
void Environment::setVerbose(int i_mode)
{

}
