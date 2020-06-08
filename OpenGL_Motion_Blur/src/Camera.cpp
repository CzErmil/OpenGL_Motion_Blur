#include "Camera.h"

Camera::Camera(float* deltaTime)
	:m_Fov(45), m_Near(0.1), m_Far(100),
	m_Position(0, 0, 5),
	m_vWorldUp(0, 1, 0),
	m_Yaw(-90.0f), m_Pitch(0.0f),
	m_MovementSpeed(2.5f), m_MouseSensitivity(0.3f)
{
	m_Window = glfwGetCurrentContext();
	glfwGetWindowSize(m_Window, &m_Width, &m_Height);
	m_Aspect = (float)m_Width / m_Height;

	setProj(m_Fov, m_Aspect, m_Near, m_Far);

	m_DeltaTime = deltaTime;

	updateCameraVectors();
}

void Camera::updateCamera()
{
	float velocity = m_MovementSpeed * *m_DeltaTime;
	if (m_Forward)
		m_Position += m_vFront * velocity;

	if (m_Backward)
		m_Position -= m_vFront * velocity;

	if (m_Left)
		m_Position -= m_vRight * velocity;

	if (m_Right)
		m_Position += m_vRight * velocity;
}

void Camera::processMouseMovment(double x, double y, double lastX, double lastY)
{
	if (m_MouseMiddleButton)
	{
		double xOffset = (x - lastX) * m_MouseSensitivity;
		double yOffset = (y - lastY) * m_MouseSensitivity;

		m_Yaw += xOffset;
		m_Pitch += yOffset;

		if (m_Pitch > 89.0f)
			m_Pitch = 89.0f;

		if (m_Pitch < -89.0f)
			m_Pitch = -89.0f;

		updateCameraVectors();
	}
}

glm::mat4 Camera::getView()
{
	updateCamera();
	return glm::lookAt(m_Position, m_Position + m_vFront, m_vUp);
}

void Camera::updateCameraVectors()
{
	glm::vec3 front;
	front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	front.y = sin(glm::radians(m_Pitch));
	front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	m_vFront = glm::normalize(front);
	m_vRight = glm::normalize(glm::cross(m_vFront, m_vWorldUp));
	m_vUp = glm::normalize(glm::cross(m_vRight, m_vFront));
}