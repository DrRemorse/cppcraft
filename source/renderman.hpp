#ifndef RENDERMAN_HPP
#define RENDERMAN_HPP

#include "library/opengl/window.hpp"
#include <string>

namespace cppcraft
{
	class WorldManager;
	class SceneRenderer;
	
	class Renderer
	{
	private:
		// OpenGL window
		library::WindowClass gamescr;
		
		int framenumber;
		double framerad, frametick;
		double uptimed,  motiontimed;
		// this frames delta-time
		double dtime;
		
		// renders a scene
		void render(double dtime, WorldManager&);
		
		// scene renderer object
		SceneRenderer* sceneRenderer;
		
	public:
		Renderer();
		// initializes OpenGL & creates OpenGL window
		void create(std::string);
		// prepares for rendering, initializing subsystems
		void prepare();
		// running forever -->
		void renderloop(WorldManager&);
		// <-- until game ends
		library::WindowClass& getScreen() { return gamescr; }
		
		friend class SceneRenderer;
		friend class GUIRenderer;
	};
	
}

#endif
