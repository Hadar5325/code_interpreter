//
// Created by linor on 09/01/2020.
//
/*
#include "SinletonMutex.h"
#include <iostream>


SinletonMutex* SinletonMutex::instance = 0;

SinletonMutex* SinletonMutex::getInstance()
{
    if (instance == 0)
    {
        instance = new SinletonMutex();
    }

    return instance;
}
void SinletonMutex::LockMutex(){
    pthread_mutex_lock(&this->lock);
}
void SinletonMutex::UnlockMutex(){
    pthread_mutex_unlock(&this->lock);
}

SinletonMutex::SinletonMutex()
{}
*/