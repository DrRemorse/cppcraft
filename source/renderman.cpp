#include "renderman.hpp"

#include "library/config.hpp"
#include "library/log.hpp"
#include "library/opengl/opengl.hpp"

#include "compilers.hpp"
#include "camera.hpp"
#include "gameconf.hpp"
#include "player.hpp"
#include "render_fs.hpp"
#include "render_gui.hpp"
#include "render_scene.hpp"
#include "shaderman.hpp"
#include "sun.hpp"
#include "textureman.hpp"
#include "tiles.hpp"
#include "threading.hpp"
#include "worldmanager.hpp"
#include <cmath>
#include <sstream>

//#define DEBUG
#include "input.hpp"

using namespace library;

namespace cppcraft
{
	Renderer::Renderer()
	{
		this->frametick = 0.0;
		this->dtime = 1.0;
	}
	
	void Renderer::create(std::string windowTitle)
	{
		WindowConfig wndconf;
		
		wndconf.title = windowTitle;
		wndconf.fullscreen  = config.get("opengl.fullscreen", false);
		if (wndconf.fullscreen)
		{
			wndconf.SW = config.get("screen.width", 1280);
			wndconf.SH = config.get("screen.height", 720);
			wndconf.refreshrate = config.get("opengl.refresh", 0);
		}
		else
		{
			wndconf.SW = config.get("window.width", 1280);
			wndconf.SH = config.get("window.height", 720);
		}
		wndconf.vsync       = config.get("opengl.vsync", true);
		
		// open a GLFW ogl context window
		gamescr.open(wndconf);
		// initialize openGL extensions
		ogl.init(gamescr);
		
		// enable custom point sprites
		glEnable(GL_VERTEX_PROGRAM_POINT_SIZE_ARB);
		glTexEnvi(GL_POINT_SPRITE, GL_COORD_REPLACE, GL_TRUE);
		
		// enable seamless cubemaps (globally)
		glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
		
		if (OpenGL::checkError())
		{
			logger << Log::ERR << "Renderer::init(): OpenGL error. Line: " << __LINE__ << Log::ENDL;
			throw std::string("Renderer::init(): General openGL error");
		}
		
	}
	
	void Renderer::prepare()
	{
		logger << Log::INFO << "* Preparing renderer" << Log::ENDL;
		
		// initialize compilers
		compilers.initCompilers();
		
		// initialize camera
		camera.init(gamescr);
		// init tile sizes
		tiles.init();
		// initialize sun, and lens textures at half-size
		thesun.init(SunClass::SUN_DEF_ANGLE);
		
		// initialize screenspace blur, lensflare & crepuscular beams
		screenspace.init(gamescr);
		
		// initialize texture manager
		textureman.init(gamescr);
		// initialize shader manager
		shaderman.init(gamescr, camera.getProjection(), camera.getProjectionLong());
		
		// initialize scene renderer
		sceneRenderer = new SceneRenderer();
		sceneRenderer->init(*this);
		
		// initialize gui renderer
		rendergui.init(*this);
	}
	
	void Renderer::render(double time_d_factor, WorldManager& worldman)
	{
		this->dtime = time_d_factor;
		
		// increment framecounter
		this->frametick += dtime;
		
		thesun.integrate(0.02 * dtime);
		
		switch (worldman.getState())
		{
		case worldman.GS_RUNNING:
			// render scene
			sceneRenderer->render(*this, worldman);
			// post processing
			screenspace.render(gamescr, this->frametick);
			// gui
			rendergui.render(*this);
			break;
		case worldman.GS_PAUSED:
			sceneRenderer->render(*this, worldman);
			//renderPause();
			break;
			
		default:
			logger << Log::ERR << "Renderer::render(): Invalid gamestate. Line: " << __LINE__ << Log::ENDL;
			throw std::string("Renderer::render(): This gamestate is not implemented yet!");
		}
		
		if (ogl.checkError()) throw std::string("After frame");
		
		// flip burgers
		glfwSwapBuffers(gamescr.window());
		
		// disable stuff
		camera.rotated = false;
		
		if (OpenGL::checkError())
		{
			logger << Log::ERR << "Renderer::render(): OpenGL error. Line: " << __LINE__ << Log::ENDL;
			throw std::string("Renderer::render(): OpenGL state error");
		}
		
	}
	
	
	/**
	 * Rendering loop w/delta frame timing
	 * Runs forever until game ends
	**/
	
	void Renderer::renderloop(WorldManager& worldman)
	{
		const double render_granularity = 0.01; // 10ms granularity
		double t1 = glfwGetTime();
		
		//float FPS = 0.0;
		
		while (glfwWindowShouldClose(gamescr.window()) == 0 && mtx.terminate == false)
		{
			// variable delta frame timing
			double t0 = t1;
			t1 = glfwGetTime();
			dtime = (t1 - t0) / render_granularity;
			
			/// FPS COUNTER ///
			/*
			FPS = FPS * 0.9 + (1.0 / (t0 - t1)) * 0.1;
			
			std::stringstream ss;
			ss << FPS;
			glfwSetWindowTitle(gamescr.window(), ss.str().c_str());
			*/
			
			// compiling columns
		#ifdef DEBUG
			try
			{
				if (mtx.sectorseam.try_lock())
				{
					compilers.compile();
					mtx.sectorseam.unlock();
				}
			}
			catch (std::string errorstring)
			{
				logger << Log::ERR << errorstring << Log::ENDL;
				logger << Log::ERR << "Error during compilation stage, exiting..." << Log::ENDL;
				break;
			}
		#else
			if (mtx.sectorseam.try_lock())
			{
				compilers.compile();
				mtx.sectorseam.unlock();
			}
		#endif
			// rendering function
		#ifdef DEBUG
			try
			{
				render(dtime, worldman);
			}
			catch (std::string errorstring)
			{
				logger << Log::ERR << errorstring << Log::ENDL;
				logger << Log::ERR << "Error during rendering stage, exiting..." << Log::ENDL;
				break;
			}
		#else
			render(dtime, worldman);
		#endif
			// poll for events
			glfwPollEvents();
			
			// handle rotation
			player.handleRotation();
			
		}
		
		// close main window
		gamescr.close();
		
		// cleanup GLFW / threading (whenever that happens)
		glfwTerminate();
	}
	
}
