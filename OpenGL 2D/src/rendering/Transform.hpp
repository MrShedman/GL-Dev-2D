#pragma once

#include "..\math\Vector.h"
#include "..\math\Matrix.h"
#include "..\math\Quaternion.h"

class Transform
{
public:

	Transform();

	virtual ~Transform();

	void setPosition(float x, float y, float z = 0);

	void setPosition(const Vector3f& position);

	void setRotation(float angle);

	void setRotation(const Vector3f& axis, float angle);

	void setRotation(const Quaternion& rotation);

	void setScale(float factorX, float factorY, float factorZ = 1);

	void setScale(const Vector3f& factors);

	void setOrigin(float x, float y, float z = 0);

	void setOrigin(const Vector3f& origin);

	const Vector3f& getPosition() const;

	float get2DRotation() const;

	const Quaternion& getRotation() const;

	const Vector3f& getScale() const;

	const Vector3f& getOrigin() const;

	void move(float offsetX, float offsetY, float offsetZ = 0);

	void move(const Vector3f& offset);

	void rotate(float angle);

	void rotate(const Vector3f& axis, float angle);

	void rotate(const Vector3f& pivot, const Vector3f& axis, float angle);

	void rotate(const Quaternion& rotation);

	void scale(float factorX, float factorY, float factorZ = 1);

	void scale(const Vector3f& factor);

	const Matrix4f& getTransform() const;

	const Matrix4f& getInverseTransform() const;

private:

	void update() const;

	Vector3f          m_origin;                     ///< Origin of translation/rotation/scaling of the object
	Vector3f          m_position;                   ///< Position of the object in the 2D world
	Quaternion		  m_rotation;					///< Orientation of the object
	Vector3f          m_scale;                      ///< Scale of the object
	mutable Matrix4f  m_transform;                  ///< Combined transformation of the object
	mutable bool      m_transformNeedUpdate;        ///< Does the transform need to be recomputed?
	mutable Matrix4f  m_inverseTransform;           ///< Combined transformation of the object
	mutable bool      m_inverseTransformNeedUpdate; ///< Does the transform need to be recomputed?
};