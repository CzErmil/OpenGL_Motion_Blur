#pragma once

#include "GLFW/glfw3.h"
#include "glm/gtc/matrix_transform.hpp"

class Camera
{
private:
	GLFWwindow* m_Window;
	double* m_DeltaTime;
	int m_Width, m_Height;
	glm::mat4 m_Proj;
	float m_Fov, m_Aspect, m_Near, m_Far;
	short m_Forward, m_Backward, m_Left, m_Right, m_Up, m_Down;
	short m_MouseMiddleButton;
	glm::vec3 m_Position;
	glm::vec3 m_vFront, m_vUp, m_vRight, m_vWorldUp;
	float m_Yaw, m_Pitch;
	float m_MovementSpeed;
	float m_MouseSensitivity;
public:

	Camera(double* deltaTime);

	glm::mat4 getView();
	inline glm::mat4 getProj() { return m_Proj; }
	inline glm::vec3 getPosition() { return m_Position; }

	void updateCamera();

	void setForward(int v) { m_Forward = v; }
	void setBackward(int v) { m_Backward = v; }
	void setLeft(int v) { m_Left = v; }
	void setRight(int v) { m_Right = v; }
	void setUp(int v) { m_Up = v; }
	void setDown(int v) { m_Down = v; }
	void setMouseMiddleButton(int v) { m_MouseMiddleButton = v; }

	void processMouseMovment(double x, double y, double lastX, double lastY);
private:
	inline void setPosition(float x, float y, float z) { m_Position = glm::vec3(x, y, z); }
	inline void setProj(float fov, float aspect, float near, float far) { m_Proj = glm::perspective(fov, aspect, near, far); }

	void updateCameraVectors();
};