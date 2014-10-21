#include "Matrix.h"

#include <iostream>
#include <iomanip>

std::ostream& operator << (std::ostream& os, const Matrix4f& m)
{
	for (int i = 0; i < 4; ++i)
	{
		std::cout  << std::fixed << std::setprecision(5);
		std::cout << "[";
		for (int j = 0; j < 3; ++j)
		{
			std::cout << std::left << std::setfill(' ') << std::setw(10) << m[i][j];
		}
		std::cout << m[i][3] << "]" << std::endl;
	}

	return os;
}