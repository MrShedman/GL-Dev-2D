#include "Vector.h"
#include "Quaternion.h"

#include <iostream>
#include <iomanip>

template<>
Vector3f Vector3f::Rotate(const Quaternion& rotation) const
{
	Vector3f u(rotation.x, rotation.y, rotation.z);

	//// Extract the scalar part of the quaternion
	float s = rotation.w;

	Vector3f t = u.Cross(*this) * 2.0f;
	return *this + t * s + u.Cross(t);
}

std::ostream& operator << (std::ostream& os, const Vector3f& v)
{
	std::cout << std::fixed << std::setprecision(5);
	std::cout << "(";

	std::cout << std::left << std::setfill(' ') << std::setw(10) << v.x << ", ";
	std::cout << std::left << std::setfill(' ') << std::setw(10) << v.y << ", ";
	std::cout << std::left << std::setfill(' ') << std::setw(10) << v.z;

	std::cout << ")" << std::endl;

	return os;
}