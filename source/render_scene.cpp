#include "render_scene.hpp"

#include "library/config.hpp"
#include "library/log.hpp"
#include "library/opengl/fbo.hpp"
#include "library/opengl/opengl.hpp"
#include "library/opengl/window.hpp"

#include "camera.hpp"
#include "gameconf.hpp"
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
	FBO sceneFBO;
	
	void SceneRenderer::init(Renderer& renderer)
	{
		// initialize terrain renderer
		initTerrain();
		
		// initialize sky renderer
		skyrenderer.init();
		
		// initialize minimap
		minimap.init();
		
		// the FBO we render the main scene to
		sceneFBO.create();
		sceneFBO.bind();
		sceneFBO.createDepthRBO(renderer.gamescr.SW, renderer.gamescr.SH);
		sceneFBO.unbind();
	}
	
	// render normal scene
	void SceneRenderer::render(Renderer& renderer, WorldManager& worldman)
	{
		bool frustumRecalc = false;
		
		// update frustum rotation matrix
		if (camera.rotated)
		{
			camera.setRotation(player.xrotrad, player.yrotrad, 0.0);
		}
		
		// bind the FBO that we are rendering the entire scene into
		sceneFBO.bind();
		sceneFBO.attachColor(0, textureman.get(Textureman::T_FOGBUFFER));
		sceneFBO.attachColor(1, textureman.get(Textureman::T_UNDERWATERMAP));
		sceneFBO.attachColor(2, textureman.get(Textureman::T_SKYBUFFER));
		
		// add all attachments to rendering output
		std::vector<int> dbuffers;
		dbuffers.emplace_back(GL_COLOR_ATTACHMENT0);
		dbuffers.emplace_back(GL_COLOR_ATTACHMENT1);
		dbuffers.emplace_back(GL_COLOR_ATTACHMENT2);
		
		sceneFBO.drawBuffers(dbuffers);
		
		glDepthMask(GL_TRUE);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		glDisable(GL_CULL_FACE); // because the lower half hemisphere is inverted
		glDisable(GL_DEPTH_TEST);
		glDepthMask(GL_FALSE);
		
		// render atmosphere, moon, etc.
		skyrenderer.render(*this, underwater);
		
		// render clouds before terrain if we are submerged in water
		if (underwater)
		{
			glEnable(GL_BLEND);
			glColorMask(1, 1, 1, 0);
			
			// render clouds
			skyrenderer.renderClouds(*this, renderer.frametick);
			
			glDisable(GL_BLEND);
			glColorMask(1, 1, 1, 1);
		}
		
		////////////////////////////////////////////////////
		/// take snapshots of player state               ///
		/// and recalculate rendering queue if necessary ///
		////////////////////////////////////////////////////
		
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
				camera.ref = false;
				
				// if last frame was an occlusion test
				if (camera.needsupd == 1)
				{
					// gather occlusion results
					camera.needsupd = 2;
				}
			}
			mtx.sectorseam.unlock();
		}
		
		// render queue needed update
		if (camera.needsupd == 2)
		{
			// compress rendering queue to minimal size by occlusion culling
			compressRenderingQueue();
		}
		
		// remove skybuffer from rendering output
		// and replace it with the normals texture
		sceneFBO.removeColor(2);
		//sceneFBO.attachColor(2, textureman.get(Textureman::T_FSNORMALS));
		// update buffer list
		dbuffers.clear();
		dbuffers.emplace_back(GL_COLOR_ATTACHMENT0);
		dbuffers.emplace_back(GL_COLOR_ATTACHMENT1);
		sceneFBO.drawBuffers(dbuffers);
		
		
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glDepthMask(GL_TRUE);
		
		glEnable(GL_CULL_FACE);
		if (gameconf.multisampling)
		{
			glEnable(GL_MULTISAMPLE_ARB);
		}
		/// render physical scene w/depth ///
		
		renderScene(renderer, sceneFBO);
		
		if (gameconf.multisampling)
		{
			glDisable(GL_MULTISAMPLE_ARB);
		}
		glDisable(GL_CULL_FACE);
		
		glDisable(GL_DEPTH_TEST);
		glDepthMask(GL_FALSE);
		
		sceneFBO.unbind();
		
		/// create fog based on depth ///
		textureman.bind(0, Textureman::T_FOGBUFFER);
		textureman.bind(1, Textureman::T_SKYBUFFER);
		//textureman.bind(2, Textureman::T_FSNORMALS);
		//textureman.bind(3, Textureman::T_NOISE);
		
		screenspace.fog(renderer.gamescr);
		
		// blur the render buffer
		textureman.bind(0, Textureman::T_RENDERBUFFER);
		screenspace.blur(renderer.gamescr);
		
		// without affecting depth, use screenspace renderer to render blurred terrain
		// and also blend terrain against sky background
		sceneFBO.bind();
		sceneFBO.attachColor(0, textureman.get(Textureman::T_FOGBUFFER));
		
		screenspace.terrain(renderer.gamescr);
		
		///  render clouds & particles  ///
		
		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_FALSE);
		glDepthFunc(GL_LEQUAL);
		glEnable(GL_BLEND);
		
		glColorMask(1, 1, 1, 0);
		
		if (underwater == false)
		{
			// render clouds
			skyrenderer.renderClouds(*this, renderer.frametick);
		}
		
		// render particles
		particleSystem.render(snapWX, snapWZ);
		
		glColorMask(1, 1, 1, 1);
		glDisable(GL_BLEND);
		
		sceneFBO.unbind();
		
	} // render scene
	
}
