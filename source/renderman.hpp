#ifndef RENDERMAN_HPP
#define RENDERMAN_HPP

#include <library/opengl/window.hpp>
#include <string>

namespace cppcraft
{
	class WorldManager;
	class SceneRenderer;
	
	class Renderer
	{
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
		
	private:
		// OpenGL window
		library::WindowClass gamescr;
		
		// elapsed time
		double frametick;
		// this frames delta-time
		double dtime;
		// average FPS
		double FPS;
		// rendered terrain elements this frame
		int scene_elements;
		
		// renders a scene
		void render(double dtime, WorldManager&);
		
		// scene renderer object
		SceneRenderer* sceneRenderer;
		
		friend class SceneRenderer;
		friend class GUIRenderer;
		friend class Chatbox;
	};
	
}

#endif
