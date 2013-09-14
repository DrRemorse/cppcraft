#include "window.hpp"

#include "../log.hpp"
#include <GL/glfw3.h>

namespace library
{
	WindowConfig::WindowConfig()
	{
		// set default window settings
		this->title = "Untitled window";
		this->SW = 640;
		this->SH = 480;
		this->fullscreen = false;
		this->refreshrate = 60;
		this->vsync = true;
		this->multisample = 0;
	}
	WindowConfig::WindowConfig(std::string title, int width, int height)
	{
		// set default window settings
		this->title = title;
		this->SW = width;
		this->SH = height;
		this->fullscreen = false;
		this->refreshrate = 60;
		this->vsync = true;
		this->multisample = 0;
	}
	
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
		
		// fullscreen enables setting refresh rate
		this->fullscreen = wndconf.fullscreen;
		
		GLFWmonitor* monitor = nullptr;
		// set primary monitor if we are to run in fullscreen
		if (fullscreen) monitor = glfwGetPrimaryMonitor();
		
		glfwWindowHint(GLFW_REFRESH_RATE, wndconf.refreshrate);
		glfwWindowHint(GLFW_RESIZABLE, 0);
		
		// clamp multisample to at least 0
		if (wndconf.multisample < 0) wndconf.multisample = 0;
		// set multisampling level for main framebuffer
		glfwWindowHint(GLFW_SAMPLES, wndconf.multisample);
		// common alpha, stencil & depth settings
		glfwWindowHint(GLFW_ALPHA_BITS, 8);
		glfwWindowHint(GLFW_DEPTH_BITS,  24);
		glfwWindowHint(GLFW_STENCIL_BITS, 8);
		
		// create new glfw3 window
		this->wndHandle = glfwCreateWindow(SW, SH, wndconf.title.c_str(), monitor, nullptr);
		
		if (this->wndHandle == nullptr)
		{
			logger << Log::ERR << "Could not open 32bit / 24d8s window of size: " << SW << ", " << SH << Log::ENDL;
			glfwTerminate();
			throw "Window::init(): Could not open OpenGL context window, check your drivers!";
		}
		
		// make this window the current OpenGL context
		setCurrent();
		
		// vertical sync
		glfwSwapInterval((wndconf.vsync) ? 1 : 0);
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
	
	// makes this window the current opengl context
	void WindowClass::setCurrent()
	{
		glfwMakeContextCurrent(this->wndHandle);
	}
	
	void WindowClass::startRenderingLoop(renderFunc renderfunc, double granularity)
	{
		double t0 = glfwGetTime();
		
		while (glfwWindowShouldClose(wndHandle) == 0)
		{
			double t1 = t0;
			t0 = glfwGetTime();
			// variable delta-frame timing
			double dtime = (t1 - t0) / granularity;
			
			// render function returns false if we should stop rendering
			if (renderfunc(*this, dtime, t0) == false) break;
			
		}
	}
	
}