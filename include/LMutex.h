#pragma once
#include "common.h"

class LMutex final
{
public:
    enum RecursiveMode
    {
        Recursive,
        NoRecursive
    };
    LMutex(RecursiveMode mode = NoRecursive);
    ~LMutex();
    void lock();
    bool tryLock();
    void unlock();

private:
    pthread_mutex_t mutex;
    L_DISABLE_COPY(LMutex)
};