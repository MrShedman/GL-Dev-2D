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

	//// Do the math
	float temp1 = 2.0f * u.Dot(*this);
	Vector3f temp2 = u * temp1;
	
	float temp3 = s*s - u.LengthSq();
	Vector3f temp4 = *this * temp3;

	float temp5 = 2.0f * s;
	Vector3f temp6 = u.Cross(*this);

	return temp2 + temp4 + temp6;

	/*Quaternion w = rotation * (*this) * rotation.Conjugate();

	return Vector3f(w.x, w.y, w.z);*/
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