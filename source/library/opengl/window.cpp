#include "window.hpp"

#include "../log.hpp"
#include <GL/glfw3.h>

namespace library
{
	void WindowClass::open(WindowConfig& wndconf)
	{
		if (this->init == false)
		{
			glfwInit();
			init = true;
		}
		
		// renderer initialization settings
		this->SW = wndconf.SW;
		this->SH = wndconf.SH;
		this->SA = (float)this->SW / (float)this->SH;
		
		// fullscreen (boolean)
		bool fullscr = wndconf.fullscreen;
		
		GLFWmonitor* monitor = nullptr;
		// set primary monitor if we are to run in fullscreen
		if (fullscr) monitor = glfwGetPrimaryMonitor();
		
		glfwWindowHint(GLFW_REFRESH_RATE, wndconf.refreshrate);
		glfwWindowHint(GLFW_RESIZABLE, 0);
		glfwWindowHint(GLFW_SAMPLES, wndconf.multisample);
		
		glfwWindowHint(GLFW_ALPHA_BITS, 8);
		
		// create new glfw3 window
		this->wndHandle = glfwCreateWindow(SW, SH, wndconf.title.c_str(), monitor, nullptr);
		
		if (this->wndHandle == nullptr)
		{
			logger << Log::ERR << "Could not open 32bit / 24d8s window of size: " << SW << ", " << SH << Log::ENDL;
			glfwTerminate();
			throw "Window::init(): Could not open OpenGL context window, check your drivers!";
		}
		
		// make this window the current OpenGL context
		glfwMakeContextCurrent(this->wndHandle);
		
		// vertical sync
		if (wndconf.vsync)
		{
			glfwSwapInterval(1);
		}
	}
	
	void WindowClass::close()
	{
		glfwDestroyWindow(wndHandle);
		wndHandle = nullptr;
	}
	
	GLFWwindow* WindowClass::window()
	{
		return this->wndHandle;
	}
	
	void WindowClass::setTitle(std::string title)
	{
		glfwSetWindowTitle(wndHandle, title.c_str());
	}
	
	void WindowClass::setPosition(int x, int y)
	{
		glfwSetWindowPos(wndHandle, x, y);
	}
	
	void WindowClass::startRenderingLoop(renderFunc renderfunc, double granularity)
	{
		double dtime, t0, t1;
		t0 = t1 = glfwGetTime();
		
		while (glfwWindowShouldClose(wndHandle) == 0)
		{
			// variable delta-frame timing
			dtime = 1.0 + (t0 - (t1 + granularity)) / granularity;
			
			// render function returns false if we should stop rendering
			if (renderfunc(*this, dtime) == false) break;
			
			t1 = t0;
			t0 = glfwGetTime();
		}
	}
	
}