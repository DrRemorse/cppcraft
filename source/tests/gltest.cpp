// opengl window
#include "../library/opengl/window.hpp"
// matrix and vector
#include "../library/math/vector.hpp"
#include "../library/math/matrix.hpp"

#include <GL/glfw3.h>
#include <iostream>
#include <cmath>
#include "sleep.hpp"

using namespace library;

double accumulatedTime;

bool blueScreen(WindowClass& wnd, double dtime)
{
	accumulatedTime += dtime;
	
	glClearColor(0.3, 0.6, 1.0, 1.0);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	
	// swap frontbuffer with backbuffer (frame change)
	glfwSwapBuffers(wnd.window());
	// close window after 2 seconds (2000ms)
	return (accumulatedTime < 1);
}

bool triangleScreen(WindowClass& wnd, double dtime)
{
	accumulatedTime += dtime;
	
	// (initial) depth settings
	glClearDepth(1.0);
	glDepthRange(0.0, 1.0);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	
	// alpha stencil test
	glAlphaFunc(GL_NOTEQUAL, 0.0);
	//glEnable(GL_ALPHA_TEST);
	
	// culling & shading
	glCullFace(GL_BACK);
	glPolygonMode(GL_FRONT, GL_FILL);
	glShadeModel(GL_SMOOTH);
	
	glClearColor(0.3, 0.6, 1.0, 1.0);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	
	// set up orthographic matrix
	Matrix matproj;
	matproj.orthoScreen(wnd.SW, wnd.SH, 0, 2);
	
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(matproj.data());
	
	// create rotation matrix, for rotating the "screen" around itself
	Matrix matrot;
	matrot.rotateZYX(0, 0, accumulatedTime * 2.5);
	
	// modelview matrix
	Matrix matview;
	matview.translated(wnd.SW / 2, wnd.SH / 2, 0.0);
	matview.scale(0.75 + 0.25 * sin(accumulatedTime * 5.0));
	matview = matview * matrot;
	matview.translated(-wnd.SW / 2, -wnd.SH / 2, 0.0);
	
	// translate camera back, allowing stuff located at z=0 to appear in front
	matview.translated(0.0, 0.0, -2.0);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(matview.data());
	
	glBegin(GL_TRIANGLES);
		glColor3f(0.1, 0.2, 0.3);
		glVertex3f(0, wnd.SH, 0);
		glColor3f(0.4, 0.5, 0.6);
		glVertex3f(0, 0, 1);
		glColor3f(0.5, 0.8, 1.0);
		glVertex3f(wnd.SW, 0, 2);
	glEnd();
	
	glBegin(GL_TRIANGLES);
		glColor3f(0.3, 0.2, 0.1);
		glVertex3f(0, 0, 2);
		glColor3f(0.6, 0.4, 0.2);
		glVertex3f(wnd.SW, 0, 1);
		glColor3f(1.0, 0.5, 0.3);
		glVertex3f(0, wnd.SH, 0);
	glEnd();
	
	glBegin(GL_TRIANGLES);
		glColor3f(0.3, 0.2, 0.1);
		glVertex3f(wnd.SW, 0, 2);
		glColor3f(0.6, 0.4, 0.2);
		glVertex3f(wnd.SW, wnd.SH, 0);
		glColor3f(1.0, 0.5, 0.3);
		glVertex3f(0, wnd.SH, 1);
	glEnd();
	
	glBegin(GL_TRIANGLES);
		glColor3f(0.1, 0.2, 0.3);
		glVertex3f(wnd.SW, wnd.SH, 1);
		glColor3f(0.4, 0.5, 0.6);
		glVertex3f(0, wnd.SH, 0);
		glColor3f(0.5, 0.8, 1.0);
		glVertex3f(wnd.SW, 0, 2);
	glEnd();
	
	// swap frontbuffer with backbuffer (frame change)
	glfwSwapBuffers(wnd.window());
	// close window after 2 seconds (2000ms)
	return (accumulatedTime < 4);
}

void test_opengl_window()
{
	std::cout << "Opening OpenGL window" << std::endl;
	
	WindowConfig wndconf;
	wndconf.fullscreen = false;
	wndconf.SW = 640;
	wndconf.SH = 480;
	wndconf.multisample = 0;
	
	// open up OpenGL window
	WindowClass bluescr;
	// throws error if something bad happens...
	bluescr.open(wndconf);
	
	//////////////////////////////////////////////////////////////////
	std::cout << "Starting rendering loop: Blue screen" << std::endl;
	std::cout << "Window closes after 1 second" << std::endl;
	
	// what we want delta-time to be measured in:
	// 1ms granularity, meaning 1.0 dtime is 1ms, 100.0 is 100ms etc.
	// Examples: 1/1000 = 1ms/frame, 1/60 = 16.7ms/frame (60fps) etc.
	// And finally, 1.0 means dtime is measured in (whole) seconds
	const double timing_granularity = 1.0;
	
	accumulatedTime = 0;
	bluescr.startRenderingLoop(blueScreen, timing_granularity);
	
	//////////////////////////////////////////////////////////////////
	std::cout << "Starting rendering loop: Rotating triangle" << std::endl;
	std::cout << "Window closes after 2 seconds" << std::endl;
	
	WindowClass triscr;
	// open up another window
	triscr.open(wndconf);
	// reset timer
	accumulatedTime = 0;
	// start rendering using triangleScreen function
	triscr.startRenderingLoop(triangleScreen, timing_granularity);
	
	std::cout << "Closing all windows" << std::endl;
	triscr.close();
	bluescr.close();
	
	//////////////////////////////////////////////////////////////////
	std::cout << "OpenGL tests completed, exiting module..." << std::endl;
	sleep(1);
}