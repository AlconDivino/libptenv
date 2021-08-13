//
// Created by liam on 8/10/21.
//

#include "gaf_single.h"

/**
 * @brief Constructor for sGAF.
 *
 * Takes a 2d vector that contains vectors of continous indicator value
 *
 * @param us_startIdx Index for the computation to start
 * @param vf64_in 2d double vector to compute gafs from.
 * @param t_out Tensor to save the Data to. Same Tensor gets overwritten everytime compute_next is called
 */
sGAF::sGAF(size_t us_startIdx, size_t us_window, dVec2D& vf64_in) : vf64_input(vf64_in)
{
    us_globalPos = us_startIdx;
    us_numIndicators = vf64_in.size();
    us_windowSize = us_window;

    vf_dataOut.resize(us_windowSize * us_windowSize * us_numIndicators);

    // Create threads
    i_nthreads = (int)std::thread::hardware_concurrency();
    for(int id = 1; id < i_nthreads; id++)
    {
        v_threads.emplace_back(sGAF::_compute,
                               id,
                               &syncer,
                               &us_globalPos,
                               &us_idxIndicator,
                               us_windowSize,
                               &vf64_input,
                               &vf_dataOut);
    }

}

/**
 * @brief Takes care of stoping the threads
 */
sGAF::~sGAF()
{

    syncer.setKillThreads(true);
    sGAF::_compute(0,
                   &syncer,
                   &us_globalPos,
                   &us_idxIndicator,
                   us_windowSize,
                   &vf64_input,
                   &vf_dataOut);

    for(auto & v_thread : v_threads)
        v_thread.join();

}

/**
 * @brief Computes the next input gaf
 * @return A Pointer to vf_dataOut data Array for Tensor creation
 */
float* sGAF::compute_next()
{


    // Start loop for indicators
    for( size_t i = 0; i < us_numIndicators; i++)
    {
        i_nWaiting = 0;
        us_idxIndicator = i;

        // Compute in MainThread (id 0)
        sGAF::_compute(0,
                       &syncer,
                       &us_globalPos,
                       &us_idxIndicator,
                       us_windowSize,
                       &vf64_input,
                       &vf_dataOut);

        i_nWaiting = syncer.getWaiting();
        while(i_nWaiting < i_nthreads-1)
            i_nWaiting = syncer.getWaiting();
    }
    // Increment the global pos
    us_globalPos++;

    return vf_dataOut.data();
}

/**
 * @brief Sets the index in the class to know which step to compute now
 * @param idx index
 */
void sGAF::setIdx(size_t idx)
{
    us_globalPos = idx;
}

/**
 * @brief Function is passed to the waiting threads. Makes them wait for signal of main before computing
 * The thread with the id 0 needs notify the waiting threads. So the Main thread is expected to call
 * the function with the thread id 0. This is to ensure the threads are being used efficiently.
 *
 * @param id Unique ID of the thread
 * @param syncer Syncer class for waiting and starting
 * @param us_globPos Pointer to the current globalIndex on all Indicators
 * @param us_idxInd Pointer to the current Index on vf64_input. For knowing where to store the out data
 * @param input Pointer to the 2D double vector of input
 * @param vf_out Pointer to out-DataVector (presized)
 */
void sGAF::_compute(int id,
                    ThreadSync *syncer,
                    size_t *us_globPos,
                    size_t *us_idxInd,
                    size_t us_windowSize,
                    dVec2D *input,
                    fVec*vf_out)
{
    // Reusable variable declaration //
    size_t in_startIdx; // Index on the input vector to work with
    size_t in_endIdx; // Index that starts the area of a new loop
    size_t out_startIdx; // Index on which to start writing the results
    double af_Buffer[us_windowSize]; // Buffer for acos Values
    // How many jobs have to be done (How many idx)
    auto nJobs = static_cast<size_t>(us_windowSize / std::thread::hardware_concurrency());
    double f64_min, f64_max, f64_divisor;

    // Start of waiting and stuff
    while(true)
    {
        /**
         * Notify and kill stuff
         */
        if(id == 0)                                     // main Thread
        {
            syncer->notifyWaiting();
        }
        else
        {                                        // Every other thread
            syncer->waitForSignal();
        }

        // Get if threads should be killed
        if(syncer->getKillThreads())
            return;

        // ------------------------------------------------------------------------ //
        // ---------------------Start Computation of the GAF----------------------- //
        // ------------------------------------------------------------------------ //

        // Calculation of the start and end Idx for the Inputs and start Idx for the Output
        in_startIdx = (*us_globPos) + (id * nJobs);
        in_endIdx = in_startIdx * nJobs;
        out_startIdx = (*us_idxInd) * (id * (us_windowSize + us_windowSize * nJobs));

        for(size_t idx = in_startIdx; idx < in_endIdx; idx++)
        {
            // Get Min/Max
            f64_max = (*input)[*us_idxInd][idx];
            f64_min = (*input)[*us_idxInd][idx];
            for(size_t i = idx - us_windowSize - 1; i <= idx; i++)
            {
                if((*input)[*us_idxInd][i] > f64_max)
                    f64_max = (*input)[*us_idxInd][i];
                if((*input)[*us_idxInd][i] < f64_min)
                    f64_min = (*input)[*us_idxInd][i];
            }

            // Scale and acos the values
            f64_divisor = f64_max - f64_min;
            for(size_t i = idx - us_windowSize - 1, j = 0; i <= idx; i++, j++)
            {
                af_Buffer[j] = acos(( (*input)[*us_idxInd][i] - f64_min) / f64_divisor);
            }

            // Calculate the cos sum thing of dotproduct thing
            for(size_t i = 0; i < us_windowSize; i++)
            {
                for(size_t j = 0; j < us_windowSize; j++)
                {
                    (*vf_out)[out_startIdx] = static_cast<float>(cos(af_Buffer[i] + af_Buffer[j]));
                }
            }

        }

    }
}


