#include "Clock.h"
#include <windows.h>

namespace
{
	LARGE_INTEGER getFrequency()
	{
		LARGE_INTEGER frequency;
		QueryPerformanceFrequency(&frequency);
		return frequency;
	}

	static Time getCurrentTime()
	{
		// Force the following code to run on first core
		// (see http://msdn.microsoft.com/en-us/library/windows/desktop/ms644904(v=vs.85).aspx)
		HANDLE currentThread = GetCurrentThread();
		DWORD_PTR previousMask = SetThreadAffinityMask(currentThread, 1);

		// Get the frequency of the performance counter
		// (it is constant across the program lifetime)
		static LARGE_INTEGER frequency = getFrequency();

		// Get the current time
		LARGE_INTEGER time;
		QueryPerformanceCounter(&time);

		// Restore the thread affinity
		SetThreadAffinityMask(currentThread, previousMask);

		// Return the current time as microseconds
		return microseconds(1000000 * time.QuadPart / frequency.QuadPart);
	}

}

Clock::Clock() :
m_startTime(getCurrentTime())
{
}

Time Clock::getElapsedTime() const
{
	return getCurrentTime() - m_startTime;
}

Time Clock::restart()
{
	Time now = getCurrentTime();
	Time elapsed = now - m_startTime;
	m_startTime = now;

	return elapsed;
}