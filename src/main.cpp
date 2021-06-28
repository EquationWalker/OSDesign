#include "../include/LMutex.h"
#include "../include/LMutexLocker.h"
#include "../include/LWaitCondition.h"
#include "../include/LThread.h"
#include <queue>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <string>
#include <utility>
using std::ifstream;
using std::ofstream;
using std::pair;
using std::queue;
using std::string;
using std::vector;
struct CustomerInfo
{
    int id;
    string name;
};

int N = 0;                // 理发师数量
int M = 0;                // 顾客数量
int serverTime;           // 顾客理发时间
int remainingBarbers = 0; // 剩余理发师数量
int remainingSeats;       // 剩余座位数量

queue<CustomerInfo> haircutQue; // 顾客理发队列,理发师在这里取得理发客户
LWaitCondition haircutQueNotFull, haircutQueNotEmpty;
LWaitCondition seatNotFull;
LMutex mutex_haircut, mutex_seat;

class BarberThread : public LThread
{
public:
    BarberThread(int id, const char *name = "Tony")
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
        LMutexLocker lcoker(&mutex_haircut);
        printf("\t 理发师: %d %s 已经上线!\n", id, name);
        remainingBarbers++;
        lcoker.unlock();

        while (running)
        {
            haircutQueNotFull.wakeOne();
            //LThread::yieldCurrentThread();
            lcoker.relock();
            while (0 == haircutQue.size())
            {
                printf("\t 理发师: %d %s 进入睡眠!\n", id, name);
                haircutQueNotEmpty.wait(lcoker.mutex());
            }
            auto info = haircutQue.front();
            haircutQue.pop();
            printf("\t 理发师: %d %s 开始为 %d %s 顾客理发!\n",
                   id, name, info.id, info.name.c_str());
            sleep(serverTime);
            printf("\t 理发师: %d %s 已经为 %d %s 顾客理发完毕!\n",
                   id, name, info.id, info.name.c_str());
            remainingBarbers++;
            lcoker.unlock();
        }

        lcoker.relock();
        printf("\t 理发师: %d %s 下班啦!\n", id, name);
        remainingBarbers--;
    }

private:
    const char *name;
    int id;
    bool running = true;
};

class CustomerThread : public LThread
{
public:
    CustomerThread(int id, const char *name = "Sx")
    {
        this->id = id;
        this->name = name;
    }

protected:
    void run() noexcept override
    {
        printf("\t顾客: %d %s 开始申请座位!\n", id, name);
        mutex_seat.lock();
        while (0 == remainingSeats)
            seatNotFull.wait(&mutex_seat);
        remainingSeats--;
        printf("\t顾客: %d %s 已经申请到座位!\n", id, name);
        mutex_seat.unlock();

        printf("\t顾客: %d %s 开始申请理发师!\n", id, name);
        LMutexLocker locker(&mutex_haircut);
        while (0 == remainingBarbers)
            haircutQueNotFull.wait(locker.mutex());
        printf("\t顾客: %d %s 准备理发!\n", id, name);

        mutex_seat.lock();
        remainingSeats++;
        mutex_seat.unlock();
        seatNotFull.wakeOne();

        haircutQue.push({id, name});
        remainingBarbers--;
        printf("\t顾客: %d %s 已经做好理发准备!\n", id, name);
        haircutQueNotEmpty.wakeOne();
    }

private:
    int id;
    const char* name;
};

vector<BarberThread *> barber;
vector<CustomerThread *> customer;
char *configFile = "..//config//config.ini";

void loadConfig()
{
    printf("\t开始读取配置文件\n");
    string name;
    ifstream in(configFile);
    in >> name >> N >> name >> M;
    in >> name >> remainingSeats >> name >> serverTime;
    printf("\t配置文件读取结果:\n");
    printf("\t理发师线程数量: %d \t 顾客线程数量: %d\n", N, M);
    printf("\t理发店座位数量: %d \t 顾客服务时间: %d\n\n", remainingSeats, serverTime);
}

int main(int argc, char *argv[])
{
    if (argc >= 2)
        configFile = argv[1];
    loadConfig();
    for (int i = 0; i < N; i++)
    {
        barber.push_back(new BarberThread(i + 1));
        barber[i]->start();
        LThread::yieldCurrentThread();
    }

    for (int i = 0; i < M; i++)
    {
        customer.push_back(new CustomerThread(i + 1));
        customer[i]->start();
        LThread::yieldCurrentThread();
    }

    for (int i = 0; i < N; i++)
    {
        barber[i]->wait();
        delete barber[i];
    }

    for (int i = 0; i < M; i++)
    {
        customer[i]->wait();
        delete customer[i];
    }

    return 0;
}
