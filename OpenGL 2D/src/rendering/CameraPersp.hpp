#pragma once

#include "..\math\Matrix.h"
#include "..\math\MathHelper.h"
#include "Transform.hpp"
#include "..\window\Event.h"

class CameraPersp
{
public:
	void init(Window* w)
	{
		m_window = w;
		m_projection.InitPerspective(ToRadians(70.0f), w->getAspect(), 0.1f, 1000.0f);
	}

	void handleEvent(const Event& event)
	{
		if (event.type == Event::MouseButtonPressed)
		{
			if (event.mouseButton.button == Mouse::Left)
			{
				m_window->setRawMouseInput(true);
				m_window->setMouseCursorVisible(false);
				m_window->setClippedCursor(true);
			}
		}
		else if (event.type == Event::KeyPressed)
		{
			if (event.key.code == Keyboard::Escape || event.key.code == Keyboard::E)
			{
				m_window->setRawMouseInput(false);
				m_window->setMouseCursorVisible(true);
				m_window->setClippedCursor(false);
			}
		}
		else if (event.type == Event::MouseMotion)
		{
			m_transform.rotate(Vector3f(0, 1, 0), event.mouseMotion.dx * 0.005f);

			m_transform.rotate(m_transform.getRotation().GetRight(), event.mouseMotion.dy * 0.005f);
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

	void Move(const Vector3f& direction, float amt)
	{
		m_transform.move(direction * amt);
	}

	Vector3f getPosition() const
	{
		return m_transform.getPosition();
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

	Window*				m_window;

	mutable Transform	m_transform;
	mutable Matrix4f	m_projection;
};