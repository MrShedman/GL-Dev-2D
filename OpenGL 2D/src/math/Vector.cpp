#include "Vector.h"
#include "Quaternion.h"

template<>
Vector3f Vector3f::Rotate(const Quaternion& rotation) const
{
	Quaternion w = rotation * (*this) * rotation.Conjugate();

	return Vector3f(w.x, w.y, w.z);
}