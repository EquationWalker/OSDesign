#include "../include/LMutex.h"
#include "../include/LMutexLocker.h"
#include "../include/LWaitCondition.h"
#include "../include/LThread.h"
#include <queue>
using std::queue;

const int N = 3;
const int M = 8;

queue<int> waitQue;
LWaitCondition waitQueNotFull, waitQueNotEmpty;
//LWaitCondition barbelNotEmpty;
LMutex mutex_wait, mutex_baber;
int remainingBarbels = 0;
int maxSeatCnt = 5;
int serverTime = 1;

class BarbelThread : public LThread
{
public:
    BarbelThread(int id, const char *name = "Tony")
    {
        this->id = id;
        this->name = name;
    }

    void stop()
    {
        running = false;
    }

protected:
    void run() noexcept override
    {
        LMutexLocker lcoker(&mutex_baber);
        printf("\t第 %d 号理发师 %s 已经上线!\n", id, name);
        remainingBarbels++;
        lcoker.unlock();

        while (running)
        {
            // barbelNotEmpty.wakeOne();
            LMutexLocker lcoker(&mutex_wait);
            while (0 == waitQue.size())
            {
                printf("\t第 %d 号理发师 %s 进入睡眠!\n", id, name);
                waitQueNotEmpty.wait(lcoker.mutex());
            }
            int cid = waitQue.front();
            waitQue.pop();
            printf("\t第 %d 号理发师 %s 开始为 %d 顾客理发!\n", id, name, cid);
            sleep(serverTime);
            printf("\t第 %d 号理发师 %s 为 %d 顾客理发完毕!\n", id, name, cid);
            waitQueNotFull.wakeOne();
        }

        lcoker.relock();
        printf("\t第 %d 号理发师 %s 已经辞职!\n", id, name);
        remainingBarbels--;
    }

private:
    const char *name;
    int id;
    bool running = true;
};

class CustomerThread : public LThread
{
public:
    CustomerThread(int id)
    {
        this->id = id;
    }

protected:
    void run() noexcept override
    {
        printf("\t顾客: %d 开始申请座位!\n", id);
        LMutexLocker locker(&mutex_wait);
        while (maxSeatCnt == waitQue.size())
            waitQueNotFull.wait(locker.mutex());
        printf("\t顾客: %d 已经申请到座位!\n", id);
        waitQue.push(id);
        //locker.unlock();
        waitQueNotEmpty.wakeOne();

        /*
        printf("\t顾客: %d 开始申请Tony老师!\n", id);
        mutex_baber.lock();
        while (0 == remainingBarbels)
            barbelNotEmpty.wait(&mutex_baber);
        remainingBarbels--;
        printf("\t顾客: %d 开始理发!\n", id);
        sleep(1);*/
    }

private:
    int id;
};
BarbelThread *barbel[N];
CustomerThread *customer[M];

int main()
{
    for (int i = 0; i < N; i++)
    {
        barbel[i] = new BarbelThread(i + 1);
        barbel[i]->start();
        LThread::yieldCurrentThread();
    }

    for (int i = 0; i < M; i++)
    {
        customer[i] = new CustomerThread(i);
        customer[i]->start();
        LThread::yieldCurrentThread();
    }

    sleep(3);

    for (int i = 0; i < N; i++)
    {
        barbel[i]->wait();
        delete barbel[i];
    }

    for (int i = 0; i < M; i++)
    {
        customer[i]->wait();
        delete customer[i];
    }


    return 0;
}
