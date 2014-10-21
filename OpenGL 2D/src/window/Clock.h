#pragma once

#include "Time.h"

class Clock
{
public:

	Clock();

	Time getElapsedTime() const;

	Time restart();

private:

	Time m_startTime;
};