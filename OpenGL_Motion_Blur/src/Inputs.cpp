#include "Inputs.h"

double Inputs::m_LastCursorPosX, Inputs::m_LastCursorPosY;
std::map<int, std::function<void(int)>> Inputs::m_KeyCallbucks;
std::function<void(double, double, double, double)> Inputs::m_CursorPosCallbuck;
std::map<int, std::function<void(int)>> Inputs::m_MouseButtonCallbucks;

Inputs::Inputs()
{
	m_Window = glfwGetCurrentContext();

	glfwGetCursorPos(m_Window, &m_LastCursorPosX, &m_LastCursorPosY);
	glfwSetKeyCallback(m_Window, keyCallback);
	glfwSetCursorPosCallback(m_Window, cursorPositionCallback);
	glfwSetMouseButtonCallback(m_Window, mouseButtonCallback);
}

Inputs::~Inputs()
{
	glfwSetKeyCallback(m_Window, NULL);
	glfwSetCursorPosCallback(m_Window, NULL);
	glfwSetMouseButtonCallback(m_Window, NULL);

	m_KeyCallbucks.clear();
	m_CursorPosCallbuck = NULL;
	m_MouseButtonCallbucks.clear();
}

void Inputs::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (m_KeyCallbucks[key] && action == GLFW_PRESS)
		m_KeyCallbucks[key](1);

	if (m_KeyCallbucks[key] && action == GLFW_RELEASE)
		m_KeyCallbucks[key](0);
}

void Inputs::cursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
{
	if (m_CursorPosCallbuck) m_CursorPosCallbuck(xpos, ypos, m_LastCursorPosX, m_LastCursorPosY);
	m_LastCursorPosX = xpos;
	m_LastCursorPosY = ypos;
}

void Inputs::setWSADKeyCallbacks(std::function<void(int)> wCallback, std::function<void(int)> sCallback, std::function<void(int)> aCallback, std::function<void(int)> dCallback)
{
	m_KeyCallbucks[GLFW_KEY_W] = wCallback;
	m_KeyCallbucks[GLFW_KEY_S] = sCallback;
	m_KeyCallbucks[GLFW_KEY_A] = aCallback;
	m_KeyCallbucks[GLFW_KEY_D] = dCallback;
}

void Inputs::setSpaceKeyCallback(std::function<void(int)> spaceCallback)
{
	m_KeyCallbucks[GLFW_KEY_SPACE] = spaceCallback;
}

void Inputs::setShiftKeyCallback(std::function<void(int)> shiftCallback)
{
	m_KeyCallbucks[GLFW_KEY_LEFT_SHIFT] = shiftCallback;
}

void Inputs::setCursorCullbuck(std::function<void(double, double, double, double)> c)
{
	m_CursorPosCallbuck = c;
}

void Inputs::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	if (m_MouseButtonCallbucks[button] && action == GLFW_PRESS)
		m_MouseButtonCallbucks[button](1);

	if (m_MouseButtonCallbucks[button] && action == GLFW_RELEASE)
		m_MouseButtonCallbucks[button](0);
}

void Inputs::setMouseButtonCallbacks(int button, std::function<void(int)> buttonCallback)
{
	m_MouseButtonCallbucks[button] = buttonCallback;
}