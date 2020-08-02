#pragma once

#include "GLFW/glfw3.h"
#include "functional"
#include <map>

class Inputs
{
	GLFWwindow* m_Window;
	static double m_LastCursorPosX, m_LastCursorPosY;
	static std::map<int, std::function<void(int)>> m_KeyCallbucks;
	static std::function<void(double, double, double, double)> m_CursorPosCallbuck;
	static std::map<int, std::function<void(int)>> m_MouseButtonCallbucks;
public:
	Inputs();
	~Inputs();

	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos);
	static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

	void setWSADKeyCallbacks(std::function<void(int)> wCallback, std::function<void(int)> sCallback, std::function<void(int)> aCallback, std::function<void(int)> dCallback);
	void setSpaceKeyCallback(std::function<void(int)> spaceCallback);
	void setShiftKeyCallback(std::function<void(int)> shiftCallback);
	void setCursorCullbuck(std::function<void(double, double, double, double)> cursorCallback);
	void setMouseButtonCallbacks(int button, std::function<void(int)> buttonCallback);
};