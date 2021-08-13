//
// Created by liam on 6/2/21.
//

#ifndef LIBPTENV_THREADSYNC_H
#define LIBPTENV_THREADSYNC_H

// C/C++
#include <iostream>
#include <mutex>
#include <condition_variable>
#include <atomic>
// External

// Project


/**
 * @brief Creates the possibility to make worker threads wait for more input before
 * continuing their work
 */
class ThreadSync
{
public:
    ThreadSync() noexcept;

    // Catches a thread and waits for a notification
    void waitForSignal() noexcept;

    // Notify all waiting threads to start running
    void notifyWaiting() noexcept;

    // Get number of waiting threads
    int getWaiting() noexcept;

    // Check if all spawned threads should be killed
    bool getKillThreads() noexcept;

    // Set if all threads should be killed
    void setKillThreads(bool b_kill) noexcept;

private:

    std::mutex s_ConditionMutex; ///<Mutex to lock the condition
    std::condition_variable s_Condition; ///<Condition varbiable to make threads wait
    std::atomic<int> i_Waiting; ///<Contains the number of threads waiting. Get with getWaiting()
    std::atomic<bool> b_killThreads; ///<True if threads should be killed. Get with getKillThreads.

protected:

};


#endif //LIBPTENV_THREADSYNC_H
