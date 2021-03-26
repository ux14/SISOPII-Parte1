#ifndef SAFEQUEUE_H
#define SAFEQUEUE_H

#include <queue>
#include <mutex>
#include <condition_variable>

using namespace std;

template <typename T>
class SafeQueue
{
private:
    queue<T> q;
    mutex mtx;
    condition_variable cv;

public:
    T pop()
    {
        unique_lock<mutex> lock(mtx);

        cv.wait(lock, [&] { return !q.empty(); });

        T value = q.front();
        q.pop();

        return value;
    };

    void push(T value)
    {
        unique_lock<mutex> lock(mtx);
        q.push(value);
        cv.notify_one();
    };
};

#endif