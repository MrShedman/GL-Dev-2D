#pragma once

class Range
{
public:
	Range()
		:
		Range(0.f, 0.1f, 1.f)	
	{}

	Range(float begin, float increment, float end) 
		: 
		begin(begin), 
		increment(increment),
		end(end)
	{}

	int getUpperLimit()
	{
		return static_cast<int>(std::round((end - begin) / increment));
	}

	float map(int value)
	{
		return (static_cast<float>(value) * increment) + begin;
	}

private:

	float begin;
	float increment;
	float end;
};