#include "Vector.h"
#include "Quaternion.h"

#include <iostream>
#include <iomanip>

template<>
Vector3f Vector3f::Rotate(const Quaternion& rotation) const
{
	Quaternion w = rotation * (*this) * rotation.Conjugate();

	return Vector3f(w.x, w.y, w.z);
}

std::ostream& operator << (std::ostream& os, const Vector3f& v)
{
	std::cout << std::fixed << std::setprecision(5);
	std::cout << "(";

	std::cout << std::left << std::setfill(' ') << std::setw(10) << v.x;
	std::cout << std::left << std::setfill(' ') << std::setw(10) << v.y;
	std::cout << std::left << std::setfill(' ') << std::setw(10) << v.z;

	std::cout << ")" << std::endl;

	return os;
}