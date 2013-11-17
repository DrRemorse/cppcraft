#include "input.hpp"

#include "../log.hpp"
#include "../math/toolbox.hpp"
#include "window.hpp"
#include <GL/glfw3.h>
#include <cmath>

namespace library
{
	Input input;
	
	void keyboard(GLFWwindow* window, int key, int action, int a, int b);
	void keyboardType(GLFWwindow* window, unsigned int character);
	void mouseMove(GLFWwindow* window, double x, double y);
	void mouseButton(GLFWwindow* window, int button, int action, int mods);
	void mouseWheel(GLFWwindow* window, double x, double y);
	
	void Input::init(WindowClass& gamescr)
	{
		// set owning window
		this->gamescr  = &gamescr;
		this->speed    = 0.15;
		this->sensitivity = 8;
		this->lastmx   = gamescr.SW / 2;
		this->lastmy   = gamescr.SH / 2;
		// default rotation
		this->rotation = vec2(0.0);
		
		// hook keyboard events
		glfwSetKeyCallback (gamescr.window(), &keyboard);
		glfwSetCharCallback(gamescr.window(), &keyboardType);
		// disable sticky keys
		glfwSetInputMode(gamescr.window(), GLFW_STICKY_KEYS, GL_TRUE);
		
		// hook mouse events
		glfwSetCursorPosCallback(gamescr.window(), &mouseMove);
		// disable cursor
		glfwSetInputMode(gamescr.window(), GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		// cursor moved event
		glfwSetCursorPos(gamescr.window(), this->lastmx, this->lastmy);
		// mouse button event
		glfwSetMouseButtonCallback(gamescr.window(), &mouseButton);
		// mouse wheel event
		glfwSetScrollCallback(gamescr.window(), &mouseWheel);
	}
	
	const Input::input_t& Input::getKeyEx(int key) const
	{
		if (key < 0 || key >= MAX_KEYS)
			throw std::string("Input::getKey(): Invalid key value");
		
		return keys[key];
	}
	
	Input::key_t Input::getKey(int key) const
	{
		return getKeyEx(key).action;
	}
	
	void Input::hold(int key)
	{
		keys[key].action = KEY_LOCKED;
	}
	void Input::release(int key)
	{
		if (keys[key].action == KEY_LOCKED)
			keys[key].action = KEY_PRESSED;
	}
	
	
	const Input::input_t& Input::getMouseEx(int button) const
	{
		return mouse[button];
	}
	Input::key_t Input::getMouse(int button) const
	{
		return getMouseEx(button).action;
	}
	void  Input::holdMouse(int button)
	{
		mouse[button].action = KEY_LOCKED;
	}
	
	int  Input::getWheel()
	{
		int wheel = this->wheel;
		this->wheel = 0;
		return wheel;
	}
	
	void keyboard(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		// action: GLFW_PRESS or GLFW_RELEASE
		input.keys[key].action = (action == GLFW_PRESS || action == GLFW_REPEAT) ? Input::KEY_PRESSED : Input::KEY_RELEASED;
		input.keys[key].mods = mods;
	}
	
	void keyboardType(GLFWwindow* window, unsigned int character)
	{
		// typing!
		input.text += wchar_t(character);
	}
	
	void mouseMove(GLFWwindow* window, double x, double y)
	{
		const double PI = 4 * atan(1);
		const double degToRad = PI / 180;
		const double maxX = 89 * degToRad;
		const double maxY = PI * 2.0;
		
		// in-game
		double dx = (x - input.lastmx) * input.speed;
		double dy = (y - input.lastmy) * input.speed;
		
		if (fabs(dx) > input.sensitivity)
			dx = toolbox::signum(dx) * input.sensitivity + dx / input.sensitivity;
		if (fabs(dy) > input.sensitivity)
			dy = toolbox::signum(dy) * input.sensitivity + dy / input.sensitivity;
		
		// rotation on axes
		input.rotation.y += dx * degToRad;
		input.rotation.x += dy * degToRad;
		// clamping
		if (input.rotation.x >  maxX) input.rotation.x =  maxX;
		if (input.rotation.x < -maxX) input.rotation.x = -maxX;
		if (input.rotation.y <     0) input.rotation.y += PI * 2;
		if (input.rotation.y >= maxY) input.rotation.y -= PI * 2;
		
		// move mouse to center
		input.lastmx = input.gamescr->SW / 2;
		input.lastmy = input.gamescr->SH / 2;
		glfwSetCursorPos(input.gamescr->window(), input.lastmx, input.lastmy);
	}
	
	void mouseButton(GLFWwindow* window, int button, int action, int mods)
	{
		if (button == GLFW_MOUSE_BUTTON_1) // left mouse button
		{
			input.mouse[0].action = (action == GLFW_PRESS) ? Input::KEY_PRESSED : Input::KEY_RELEASED;
			input.mouse[0].mods = mods;
		}
		else if (button == GLFW_MOUSE_BUTTON_2)
		{
			input.mouse[1].action = (action == GLFW_PRESS) ? Input::KEY_PRESSED : Input::KEY_RELEASED;
			input.mouse[1].mods = mods;
		}
		else if (button == GLFW_MOUSE_BUTTON_3)
		{
			input.mouse[2].action = (action == GLFW_PRESS) ? Input::KEY_PRESSED : Input::KEY_RELEASED;
			input.mouse[2].mods = mods;
		}
	}
	
	void mouseWheel(GLFWwindow* window, double x, double y)
	{
		if (y < 0)      input.wheel -= 1;
		else if (y > 0) input.wheel += 1;
		else            input.wheel  = 0;
	}
	
}
