#pragma once
#include "common.h"
#include "LRunnable.h"
#include "LMutex.h"

class LThread
{
    L_DISABLE_COPY(LThread)
public:
    typedef pthread_t HANDLE;

    LThread(LRunnable *fun = nullptr) noexcept;
    ~LThread() noexcept;

    void start() noexcept;
    void detach() noexcept;
    void wait() noexcept;

    bool isRuning() const noexcept;
    bool isFinished() const noexcept;
    bool isDetached() const noexcept;

    static HANDLE currentThreadId() noexcept;
    static void yieldCurrentThread() noexcept;
    static void exit() noexcept;

protected:
    virtual void run() noexcept;

private:
    HANDLE pth;

    LRunnable *runnable = nullptr;

    bool running = false;
    bool finished = false;
    bool detached = false;

    mutable LMutex mutex;

    static void *threadFunc(void *arg) noexcept;
    static void finish(void *) noexcept;
};