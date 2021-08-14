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
    Indicators indicators;
    indicators.set_ohlcvPtr(&data, settings);
    indicators.calculateIndicators();

    /// \todo cut data to indicator length
}

Environment::~Environment()
{

}

torch::Tensor Environment::step(int action, float *reward, bool *done) {
    return torch::Tensor();
}

torch::Tensor Environment::reset() {
    return torch::Tensor();
}
