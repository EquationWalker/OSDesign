#pragma once
#include "common.h"
class LRunnable
{
    L_DISABLE_COPY(LRunnable)
public:
    LRunnable() = default;
    ~LRunnable() = default;
    virtual void run() noexcept = 0;
};