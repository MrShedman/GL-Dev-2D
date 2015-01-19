#include "Quaternion.h"

#include <iostream>
#include <iomanip>

std::ostream& operator << (std::ostream& os, const Quaternion& q)
{
	std::cout << std::fixed << std::setprecision(5);
	std::cout << "(";

	std::cout << std::left << std::setfill(' ') << std::setw(10) << q.x;
	std::cout << std::left << std::setfill(' ') << std::setw(10) << q.y;
	std::cout << std::left << std::setfill(' ') << std::setw(10) << q.z;
	std::cout << std::left << std::setfill(' ') << std::setw(10) << q.w;

	std::cout << ")" << std::endl;

	return os;
}