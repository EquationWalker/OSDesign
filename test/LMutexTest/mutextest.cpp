#include "../../include/LMutex.h"
LMutex mutex;
using LL = long long;
void* func(void* arg)
{
    LL idx = (LL)arg;
    mutex.lock();
    printf("1: %lld\n", idx);
    printf("2: %lld\n", idx);
    mutex.unlock();

    return NULL;
}

int main()
{
    pthread_t a, b;
    pthread_create(&a, NULL, func, (void*)11);
    pthread_create(&b, NULL, func, (void*)22);

    pthread_join(a, NULL);
    pthread_join(b, NULL);

    return 0;
}