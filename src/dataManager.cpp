//
// Created by liam on 5/28/21.
//

#include <dataManager.h>

/**
 * @brief Cuts the vectors in the class to a specific size
 * @param length desired size of the vector
 */
void CandleStorage::cut(size_t length)
{
    if(length == 0)
        throw std::runtime_error("[CandleStorage] ERROR Cannot operate with data size 0");

    while(f64_open.size() > length)
    {
        f64_open.erase(f64_open.begin());
        f64_high.erase(f64_high.begin());
        f64_low.erase(f64_low.begin());
        f64_close.erase(f64_close.begin());
        f64_volume.erase(f64_volume.begin());
    }
}


/**
 * @brief Reads the settings from a json-style settings file ../envsettings.json
 * @param s Settings class to save the settings to
 */
void DataManager::readSettings(Settings &s)
{
    nlohmann::json settings;
    std::string content;

    // read data from file
    std::ifstream ifs("../envsettings.json");

    // Check if file is open else return
    if(!ifs.is_open())
    {
        printf("[DataManager] %s ERROR %s Could not read settings file! "
               "(../envsettings.json)\n",
               RED, RESET);
        exit(1);
    }

    std::string line;
    while(std::getline(ifs, line))
        content.append(line);

    // Write json to settings
    settings = nlohmann::json::parse(content);

    // Parse settings here
    s.f64_learningRate = settings["learningRate"];
    s.s_ohlcvPath = settings["dataset"];
    s.i_windowSize = settings["windowSize"];

    s.i_nIndicator = (int)settings["indicators"].size(); // Get number of indicators

    // Check if SMA is enabled and get periods
    s.b_SMA = settings["indicators"]["SMA"]["enabled"];
    s.vi_smaPeriods.resize(settings["indicators"]["SMA"]["period"].size());
    for(int i = 0; i<s.vi_smaPeriods.size(); i++)
        s.vi_smaPeriods[i] = settings["indicators"]["SMA"]["period"][i];

    // Check if EMA is enabled and get periods
    s.b_EMA = settings["indicators"]["EMA"]["enabled"];
    s.vi_emaPeriods.resize(settings["indicators"]["EMA"]["period"].size());
    for(int i = 0; i<s.vi_emaPeriods.size(); i++)
        s.vi_emaPeriods[i] = settings["indicators"]["EMA"]["period"][i];

    s.b_VWAP = settings["indicators"]["VWAP"]["enabled"];
}



/**
 * @brief Reads CSV from given file. Expects a ',' delimiter and the form
 * Date,Time,Open,High,Low,Close.
 * Filters all lines that are empty or with headers beginning with <, #
 *
 * @param storage ptr to CandleStorage object to store the data in
 * @param s_filename filename for the file to load
 */
void DataManager::readOHLCV_csv(CandleStorage *storage, const std::string &s_filename)
{
    printf("[DataManager] Reading OHLCV data fro file %s\n", s_filename.c_str());
    int i_lineCount = 0;

    // Open file and check if its exists
    std::ifstream ifs(s_filename);

    if(!ifs.is_open())
    {
        printf("[DataManager] %s ERROR %s Could not read from file!\n", RED, RESET);
        exit(1);
    }

    // Read s_line per s_line and extract the data
    std::string s_line, s_col;
    bool b_infoPrinted = false;

    while(std::getline(ifs, s_line)) // Loop through lines
    {
        // Check if s_line is empty or not valid
        if(s_line.empty() || s_line[0] == '#' || s_line[0] == '<')
            continue;

        // Check how many columns there are and if they are the expected count
        // But only once
        if(!b_infoPrinted)
        {
            int i_nCols = (int)std::count(s_line.begin(), s_line.end(), ',') + 1;
            printf("[DataManager] NOTICE there are %i columns\n", i_nCols);

            // Check if there are more columns than expected
            if(i_nCols > 7)
                printf("[DataManager] %s WARNING %s the number of given columns exceeds "
                       "the expected 7 columns. The rest will not be used!\n", YELLOW, RESET);

            // CHeck if there are less columns than expected
            if(i_nCols < 7)
            {
                printf("[DataManager] %s ERROR %s There were only %i columns given"
                       "in the file! The minimum amount is 7 formatted as: \n\n"
                       "Date,Time,Open,High,Low,Close,Volume\n\n"
                       "Please use a file that meets those conditions.\n", RED, RESET, i_nCols);
                exit(1);
            }

            b_infoPrinted = true;
        }


        // Create stringstream for separating string
        std::stringstream ss(s_line);
        int i_counter = 0;

        while(std::getline(ss, s_col, ',')) // Loop through extracted string
        {
            switch (i_counter)
            {
                case 0:
                    i_counter++;
                    break;
                case 1:
                    i_counter++;
                    break;
                case 2:
                    storage->f64_open.push_back( std::stod(s_col) );
                    i_counter++;
                    break;
                case 3:
                    storage->f64_high.push_back( std::stod(s_col) );
                    i_counter++;
                    break;
                case 4:
                    storage->f64_low.push_back( std::stod(s_col) );
                    i_counter++;
                    break;
                case 5:
                    storage->f64_close.push_back( std::stod(s_col) );
                    i_counter++;
                    break;
                case 6:
                    storage->f64_volume.push_back( std::stod(s_col) );
                    i_counter++;
                    break;
                default:
                    break;
            }
        }

        i_lineCount++;
    }

    printf("[DataManager] NOTICE Read %i lines from file\n", i_lineCount);
}


