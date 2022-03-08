#include "IOThread.hpp"

IOThread::IOThread() {}
IOThread::~IOThread() {}

void IOThread::setFunction(void *(*func)(void*))
{
    _func = func;
}

void IOThread::start(void *response)
{
    pthread_create(&(this->p_thread), NULL, _func, response);
}