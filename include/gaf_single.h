//
// Created by liam on 8/10/21.
//

#ifndef LIBPTENV_GAF_SINGLE_H
#define LIBPTENV_GAF_SINGLE_H

// C/C++
#include <iostream>
#include <cmath>
#include <vector>
#include <thread>

// External

// Project
#include "threadSync.h"

using dVec2D = std::vector<std::vector<float>>;
using fVec = std::vector<float>;
using dVec = std::vector<double>;


/**
 * @brief Implementation of the GAF class while not computing the whole dataset at once
 * but one step at a time to save space for larger datasets. Computing is done with multiple threads
 * to ensure minimized loss of performance. This class assumes, that the input vectors of every indicator
 * have been trimmed to the same length
 */
class sGAF
{
public:
    // Constructor
    sGAF(size_t us_startIdx, size_t us_window, dVec2D& vf64_in);
    ~sGAF();

    // compute next
    float* compute_next();

    // Set current index
    void setIdx(size_t idx);

private:
    dVec2D &vf64_input; ///<Reference to Input 2d double vector.
    fVec vf_dataOut; ///<Storage for the output (Continuous) data_ptr is passed to tensor
    size_t us_globalPos; ///<Index on the INPUT vector to be computed
    size_t us_idxIndicator; ///<Index of the Indicator (where to store the data on the vf_dataOut)
    size_t us_numIndicators; ///<Size of vf64_input
    size_t us_windowSize; ///<Size of the Window we want to compute for
    ThreadSync syncer; ///<Threadsync class for making threads wait when not computing gafs
    std::vector<std::thread> v_threads; ///< Vector for storing running threads
    int i_nthreads;
    int i_nWaiting;

    // static compute function for threads
    static void _compute(int id,
                         ThreadSync *syncer,
                         size_t *us_globPos,
                         size_t *us_idxInd,
                         size_t us_windowSize,
                         dVec2D *input,
                         fVec *vf_out);

protected:

};



#endif //LIBPTENV_GAF_SINGLE_H
