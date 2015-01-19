#pragma once

#include "..\math\Matrix.h"
#include "..\math\Rect.h"
#include <iostream>
class Camera
{
public:

	Camera(const RectF& bounds = RectF());

	Camera(const Vector2f& center, const Vector2f& size);

	void setCenter(float x, float y);

	void setCenter(const Vector2f& center);

	void setSize(float width, float height);

	void setSize(const Vector2f& size);

	void setRotation(float angle);

	void setViewport(const RectF& viewport);

	void reset(const RectF& rectangle);

	const Vector2f& getCenter() const;

	const Vector2f& getSize() const;

	float getRotation() const;

	const RectF& getViewport() const;

	void move(float offsetX, float offsetY);

	void move(const Vector2f& offset);

	void rotate(float angle);

	void zoom(float factor);

	const Matrix4f& getTransform() const;

	const Matrix4f& getInverseTransform() const;

private:

	Vector2f			m_center;              ///< Center of the view, in scene coordinates
	Vector2f			m_size;                ///< Size of the view, in scene coordinates
	float				m_rotation;            ///< Angle of rotation of the view rectangle, in degrees
	RectF				m_viewport;            ///< Viewport rectangle, expressed as a factor of the render-target's size
	mutable Matrix4f	m_transform;           ///< Precomputed projection transform corresponding to the view
	mutable Matrix4f	m_inverseTransform;    ///< Precomputed inverse projection transform corresponding to the view
	mutable bool		m_transformUpdated;    ///< Internal state telling if the transform needs to be updated
	mutable bool		m_invTransformUpdated;
};