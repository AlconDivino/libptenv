//
// Created by liam on 6/2/21.
//

#include <threadSync.h>

/**
 * @brief Constucts the object initializes the b_killThreads variable
 */
ThreadSync::ThreadSync() noexcept : b_killThreads(false),
                                    i_Waiting(0)
{}

/**
 * @brief Catches a thread and waits for a notification.
 * Call from worker thread.
 */
void ThreadSync::waitForSignal() noexcept
{
    // Increment waiting counter
    ++i_Waiting;

    // Wait for signal
    std::unique_lock<std::mutex> s_Lock(s_ConditionMutex);
    s_Condition.wait(s_Lock);
}

/**
 * @brief Notify all waiting threads to start running again.
 * Call from main Thread
 */
void ThreadSync::notifyWaiting() noexcept
{
    // Reset counter before running all threads
    i_Waiting = 0;

    // Lock and signal
    s_Condition.notify_all();
}

/**
 * @brief Get number of waiting threads. Call from main thread.
 * @return number of waiting threads
 */
int ThreadSync::getWaiting() noexcept
{
    return i_Waiting;
}

/**
 * @brief Check if all spawed threads should be killed. Call from Worker Thread
 * @return b_killThread
 */
bool ThreadSync::getKillThreads() noexcept
{
    return b_killThreads;
}

/**
 * @brief set if all threads should be killed. Call from main Thread
 */
void ThreadSync::setKillThreads(bool b_kill) noexcept
{
    b_killThreads = b_kill;
}

