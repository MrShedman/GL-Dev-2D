#include "Angle.hpp"

#include <cmath>

namespace
{
	const float pi = 3.141592654f;
}

const Angle Angle::Zero;

Angle::Angle() :
m_radians(0)
{
}

float Angle::asDegrees() const
{
	return m_radians * 180 / ::pi;
}

float Angle::asRadians() const
{
	return m_radians;
}

Angle::Angle(float angle)
{
	// Normalize to [0, 360[
	angle = static_cast<float>(std::fmod(angle, 360));
	if (angle < 0)
		angle += 360;

	m_radians = angle * ::pi / 180;
}

Angle degrees(float amount)
{
	return Angle(amount);
}

Angle radians(float amount)
{
	return Angle(amount * 180 / ::pi);
}

bool operator ==(Angle left, Angle right)
{
	return left.asRadians() == right.asRadians();
}

bool operator !=(Angle left, Angle right)
{
	return left.asRadians() != right.asRadians();
}

bool operator <(Angle left, Angle right)
{
	return left.asRadians() < right.asRadians();
}

bool operator >(Angle left, Angle right)
{
	return left.asRadians() > right.asRadians();
}

bool operator <=(Angle left, Angle right)
{
	return left.asRadians() <= right.asRadians();
}

bool operator >=(Angle left, Angle right)
{
	return left.asRadians() >= right.asRadians();
}

Angle operator -(Angle right)
{
	return radians(-right.asRadians());
}

Angle operator +(Angle left, Angle right)
{
	return radians(left.asRadians() + right.asRadians());
}

Angle& operator +=(Angle& left, Angle right)
{
	return left = left + right;
}

Angle operator -(Angle left, Angle right)
{
	return radians(left.asRadians() - right.asRadians());
}

Angle& operator -=(Angle& left, Angle right)
{
	return left = left - right;
}

Angle operator *(Angle left, float right)
{
	return radians(left.asRadians() * right);
}

Angle operator *(float left, Angle right)
{
	return right * left;
}

Angle& operator *=(Angle& left, float right)
{
	return left = left * right;
}

Angle operator /(Angle left, float right)
{
	return radians(left.asRadians() / right);
}

Angle& operator /=(Angle& left, float right)
{
	return left = left / right;
}

float operator /(Angle left, Angle right)
{
	return left.asRadians() / right.asRadians();
}

Angle operator %(Angle left, Angle right)
{
	return radians(static_cast<float>(std::fmod(left.asRadians(), right.asRadians())));
}

Angle& operator %=(Angle& left, Angle right)
{
	return left = left % right;
}