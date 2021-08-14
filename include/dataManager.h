//
// Created by liam on 5/28/21.
//

#ifndef LIBPTENV_DATAMANAGER_H
#define LIBPTENV_DATAMANAGER_H


// C/C++
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>

// External
#include <json.hpp>

// Project
#include "color.h"
#include "settings.h"


/**
 * @brief Class used to store pointers to price arrays
 */
 class CandleStorage
 {
 public:

     std::vector<double> f64_open; ///<Vector with open price
     std::vector<double> f64_high; ///<Vector with high price
     std::vector<double> f64_low; ///<Vector with low price
     std::vector<double> f64_close; ///<Vector with close price
     std::vector<double> f64_volume; ///<Vector with volume

     void cut(size_t length);
 };


/**
 * @brief Reads OHLCV-data from file.
 * Reads settings from files
 */
class DataManager
{

public:
    // Loading Settings from file
    static void readSettings(Settings &s);

    // Loading from a csv with , delimiter
    static void readOHLCV_csv(CandleStorage* storage, const std::string &s_filename);


private:

protected:

};

#endif //LIBPTENV_DATAMANAGER_H
