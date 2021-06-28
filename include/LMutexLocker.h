#pragma once
#include "LMutex.h"

class LMutexLocker final
{
    L_DISABLE_COPY(LMutexLocker)
public:
    inline explicit LMutexLocker(LMutex *mutex) noexcept
    {
        assert(mutex != NULL); // cannot null

        m = mutex;
        isLocked = true;
        mutex->lock();
    }

    inline ~LMutexLocker() noexcept
    {
        unlock();
    }

    inline void relock() noexcept
    {
        if (!isLocked)
            m->lock(), isLocked = true;
    }
    inline void unlock() noexcept
    {
        if (isLocked)
            m->unlock(), isLocked = false;
    }

    inline LMutex *mutex() const
    {
        return const_cast<LMutex *>(m);
    }

private:
    LMutex *m;
    bool isLocked = false;
};