  //
// Created by liam on 5/31/21.
//

#include "gaf.h"

/**
 * @brief Constructor of the GAF class
 * Creates the threads
 * @param i_nThreads
 */
GAF::GAF(vvec *vf64_in,const Settings &settings)
{
    this->settings = settings;
    vf64_inData = vf64_in;
    // Inizialize the out vector
    vf64_res = vvec(vf64_in->size(),
                    dvec(settings.i_windowSize * settings.i_windowSize *
                         (*vf64_in)[0].size() - settings.i_windowSize));

    // Create threads
    i_threadCount = (int)std::thread::hardware_concurrency();
    for(int i = 0; i < i_threadCount; i++)
    {
        v_threads.emplace_back(GAF::calculate_threads,
                               i,
                               &vf64_dataBuffer,
                               &vf64_resultBuffer,
                               &s_threadSync,
                               settings);
    }
}

/**
* @brief Function passed to the threads
* @param i_d ID of the thread executing the function (Needed for deciding which
* part of the in data to calculate
*/
void GAF::calculate_threads(int i_id,
                            dvec **vf64_in,
                            dvec **vf64_out,
                            ThreadSync *ts,
                            const Settings settings)
{
    // calculate positions of the thread
    size_t us_windowStart = (i_id *
            ((*vf64_in)->size() / std::thread::hardware_concurrency()));
    size_t us_windowEnd = us_windowStart +
            ((*vf64_in)->size() / std::thread::hardware_concurrency());

    dvec vf64_scaled((*vf64_in)->size()); // init scaled vector

    if(i_id == 0)
    {
        us_windowStart += settings.i_windowSize -1;
    }
    if(us_windowEnd > (*vf64_in)->size())
    {
        us_windowEnd = (*vf64_in)->size();
    }

    // Re-usble storage
    double f64_min, f64_max;

    // start computation loop
    while(!ts->getKillThreads())
    {
        // Wait for signal to continue
        ts->waitForSignal();

        size_t resIdx = i_id * (*vf64_in)->size();

        for(size_t idx = us_windowStart; idx < us_windowEnd; idx++)
        {
            // get min/max
            f64_min = (*(*vf64_in))[idx - settings.i_windowSize];
            f64_max = (*(*vf64_in))[idx - settings.i_windowSize];
            for(size_t i = idx - settings.i_windowSize + 1; i != idx ; i++)
            {
                if((*(*vf64_in))[i] > f64_max)
                    f64_max = (*(*vf64_in))[i];
                if((*(*vf64_in))[i] < f64_min)
                    f64_min = (*(*vf64_in))[i];
            }

            // scale vectors and acos the values
            double f64_divisor = f64_max - f64_min;
            for(size_t i = idx - settings.i_windowSize + 1, j = 0; i != idx; i++, j++)
            {
                vf64_scaled[j] = acos(((*(*vf64_in))[i] - f64_min) / f64_divisor);
            }

            // cos sum of the scaled vector to the thing
            for(size_t i = 0;i < settings.i_windowSize; i ++)
            {
                for(size_t j = 0; j < settings.i_windowSize; j++)
                {
                    (*(*vf64_out))[resIdx] = cos(vf64_scaled[i] + vf64_scaled[j]);
                }
            }
        }
    }
}

/**
* @brief Starts the computation
*/
void GAF::start()
{
    printf("[GAF] NOTICE calculating Gramian Angular Fields\n");

    while(s_threadSync.getWaiting() < i_threadCount) // Waiting for threads to all wait
        std::this_thread::sleep_for(std::chrono::milliseconds(10));

    //start computation loop
    size_t us_inSize = vf64_inData->size();
    for(size_t i = 0; i < us_inSize; i++)
    {
        if(i == us_inSize - 1)
            s_threadSync.setKillThreads(true);

        // set databuffer pointer
        vf64_dataBuffer = &((*vf64_inData)[i]);
        vf64_resultBuffer = &vf64_res[i];
        s_threadSync.notifyWaiting();

        // Check every 10 ms if they are finished
        while(s_threadSync.getWaiting() < i_threadCount)
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}


/**
* @brief Writes the result tensors to pre-sized parameter reference vt_result
* @param vt_result Reference to pre-sized tensor-vector (allocated before calling function
*/
void GAF::result(std::vector<Tensor> &vt_result)
{
    printf("[GAF] NOTICE Getting result from GAF computation\n");
    float* ptr_tensor = nullptr;
    size_t us_nIndicators = vf64_res.size();
    for(size_t idx = 0; idx < vt_result.size(); idx++)
    {
        ptr_tensor = static_cast<float *>(vt_result[idx].data_ptr());
        size_t t_idx = 0;

        for(size_t ind = 0; ind < us_nIndicators; ind++)
        {
            size_t dataSize = settings.i_windowSize * settings.i_windowSize;
            size_t endIdx = idx * dataSize + dataSize;
            for(size_t i = idx * dataSize; i < endIdx; i++, t_idx++)
            {
                ptr_tensor[t_idx] = vf64_res[ind][i];
            }
        }
    }
}

/**
 * @brief Returns the nuber of tensors stored. Needed for sizing of vector
 * @return Number of Tensors needed
 *
 */
size_t GAF::getVectorSize()
{
  return vf64_res.size() / (settings.i_windowSize * settings.i_windowSize);
}
