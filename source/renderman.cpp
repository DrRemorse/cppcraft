#include "renderman.hpp"

#include "library/config.hpp"
#include "library/log.hpp"
#include "library/opengl/opengl.hpp"

#include "compilers.hpp"
#include "camera.hpp"
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

using namespace library;

namespace cppcraft
{
	static const double PI = 4 * atan(1);
	
	Renderer::Renderer()
	{
		this->frametick = 0.0;
		this->framerad  = 0.0;
		this->dtime = 1.0;
	}
	
	void Renderer::create(std::string windowTitle)
	{
		WindowConfig wndconf;
		
		wndconf.title = windowTitle;
		wndconf.SW    = config.get("screen.width", 1280);
		wndconf.SH    = config.get("screen.height", 720);
		wndconf.vsync = config.get("opengl.vsync", true);
		wndconf.fullscreen  = config.get("opengl.fullscreen", false);
		wndconf.multisample = config.get("multisampling", 0);
		
		// initialize GLFW window
		gamescr.open(wndconf);
		
		// set multisampling & anisotropic filtering level
		ogl.multisample = wndconf.multisample;
		ogl.anisotrophy = config.get("anisotropy", 1);
		if (ogl.anisotrophy < 1) ogl.anisotrophy = 1;
		// initialize openGL
		ogl.init(gamescr);
		
		// GL_COMPRESSED_RGBA setting
		glHint(GL_TEXTURE_COMPRESSION_HINT, GL_NICEST);
		// GL_GENERATE_MIPMAP setting
		glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);
		// perspective-correct interpolation setting
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		
		// buffer cleared color
		glClearColor(0.0, 0.0, 0.0, 0.0);
		// (initial) depth settings
		glClearDepth(1.0);
		glDepthRange(0.0, 1.0);
		glDepthFunc(GL_LEQUAL);
		glEnable(GL_DEPTH_TEST);
		
		// culling & shading
		glCullFace(GL_BACK);
		glPolygonMode(GL_FRONT, GL_FILL);
		
		// blend function
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		
		// point sprites
		glEnable(GL_VERTEX_PROGRAM_POINT_SIZE_ARB);
		glTexEnvi(GL_POINT_SPRITE, GL_COORD_REPLACE, GL_TRUE);
		
		// texturing
		// seamless cubemaps globally
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
		
	}
	
	void Renderer::render(double time_d_factor, WorldManager& worldman)
	{
		this->dtime = time_d_factor;
		
		// increment frame counters
		this->framerad += PI / 128.0 * dtime;
		if (this->framerad >= 2 * PI) this->framerad -= 2 * PI;
		
		this->frametick += dtime;
		
		thesun.integrate(0.01 * dtime);
		
		// clear buffers
		glDepthMask(GL_TRUE);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		
		switch (worldman.getState())
		{
		case worldman.GS_RUNNING:
			// render scene
			sceneRenderer->render(*this, worldman);
			// post processing
			screenspace.render(gamescr);
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
		
		// flip burgers / buffers
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
		double t0 = glfwGetTime();
		
		while (glfwWindowShouldClose(gamescr.window()) == 0 && mtx.terminate == false)
		{
			// variable delta frame timing
			double t1 = t0;
			t0 = glfwGetTime();
			dtime = 1.0 * (t0 - t1) / render_granularity;
			
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
			
		}
		
		// close main window
		gamescr.close();
		
		// cleanup GLFW / threading (whenever that happens)
		glfwTerminate();
	}
	
}
