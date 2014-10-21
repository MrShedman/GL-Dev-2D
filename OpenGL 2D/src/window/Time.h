#pragma once

class  Time
{
public:

	Time();

	float asSeconds() const;

	int asMilliseconds() const;

	long long asMicroseconds() const;

	static const Time Zero;

private:

	friend Time seconds(float);
	friend Time milliseconds(int);
	friend Time microseconds(long long);

	explicit Time(long long microseconds);

private:

	long long m_microseconds;
};

Time seconds(float amount);

Time milliseconds(int amount);

Time microseconds(long long amount);

bool operator ==(Time left, Time right);

bool operator !=(Time left, Time right);

bool operator <(Time left, Time right);

bool operator >(Time left, Time right);

bool operator <=(Time left, Time right);

bool operator >=(Time left, Time right);

Time operator -(Time right);

Time operator +(Time left, Time right);

Time& operator +=(Time& left, Time right);

Time operator -(Time left, Time right);

Time& operator -=(Time& left, Time right);

Time operator *(Time left, float right);

Time operator *(Time left, long long right);

Time operator *(float left, Time right);

Time operator *(long long left, Time right);

Time& operator *=(Time& left, float right);

Time& operator *=(Time& left, long long right);

Time operator /(Time left, float right);

Time operator /(Time left, long long right);

Time& operator /=(Time& left, float right);

Time& operator /=(Time& left, long long right);

float operator /(Time left, Time right);

Time operator %(Time left, Time right);

Time& operator %=(Time& left, Time right);