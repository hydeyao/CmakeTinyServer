#ifndef LOCKER_H
#define LOCKER_H

#pragma once
#include <exception>
#include <pthread.h>
#include <semaphore.h>

class sem
{
public:
    sem();
    ~sem();
};




class locker
{
public:
    locker();
    ~locker();

private:

};

#endif