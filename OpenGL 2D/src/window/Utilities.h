#pragma once

#include <string>
#include <vector>
#include <codecvt>
#include "Time.h"
#include <iostream>
#include <windows.h>
#include <psapi.h>

namespace Util
{
	std::vector<std::string> Split(const std::string& s, char delim);

	inline std::wstring s2ws(const std::string& str)
	{
		typedef std::codecvt_utf8<wchar_t> convert_typeX;
		std::wstring_convert<convert_typeX, wchar_t> converterX;

		return converterX.from_bytes(str);
	}

	inline std::string ws2s(const std::wstring& wstr)
	{
		typedef std::codecvt_utf8<wchar_t> convert_typeX;
		std::wstring_convert<convert_typeX, wchar_t> converterX;

		return converterX.to_bytes(wstr);
	}

	inline void sleep(Time duration)
	{
		if (duration <= Time::Zero)
		{
			return;
		}

		// Get the supported timer resolutions on this system
		TIMECAPS tc;
		timeGetDevCaps(&tc, sizeof(TIMECAPS));

		// Set the timer resolution to the minimum for the Sleep call
		timeBeginPeriod(tc.wPeriodMin);

		::Sleep(duration.asMilliseconds());

		// Reset the timer resolution back to the system default
		timeEndPeriod(tc.wPeriodMin);
	}
}