#pragma once

class Angle
{
public:

	Angle();

	Angle(float angle);

	float asDegrees() const;

	float asRadians() const;

	static const Angle Zero; ///< Predefined 0 degree angle value

private:

	friend Angle degrees(float);
	friend Angle radians(float);

private:

	float m_radians; ///< Angle value stored as radians
};

Angle degrees(float angle);

Angle radians(float angle);

bool operator ==(Angle left, Angle right);

bool operator !=(Angle left, Angle right);

bool operator <(Angle left, Angle right);

bool operator >(Angle left, Angle right);

bool operator <=(Angle left, Angle right);

bool operator >=(Angle left, Angle right);

Angle operator -(Angle right);

Angle operator +(Angle left, Angle right);

Angle& operator +=(Angle& left, Angle right);

Angle operator -(Angle left, Angle right);

Angle& operator -=(Angle& left, Angle right);

Angle operator *(Angle left, float right);

Angle operator *(float left, Angle right);

Angle& operator *=(Angle& left, float right);

Angle operator /(Angle left, float right);

Angle& operator /=(Angle& left, float right);

float operator /(Angle left, Angle right);

Angle operator %(Angle left, Angle right);

Angle& operator %=(Angle& left, Angle right);