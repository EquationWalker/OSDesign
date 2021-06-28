#pragma once
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <cassert>
#include <pthread.h>
#include <sys/time.h>
#include <sched.h>
using std::printf;


#define L_DISABLE_COPY(Class)      \
    Class(const Class &) = delete; \
    Class &operator=(const Class &) = delete;