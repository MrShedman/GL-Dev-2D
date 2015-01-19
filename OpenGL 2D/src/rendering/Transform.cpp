#include "Transform.hpp"
#include "..\math\MathHelper.h"

Transform::Transform() :
m_origin(0, 0, 1),
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
	rotate(m_origin, angle);
}

void Transform::rotate(const Vector3f& axis, float angle)
{
	rotate(Quaternion(axis, angle));
}

void Transform::rotate(const Quaternion& rotation)
{
	setRotation(Quaternion((rotation * m_rotation).Normalized()));
}

void Transform::scale(float factorX, float factorY, float factorZ)
{
	setScale(m_scale.x * factorX, m_scale.y * factorY, m_scale.z * factorZ);
}

void Transform::scale(const Vector3f& factor)
{
	setScale(m_scale.x * factor.x, m_scale.y * factor.y, m_scale.z * factor.z);
}

const Matrix4f& Transform::getTransform() const
{
	// Recompute the combined transform if needed
	if (m_transformNeedUpdate)
	{
		Matrix4f translationMatrix;
		Matrix4f scaleMatrix;

		translationMatrix.InitTranslation(m_position);
		scaleMatrix.InitScale(m_scale);

		m_transform = translationMatrix * m_rotation.ToRotationMatrix() * scaleMatrix;


		/*float angle = -m_rotation * 3.141592654f / 180.f;
		float cosine = static_cast<float>(std::cos(angle));
		float sine = static_cast<float>(std::sin(angle));
		float sxc = m_scale.x * cosine;
		float syc = m_scale.y * cosine;
		float sxs = m_scale.x * sine;
		float sys = m_scale.y * sine;
		float tx = -m_origin.x * sxc - m_origin.y * sys + m_position.x;
		float ty = m_origin.x * sxs - m_origin.y * syc + m_position.y;

		m_transform = Matrix4f(sxc, sys, 0.f, tx,
			-sxs, syc, 0.f, ty,
			0.f, 0.f, 1.f, 0.f,
			0.f, 0.f, 0.f, 1.f);*/
		m_transformNeedUpdate = false;
	}

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