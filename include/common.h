#pragma once
#include <cstdio>
#include <cstdlib>
#include <pthread.h>

#define L_DISABLE_COPY(Class)      \
    Class(const Class &) = delete; \
    Class &operator=(const Class &) = delete;