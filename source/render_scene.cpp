#include "render_scene.hpp"

#include "library/config.hpp"
#include "library/log.hpp"
#include "library/opengl/opengl.hpp"
#include "library/opengl/window.hpp"

#include "camera.hpp"
#include "minimap.hpp"
#include "particles.hpp"
#include "player.hpp"
#include "player_logic.hpp"
#include "renderman.hpp"
#include "render_fs.hpp"
#include "render_sky.hpp"
#include "shaderman.hpp"
#include "textureman.hpp"
#include "threading.hpp"
#include "world.hpp"
#include <cmath>

using namespace library;

namespace cppcraft
{
	void SceneRenderer::init(Renderer& renderer)
	{
		// initialize terrain renderer
		initTerrain();
		
		// initialize sky renderer
		skyrenderer.init();
		
		// initialize minimap
		minimap.init();
	}
	
	// render normal scene
	void SceneRenderer::render(Renderer& renderer, WorldManager& worldman)
	{
		bool frustumRecalc = false;
		bool underwater    = false;
		
		// render each blah, because of blah-bah
		if (mtx.sectorseam.try_lock())
		{
			mtx.playermove.lock();
			{
				//------------------------------------//
				//          player snapshot           //
				//------------------------------------//
				this->playerX = player.snapX;
				this->playerY = player.snapY;
				this->playerZ = player.snapZ;
				this->playerMoved = player.JustMoved;
				
				this->playerSectorX = playerX / 16; //Sector::BLOCKS_XZ;
				this->playerSectorZ = playerZ / 16; //Sector::BLOCKS_XZ;
				
				underwater = plogic.FullySubmerged != PlayerLogic::PS_None;
				
				frustumRecalc = camera.recalc;
				camera.recalc = false;
			}
			mtx.playermove.unlock();
			
			if (frustumRecalc)
			{
				/// update matview matrix using player snapshot ///
				camera.setWorldOffset(playerX, playerZ);
			}
			
			/// world coordinate snapshots ///
			snapWX = world.getWX();
			snapWZ = world.getWZ();
			
			/// update minimap ///
			minimap.update();
			
			/// update particles ///
			particleSystem.renderUpdate();
			
			/// camera deviations ///
			if (cameraDeviation(renderer.frametick, renderer.dtime))
			{
				frustumRecalc = true;
			}
			
			// update frustum rotation matrix
			if (camera.rotated)
			{
				camera.setRotation(player.xrotrad, player.yrotrad, 0.0);
			}
			
			//---------------------------------//
			// Start of frustum recalculations //
			//---------------------------------//
			
			// frustum.ref: run occlusion tests
			// - is never automatically disabled
			// frustum.needsupd = gather occlusion data
			// - once an update has happened, it is disabled
			
			if (frustumRecalc)
			{
				// -= recalculate frustum =-
				
				// do as little as possible this frame
				camera.needsupd = 1;
				// frustum was updated
				//camera.updated = true;
				camera.ref = true;
				// process columns & modify occlusion
				recalculateFrustum();
			}
			else
			{
				// not recalculating frustum
				
				// if last frame was an occlusion test
				if (camera.ref)
				{
					// gather occlusion data
					camera.needsupd = 2;
					// disable full refresh
					camera.ref = false;
				}
				// if signal for refresh
				else if (camera.needsupd == 1)
				{
					// run occlusion test
					camera.ref = true;
					camera.needsupd = 0;
				}
			}
			mtx.sectorseam.unlock();
		}
		
		// render queue needed update
		if (camera.needsupd)
		{
			// compress rendering queue to minimal size by occlusion culling
			compressRenderingQueue();
		}
		
		glDisable(GL_CULL_FACE); // because the lower half hemisphere is inverted
		glDisable(GL_DEPTH_TEST);
		glDepthMask(GL_FALSE);
		
		// render sky
		skyrenderer.render(*this, underwater);
		
		if (underwater)
		{
			glEnable(GL_BLEND);
			glColorMask(1, 1, 1, 0);
			
			// render clouds
			skyrenderer.renderClouds(*this, renderer.frametick);
			
			glDisable(GL_BLEND);
			glColorMask(1, 1, 1, 1);
		}
		
		// copy sky to texture (skybuffer)
		textureman.bind(5, Textureman::T_SKYBUFFER);
		textureman.copyScreen(renderer.gamescr, Textureman::T_SKYBUFFER);
		
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glDepthMask(GL_TRUE);
		
		glEnable(GL_CULL_FACE);
		glEnable(GL_MULTISAMPLE_ARB);
		
		/// render physical scene w/depth ///
		
		renderScene(renderer);
		
		glDisable(GL_MULTISAMPLE_ARB);
		glDisable(GL_CULL_FACE); // because the lower half hemisphere is inverted
		
		glDisable(GL_DEPTH_TEST);
		glDepthMask(GL_FALSE);
		
		/// take snapshot of scene ///
		// there are no more things in the depth buffer after rendering scene
		textureman.bind(1, Textureman::T_DEPTHBUFFER);
		textureman.copyScreen(renderer.gamescr, Textureman::T_DEPTHBUFFER);
		
		// first snapshot of world
		textureman.bind(0, Textureman::T_RENDERBUFFER);
		textureman.copyScreen(renderer.gamescr, Textureman::T_RENDERBUFFER);
		
		// blur the render buffer
		screenspace.blur(renderer.gamescr);
		
		// without affecting depth, use screenspace renderer to render blurred terrain
		// and also blend terrain against sky background
		
		screenspace.terrain(renderer.gamescr);
		
		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_FALSE);
		glDepthFunc(GL_LEQUAL);
		glEnable(GL_BLEND);
		
		if (!underwater)
		{
			glDisable(GL_CULL_FACE);
			
			// render clouds
			skyrenderer.renderClouds(*this, renderer.frametick);
		}
		
		// render particles
		particleSystem.render(snapWX, snapWZ);
		
	} // render scene
	
}
