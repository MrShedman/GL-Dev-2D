#include "Transform.hpp"
#include "..\math\MathHelper.h"

Transform::Transform() :
m_origin(0, 0, 0),
m_position(0, 0, 0),
m_rotation(),
m_scale(1, 1, 1),
m_transform(),
m_transformNeedUpdate(true),
m_inverseTransform(),
m_inverseTransformNeedUpdate(true)
{
}

Transform::~Transform()
{
}

void Transform::setPosition(float x, float y, float z)
{
	m_position.x = x;
	m_position.y = y;
	m_position.z = z;
	m_transformNeedUpdate = true;
	m_inverseTransformNeedUpdate = true;
}

void Transform::setPosition(const Vector3f& position)
{
	setPosition(position.x, position.y, position.z);
}

void Transform::setRotation(float angle)
{
	setRotation(m_origin, angle);
}

void Transform::setRotation(const Vector3f& axis, float angle)
{
	setRotation(Quaternion(axis, ToRadians(angle)));
}

void Transform::setRotation(const Quaternion& rotation)
{
	m_rotation = rotation;

	m_transformNeedUpdate = true;
	m_inverseTransformNeedUpdate = true;
}

void Transform::setScale(float factorX, float factorY, float factorZ)
{
	m_scale.x = factorX;
	m_scale.y = factorY;
	m_scale.z = factorZ;
	m_transformNeedUpdate = true;
	m_inverseTransformNeedUpdate = true;
}

void Transform::setScale(const Vector3f& factors)
{
	setScale(factors.x, factors.y, factors.z);
}

void Transform::setOrigin(float x, float y, float z)
{
	m_origin.x = x;
	m_origin.y = y;
	m_origin.z = z;
	m_transformNeedUpdate = true;
	m_inverseTransformNeedUpdate = true;
}

void Transform::setOrigin(const Vector3f& origin)
{
	setOrigin(origin.x, origin.y, origin.z);
}

const Vector3f& Transform::getPosition() const
{
	return m_position;
}

float Transform::get2DRotation() const
{
	Vector3f right = getRotation().GetRight();

	return ToDegrees(std::atan(right.y / right.x));
}

const Quaternion& Transform::getRotation() const
{
	return m_rotation;
}

const Vector3f& Transform::getScale() const
{
	return m_scale;
}

const Vector3f& Transform::getOrigin() const
{
	return m_origin;
}

void Transform::move(float offsetX, float offsetY, float offsetZ)
{
	setPosition(m_position.x + offsetX, m_position.y + offsetY, m_position.z + offsetZ);
}

void Transform::move(const Vector3f& offset)
{
	setPosition(m_position.x + offset.x, m_position.y + offset.y, m_position.z + offset.z);
}

void Transform::rotate(float angle)
{
	rotate(Vector3f(1, 0, 0), angle);
}

void Transform::rotate(const Vector3f& axis, float angle)
{
	rotate(Quaternion(axis, angle));
}

void Transform::rotate(const Vector3f& pivot, const Vector3f& axis, float angle)
{
	Quaternion rotation = Quaternion(axis, angle).Normalized();

	Vector3f vector = m_position - m_origin - pivot;

	setPosition(pivot + vector.Rotate(rotation) + m_origin);

	rotate(rotation);
}

void Transform::rotate(const Quaternion& rotation)
{
	setRotation(Quaternion(rotation * m_rotation).Normalized());
}

void Transform::scale(float factorX, float factorY, float factorZ)
{
	setScale(m_scale.x * factorX, m_scale.y * factorY, m_scale.z * factorZ);
}

void Transform::scale(const Vector3f& factor)
{
	setScale(m_scale.x * factor.x, m_scale.y * factor.y, m_scale.z * factor.z);
}

void Transform::update() const
{
	// Recompute the combined transform if needed
	if (m_transformNeedUpdate)
	{
		Matrix4f translationMatrix;
		Matrix4f scaleMatrix;

		translationMatrix.InitTranslation(m_position - m_origin);
		scaleMatrix.InitScale(m_scale);

		m_transform = translationMatrix * m_rotation.ToRotationMatrix() * scaleMatrix;

		m_transformNeedUpdate = false;
	}
}

const Matrix4f& Transform::getTransform() const
{
	update();

	return m_transform;
}

const Matrix4f& Transform::getInverseTransform() const
{
	// Recompute the inverse transform if needed
	if (m_inverseTransformNeedUpdate)
	{
		m_inverseTransform = getTransform().Inverse();
		m_inverseTransformNeedUpdate = false;
	}

	return m_inverseTransform;
}