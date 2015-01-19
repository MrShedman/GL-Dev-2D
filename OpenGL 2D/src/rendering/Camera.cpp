#include "Camera.hpp"

Camera::Camera(const RectF& bounds) 
:
m_center(),
m_size(),
m_rotation(0),
m_viewport(0, 1, 0, 1),
m_transformUpdated(false),
m_invTransformUpdated(false)
{
	reset(bounds);
}

Camera::Camera(const Vector2f& center, const Vector2f& size) 
:
m_center(center),
m_size(size),
m_rotation(0),
m_viewport(0, 1, 0, 1),
m_transformUpdated(false),
m_invTransformUpdated(false)
{

}

void Camera::setCenter(float x, float y)
{
	m_center.x = x;
	m_center.y = y;

	m_transformUpdated = false;
	m_invTransformUpdated = false;
}

void Camera::setCenter(const Vector2f& center)
{
	setCenter(center.x, center.y);
}

void Camera::setSize(float width, float height)
{
	m_size.x = width;
	m_size.y = height;

	m_transformUpdated = false;
	m_invTransformUpdated = false;
}

void Camera::setSize(const Vector2f& size)
{
	setSize(size.x, size.y);
}

void Camera::setRotation(float angle)
{
	m_rotation = static_cast<float>(fmod(angle, 360));
	if (m_rotation < 0)
		m_rotation += 360.f;

	m_transformUpdated = false;
	m_invTransformUpdated = false;
}

void Camera::setViewport(const RectF& viewport)
{
	m_viewport = viewport;
}

void Camera::reset(const RectF& rectangle)
{
	m_center = rectangle.getCenter();
	m_size.x = rectangle.getWidth();
	m_size.y = rectangle.getHeight();
	m_rotation = 0;

	m_transformUpdated = false;
	m_invTransformUpdated = false;
}

const Vector2f& Camera::getCenter() const
{
	return m_center;
}

const Vector2f& Camera::getSize() const
{
	return m_size;
}

float Camera::getRotation() const
{
	return m_rotation;
}

const RectF& Camera::getViewport() const
{
	return m_viewport;
}

void Camera::move(float offsetX, float offsetY)
{
	setCenter(m_center.x + offsetX, m_center.y + offsetY);
}

void Camera::move(const Vector2f& offset)
{
	setCenter(m_center + offset);
}

void Camera::rotate(float angle)
{
	setRotation(m_rotation + angle);
}

void Camera::zoom(float factor)
{
	setSize(m_size.x * factor, m_size.y * factor);
}

const Matrix4f& Camera::getTransform() const
{
	// Recompute the matrix if needed
	if (!m_transformUpdated)
	{
		// Rotation components
		float angle = m_rotation * 3.141592654f / 180.f;
		float cosine = static_cast<float>(std::cos(angle));
		float sine = static_cast<float>(std::sin(angle));
		float tx = -m_center.x * cosine - m_center.y * sine + m_center.x;
		float ty = m_center.x * sine - m_center.y * cosine + m_center.y;

		// Projection components
		float a = 2.f / m_size.x;
		float b = -2.f / m_size.y;
		float c = -a * m_center.x;
		float d = -b * m_center.y;

		// Rebuild the projection matrix
		m_transform = Matrix4f(a * cosine, a * sine, 0.f, a * tx + c,
			-b * sine, b * cosine, 0.f, b * ty + d,
			0.f, 0.f, 1.f, 0.f,
			0.f, 0.f, 0.f, 1.f);
		m_transformUpdated = true;
	}

	return m_transform;
}

const Matrix4f& Camera::getInverseTransform() const
{
	// Recompute the matrix if needed
	if (!m_invTransformUpdated)
	{
		m_inverseTransform = getTransform().Inverse();
		m_invTransformUpdated = true;
	}

	return m_inverseTransform;
}