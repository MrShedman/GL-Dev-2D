#pragma once

#include "NonCopyable.hpp"

class Mutex;

class Lock : NonCopyable
{
public:

    explicit Lock(Mutex& mutex);

    ~Lock();

private:

    Mutex& m_mutex;
};