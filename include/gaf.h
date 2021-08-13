//
// Created by liam on 5/31/21.
//

#ifndef LIBPTENV_GAF_H
#define LIBPTENV_GAF_H

// C/C++
#include <iostream>
#include <vector>
#include <thread>
#include <cmath>

// External
#include <torch/torch.h>

// Project
#include "threadSync.h"
#include "settings.h"


using vvec = std::vector<std::vector<double>>;
using dvec = std::vector<double>;
using Tensor = torch::Tensor;


/**
 * @brief Gramian Angular field class
 * Takes the data vectors computed using indicators etc and computes corresponding
 * Gramian Angular Fields from that input
 */
class GAF
{
public:
    // Constructor
    GAF(vvec *vf64_in, const Settings &settings);


private:
    // function passed to threads
    static void calculate_threads(int i_id,
                                  dvec **vf64_in,
                                  dvec **vf64_out,
                                  ThreadSync *ts,
                                  const Settings settings);

    // Starts computation and gets result
    void start();
    void result(std::vector<Tensor> &vt_result);
    size_t getVectorSize();

    // Storage classes
    int i_threadCount; ///<Number of threads created by GAF
    ThreadSync s_threadSync; ///<ThreadSync class to make the computing threads pause before new data is set
    Settings settings;
    std::vector<std::thread> v_threads; ///<Vector to store Running threads in
    vvec vf64_res; ///<Vector to store the computed GAFs into
    vvec *vf64_inData; ///<Pointer storage for start()
    dvec *vf64_dataBuffer = nullptr; ///<Data buffer pointer for threads
    dvec *vf64_resultBuffer; ///<Result buffer for threads

protected:

};


#endif //LIBPTENV_GAF_H
