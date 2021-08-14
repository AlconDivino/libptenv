//
// Created by liam on 5/28/21.
//

#include "indicators.h"

/**
 * @brief Default constructor for Indicators
 */
Indicators::Indicators() {}

/**
 * @brief Explicit Constructor for Indicators class
 * takes pointer to specified CandleStorage and sets it to
 * the idData pointer for further use
 * @param in Pointer to filled CandleStorage object
 */
Indicators::Indicators(CandleStorage *in, Settings s)
{
    inData = in;
    settings = s;
}

/**
 * @brief Sets the pointer address of in to inData for further use
 * @param in pointer to filled CandleStorage object
 */
void Indicators::set_ohlcvPtr(CandleStorage *in, const Settings &s)
{
    inData = in;
    settings = s;
}

/**
 * @brief Calculates the value for the indicators the user specified
 */
void Indicators::calculateIndicators()
{
    printf("[Indicators] NOTICE Calculating indicators. "
           "User specified a total of %i Indicators:\n", settings.i_nIndicator);

    TA_RetCode retCode;
    TA_Integer outBeg;
    TA_Integer outNbElement;

    // SMA
    if(settings.b_SMA)
    {
        for(int i = 0; i < settings.vi_smaPeriods.size(); i++)
        {
            double out[inData->f64_close.size()];
            retCode = TA_SMA(0, (int)inData->f64_close.size(),
                             inData->f64_close.data(),
                             settings.vi_smaPeriods[i],
                             &outBeg, &outNbElement, out);

            if(retCode != TA_SUCCESS) // Check for errors
            {
                printf("[Indicators] %s ERROR %s there was an error computing the SMA"
                       " with the period %i!", RED, RESET, settings.vi_smaPeriods[i]);
                exit(1);
            }

            // Pushback sized vector and copy values
            vf64_indicators.emplace_back(outNbElement);
            size_t indIdx = vf64_indicators.size() - 1;
            for(int idx = 0; idx < outNbElement; idx++)
            {
                vf64_indicators[indIdx][idx] = out[idx];
            }
        }
    }

    // EMA
    if(settings.b_EMA)
    {
        for(int i = 0; i < settings.vi_emaPeriods.size(); i++)
        {
            double out[inData->f64_close.size()];
            retCode = TA_EMA(0, (int)inData->f64_close.size(),
                             inData->f64_close.data(),
                             settings.vi_emaPeriods[i],
                             &outBeg, &outNbElement, out);

            if(retCode != TA_SUCCESS) // Check for errors
            {
                printf("[Indicators] %s ERROR %s there was an error computing the EMA"
                       " with the period %i!", RED, RESET, settings.vi_emaPeriods[i]);
                exit(1);
            }

            // Pushback sized vector and copy values
            vf64_indicators.emplace_back(outNbElement);
            size_t indIdx = vf64_indicators.size() - 1;
            for(int idx = 0; idx < outNbElement; idx++)
            {
                vf64_indicators[indIdx][idx] = out[idx];
            }
        }
    }

    /// \todo implement VWAP and others
    /// \todo Maybe option to feed prices / price average from last candle too
    /// \todo Check if the indicator vectors are already being cut to the same length

    // Find the smallest vector and cut the rest to its size
    size_t us_smallest = vf64_indicators[0].size();
    for(auto &indicator : vf64_indicators)
    {
        if(indicator.empty())
            throw std::runtime_error("[INDICATORS] ERROR One of the Indicators has size 0!");
        if(indicator.size() < us_smallest)
            us_smallest = indicator.size();
    }

    for(auto &indicator : vf64_indicators)
    {
        while(indicator.size() > us_smallest)
            indicator.erase(indicator.begin());
    }

}

/**
 * @brief For getting the data from indicators array
 * @return pointer to vf64_indicators vector
 */
std::vector<std::vector<double>> *Indicators::getIndicators()
{
    return &vf64_indicators;
}

/**
 * @brief Gets how many entries each indicators has
 * @return size of vf64_indicators[0]
 */
size_t Indicators::getSize()
{
    return vf64_indicators[0].size();
}
