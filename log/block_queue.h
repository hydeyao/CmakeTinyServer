#ifndef BLOCK_QUEUE_H
#define BLOCK_QUEUE_H

#pragma once

#include <iostream>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include "../lock/locker.h"

using namespace std;

template <class T>
class block_queue
{
private:
    locker m_mutex;
    cond m_cond;

    T *m_array;
    int m_size;
    int m_max_size;
    int m_front;
    int m_back;

public:
    block_queue(int max_size = 1000);
    ~block_queue();
    void clear();
    bool full();
    bool empty();
    bool front(T &value);
    bool back(T &value);
    int size();
    int max_size();
    bool push(const T &item);
    bool pop(const T &item);
    //handle if timeout
    bool pop(const T &item,int ms_timeout);
};

#endif