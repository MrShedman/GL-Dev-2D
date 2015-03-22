#pragma once

#include "..\math\Matrix.h"
#include "..\math\MathHelper.h"
#include "..\math\Angle.hpp"
#include "Transform.hpp"
#include "..\window\Event.h"
#include "Camera.hpp"

class CameraPersp : public CameraDerived
{
public:
	void init(Window* w)
	{
		mouseClipped = false;

		m_fov = degrees(70.0f);
		m_aspect = w->getAspect();
		m_nearPlaneDistance = 0.1f;
		m_farPlaneDistance = 1000.0f;

		m_window = w;
		m_projection.InitPerspective(m_fov.asRadians(), m_aspect, m_nearPlaneDistance, m_farPlaneDistance);
	}

	void handleEvent(const Event& event)
	{
		if (event.type == Event::MouseButtonPressed)
		{
			if (event.mouseButton.button == Mouse::Left)
			{
				mouseClipped = true;

				m_window->setRawMouseInput(true);
				m_window->setMouseCursorVisible(false);
				m_window->setClippedCursor(mouseClipped);
			}
		}
		else if (event.type == Event::KeyPressed)
		{
			if (event.key.code == Keyboard::Escape || event.key.code == Keyboard::E)
			{
				mouseClipped = false;

				m_window->setRawMouseInput(false);
				m_window->setMouseCursorVisible(true);
				m_window->setClippedCursor(mouseClipped);

				Mouse::setPosition(m_window->getCentre(), *m_window);
			}
		}
		else if (event.type == Event::MouseMotion)
		{
			m_transform.rotate(Vector3f(0, 1, 0), radians(event.mouseMotion.dx * 0.005f));

			m_transform.rotate(m_transform.getRotation().GetRight(), radians(event.mouseMotion.dy * 0.005f));
		}
	}

	void Update(float delta)
	{
		float mult = 1.0f;

		if (Keyboard::isKeyPressed(Keyboard::LShift))
		{
			mult = 3.0f;
		}
		if (Keyboard::isKeyPressed(Keyboard::LControl))
		{
			mult = 0.33f;
		}
		float m_speed = 12.0f;
		float movAmt = m_speed * delta * mult;

		if (Keyboard::isKeyPressed(Keyboard::Space))
		{
			Move(Vector3f(0, 1, 0), movAmt);
		}
		if (Keyboard::isKeyPressed(Keyboard::W))
		{
			Move(m_transform.getRotation().GetForward(), movAmt);
		}
		if (Keyboard::isKeyPressed(Keyboard::S))
		{
			Move(m_transform.getRotation().GetBack(), movAmt);
		}
		if (Keyboard::isKeyPressed(Keyboard::A))
		{
			Move(m_transform.getRotation().GetLeft(), movAmt);
		}
		if (Keyboard::isKeyPressed(Keyboard::D))
		{
			Move(m_transform.getRotation().GetRight(), movAmt);
		}
	}

	bool isEngaged() const
	{
		return mouseClipped;
	}

	void Move(const Vector3f& direction, float amt)
	{
		m_transform.move(direction * amt);
	}

	Vector3f getPosition() const
	{
		return m_transform.getPosition();
	}

	Matrix4f getTransform() const
	{
		return m_transform.getTransform();
	}

	Vector3f getLookAt() const
	{
		Matrix4f mat = getTransform();
		return Vector3f(mat[2][0], mat[2][1], mat[2][2]);
		//return m_transform.getRotation().GetForward();
	}

	Quaternion getRotation() const
	{
		return m_transform.getRotation();
	}

	float getNearClippingDistance() const
	{
		return m_nearPlaneDistance;
	}

	float getFarClippingDistance() const
	{
		return m_farPlaneDistance;
	}

	Angle getFOV() const
	{
		return m_fov;
	}

	float getAspectRatio() const
	{
		return m_aspect;
	}
	
	Matrix4f getProjection() const
	{
		Matrix4f cameraRotation = m_transform.getRotation().Conjugate().ToRotationMatrix();

		//Similarly, the translation is inverted because the world appears to move opposite
		//to the camera's movement.
		Matrix4f cameraTranslation = Matrix4f().InitTranslation(m_transform.getPosition().inverted());

		return m_projection * cameraRotation * cameraTranslation;
	}


private:

	bool mouseClipped;

	Angle m_fov;
	float m_aspect;
	float m_nearPlaneDistance;
	float m_farPlaneDistance;

	Window*				m_window;

	mutable Transform	m_transform;
	mutable Matrix4f	m_projection;
};