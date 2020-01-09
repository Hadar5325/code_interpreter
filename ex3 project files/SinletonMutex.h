//
// Created by linor on 09/01/2020.
//
/*
#ifndef EX3_SINLETONMUTEX_H
#define EX3_SINLETONMUTEX_H

#include "pthread.h"

class SinletonMutex
{
private:

    static SinletonMutex* instance;
     pthread_mutex_t lock;

    SinletonMutex();

public:

    static SinletonMutex* getInstance();

    void LockMutex();

    void UnlockMutex();
};

#endif //EX3_SINLETONMUTEX_H

*/