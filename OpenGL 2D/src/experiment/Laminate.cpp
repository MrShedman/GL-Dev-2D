#include "Laminate.h"

std::ostream& operator << (std::ostream& os, const Laminate::Properties& prop)
{
	os << std::fixed << std::setprecision(5);

	os << std::left << std::setfill(' ') << std::setw(5) << "Ex: " << prop.Ex << std::endl;
	os << std::left << std::setfill(' ') << std::setw(5) << "Ey: " << prop.Ey << std::endl;
	os << std::left << std::setfill(' ') << std::setw(5) << "Gxy: " << prop.Gxy << std::endl;
	os << std::left << std::setfill(' ') << std::setw(5) << "vxy: " << prop.vxy << std::endl;

	return os;
}