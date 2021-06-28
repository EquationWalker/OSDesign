#pragma once
#include "common.h"
class LWaitCondition;
class LMutex final
{
    L_DISABLE_COPY(LMutex)
public:
    friend class LWaitCondition;
    enum RecursiveMode
    {
        Recursive,
        NoRecursive
    };
    LMutex(RecursiveMode mode = NoRecursive) noexcept;
    ~LMutex() noexcept;
    void lock() noexcept;
    bool tryLock() noexcept;
    void unlock() noexcept;

private:
    pthread_mutex_t mutex;
};