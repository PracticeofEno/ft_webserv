#ifndef IOTHREAD_HPP
#define IOTHREAD_HPP

#include "Total.hpp"
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

class IOThread
{
    public:
        IOThread();
        ~IOThread();
        void setFunction(void *(*func)(void*));
        void start(void *response);
    private:
        pthread_t p_thread;
        void *(*_func)(void*);


};

#endif