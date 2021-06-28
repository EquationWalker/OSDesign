#pragma once
#include "LMutex.h"

class LWaitCondition final
{
    L_DISABLE_COPY(LWaitCondition)
public:
    LWaitCondition() noexcept;
    ~LWaitCondition() noexcept;

    /* The calling thread must ensure that mutex is locked
     If the time is 0, it means wait all the time; 
     otherwise wait for the absolute value of the time.*/
    void wait(LMutex *mutex, unsigned long time = 0) noexcept;
    void wakeOne() noexcept;
    void wakeAll() noexcept;

private:
    pthread_cond_t cond;
};