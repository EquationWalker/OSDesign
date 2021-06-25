#include "../include/LMutex.h"

LMutex::LMutex(RecursiveMode mode)
{
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    if (mode == Recursive)
        pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&mutex, &attr);
}
LMutex::~LMutex()
{
    pthread_mutex_destroy(&mutex);
}
void LMutex::lock()
{
    pthread_mutex_lock(&mutex);
}

bool LMutex::tryLock()
{
    return pthread_mutex_trylock(&mutex) == 0;
}

void LMutex::unlock()
{
    pthread_mutex_unlock(&mutex);
}