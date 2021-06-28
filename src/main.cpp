#include "../include/LMutex.h"
#include "../include/LMutexLocker.h"
#include "../include/LWaitCondition.h"
#include "../include/LThread.h"
#include <queue>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
using std::queue;
using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::ifstream;
using std::ofstream;
using std::vector;

int N = 0;
int M = 0;

queue<int> waitQue;
LWaitCondition waitQueNotFull, waitQueNotEmpty;
LMutex mutex_wait, mutex_baber;
int remainingBarbels = 0;
int maxSeatCnt = 5;
int serverTime = 1;

class BarbelThread : public LThread
{
public:
    BarbelThread(int id = -1, const char *name = "Tony")
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
        printf("\t %d 号理发师 %s 已经上线!\n", id, name);
        remainingBarbels++;
        lcoker.unlock();

        while (running)
        {
            LMutexLocker lcoker(&mutex_wait);
            while (0 == waitQue.size())
            {
                printf("\t %d 号理发师 %s 进入睡眠!\n", id, name);
                waitQueNotEmpty.wait(lcoker.mutex());
            }
            int cid = waitQue.front();
            waitQue.pop();
            printf("\t %d 号理发师 %s 开始为 %d 顾客理发!\n", id, name, cid);
            sleep(serverTime);
            printf("\t %d 号理发师 %s 为 %d 顾客理发完毕!\n", id, name, cid);
            waitQueNotFull.wakeOne();
        }

        lcoker.relock();
        printf("\t %d 号理发师 %s 已跑路!\n", id, name);
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
        waitQueNotEmpty.wakeOne();
    }

private:
    int id;
};

vector<BarbelThread*> barbel;
vector<CustomerThread*> customer;

void loadConfig()
{
    printf("\t开始读取配置文件\n");
    string name;
    ifstream in("../config/config.ini");
    in >> name >> N >> name >> M;
    in >> name >> maxSeatCnt >> name >> serverTime;
    printf("\t配置文件读取结果:\n");
    printf("\t理发师线程数量: %d \t 顾客线程数量: %d\n", N, M);
    printf("\t理发店座位数量: %d \t 顾客服务时间: %d\n\n", maxSeatCnt, serverTime);
}

int main()
{
    loadConfig();
    for (int i = 0; i < N; i++)
    {
        barbel.push_back(new BarbelThread(i + 1));
        barbel[i]->start();
        LThread::yieldCurrentThread();
    }

    for (int i = 0; i < M; i++)
    {
        customer.push_back(new CustomerThread(i));
        customer[i]->start();
        LThread::yieldCurrentThread();
    }

    //sleep(3);

    for (int i = 0; i < N; i++)
    {
        //barbel[i]->stop();
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
