#include "render_scene.hpp"

#include "library/config.hpp"
#include "library/log.hpp"
#include "library/opengl/fbo.hpp"
#include "library/opengl/opengl.hpp"
#include "library/opengl/window.hpp"
#include "library/opengl/camera.hpp"

#include "drawq.hpp"
#include "gameconf.hpp"
#include "minimap.hpp"
#include "particles.hpp"
#include "player.hpp"
#include "player_logic.hpp"
#include "renderman.hpp"
#include "render_fs.hpp"
#include "render_player_selection.hpp"
#include "render_sky.hpp"
#include "renderconst.hpp"
#include "shaderman.hpp"
#include "textureman.hpp"
#include "threading.hpp"
#include "world.hpp"
#include <cmath>

using namespace library;

namespace cppcraft
{
	FBO sceneFBO;
	FBO reflectionFBO;
	FBO screenFBO;
	
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
		
		// the FBO we render reflection to
		reflectionFBO.create();
		reflectionFBO.bind();
		reflectionFBO.attachColor(0, textureman.get(Textureman::T_REFLECTION));
		reflectionFBO.createDepthRBO(renderer.gamescr.SW, renderer.gamescr.SH);
		reflectionFBO.unbind();
		
		// initialize reflection camera to be the same as regular camera,
		// except it will be mirrored on Y-axis from water-plane level
		reflectionCamera.init(renderer.gamescr);
	}
	
	// render normal scene
	void SceneRenderer::render(Renderer& renderer, WorldManager& worldman)
	{
		bool frustumRecalc = false;
		
		// bind the FBO that we are rendering the entire scene into
		sceneFBO.bind();
		sceneFBO.attachColor(0, textureman.get(Textureman::T_FOGBUFFER));
		sceneFBO.attachColor(1, textureman.get(Textureman::T_SKYBUFFER));
		
		// add all attachments to rendering output
		std::vector<int> dbuffers;
		dbuffers.emplace_back(GL_COLOR_ATTACHMENT0);
		dbuffers.emplace_back(GL_COLOR_ATTACHMENT1);
		sceneFBO.drawBuffers(dbuffers);
		
		glDepthMask(GL_TRUE);
		glClear(GL_DEPTH_BUFFER_BIT);
		
		glDisable(GL_CULL_FACE); // because the lower half hemisphere is inverted
		glDisable(GL_DEPTH_TEST);
		glDepthMask(GL_FALSE);
		
		/// update frustum rotation matrix ///
		if (camera.rotated)
		{
			camera.setRotation(player.xrotrad, player.yrotrad, 0.0);
		}
		
		/////////////////////////////////////////
		///   render atmosphere, moon, etc.   ///
		/////////////////////////////////////////
		skyrenderer.render(camera, underwater);
		
		// render clouds before terrain if we are submerged in water
		if (true) //underwater)
		{
			glEnable(GL_BLEND);
			glColorMask(1, 1, 1, 0);
			
			// render clouds
			skyrenderer.renderClouds(-playerY, camera, renderer.frametick);
			
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
				reflectionCamera.setWorldOffset(playerX, playerZ);
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
		
		const bool renderconfReflection = true;
		const bool renderconfReflectTerrain = true;
		
		if (underwater == false && drawq[(int)RenderConst::MAX_UNIQUE_SHADERS-1].count() != 0)
		{
			if (renderconfReflection)
			{
				reflectionCamera.ref = camera.ref;
				reflectionCamera.rotated = camera.rotated;
				
				// render to reflection texture
				reflectionFBO.bind();
				reflectionFBO.attachColor(0, textureman.get(Textureman::T_REFLECTION));
				// render at half size
				glViewport(0, 0, renderer.gamescr.SW / 2, renderer.gamescr.SH / 2);
				
				glDepthMask(GL_TRUE);
				glClear(GL_DEPTH_BUFFER_BIT);
				
				glDisable(GL_DEPTH_TEST);
				glDepthMask(GL_FALSE);
				glDisable(GL_CULL_FACE);
				
				skyrenderer.render(reflectionCamera, false);
				
				if (renderconfReflectTerrain)
				{
					glEnable(GL_DEPTH_TEST);
					glDepthFunc(GL_LEQUAL);
					glDepthMask(GL_TRUE);
					glEnable(GL_CULL_FACE);
					
					renderReflectedScene(renderer, reflectionCamera);
				}
				
				// render clouds to be reflected after terrain
				if (true)
				{
					glDisable(GL_CULL_FACE);
					glEnable(GL_BLEND);
					glColorMask(1, 1, 1, 0);
					
					// render clouds
					float dy = playerY - RenderConst::WATER_LEVEL;
					skyrenderer.renderClouds(dy, reflectionCamera, renderer.frametick);
					
					glDisable(GL_BLEND);
					glColorMask(1, 1, 1, 1);
				}
				
				reflectionFBO.unbind();
				glViewport(0, 0, renderer.gamescr.SW, renderer.gamescr.SH);
				
			}
		}
		
		// we are underwater, blit sky instead of reflection
		reflectionFBO.bind();
		reflectionFBO.attachColor(0, textureman.get(Textureman::T_UNDERWATERMAP));
		
		sceneFBO.blitTo(reflectionFBO, renderer.gamescr.SW, renderer.gamescr.SH, GL_COLOR_BUFFER_BIT, GL_NEAREST);
		
		sceneFBO.bind();
		// replace skybuffer with underwater buffer
		sceneFBO.attachColor(1, textureman.get(Textureman::T_UNDERWATERMAP));
		
		/// render physical scene w/depth ///
		
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glDepthMask(GL_TRUE);
		
		glEnable(GL_CULL_FACE);
		
		// scene
		renderScene(renderer, camera);
		
		// render player selection
		renderPlayerSelection();
		
		// stop writing to underwatermap
		sceneFBO.removeColor(1);
		sceneFBO.drawBuffers();
		
		// cull water faces if player is not (fully) submerged in water
		if (plogic.FullySubmerged == plogic.PS_None)
		{
			glEnable(GL_CULL_FACE);
		}
		glDisable(GL_BLEND);
		
		// then water
		renderSceneWater(renderer);
		
		glDisable(GL_CULL_FACE);
		
		glDisable(GL_DEPTH_TEST);
		glDepthMask(GL_FALSE);
		
		sceneFBO.unbind();
		
		/// create fog based on depth ///
		textureman.bind(0, Textureman::T_FOGBUFFER);
		textureman.bind(1, Textureman::T_SKYBUFFER);
		
		screenspace.fog(renderer.gamescr);
		
		// blur the render buffer
		textureman.bind(0, Textureman::T_RENDERBUFFER);
		screenspace.blur(renderer.gamescr);
		
		// without affecting depth, use screenspace renderer to render blurred terrain
		// and also blend terrain against sky background
		sceneFBO.bind();
		sceneFBO.attachColor(0, textureman.get(Textureman::T_FOGBUFFER));
		
		textureman.bind(0, Textureman::T_RENDERBUFFER);
		textureman.bind(1, Textureman::T_BLURBUFFER2);
		
		screenspace.terrain(renderer.gamescr);
		
		///  render clouds & particles  ///
		
		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_FALSE);
		glDepthFunc(GL_LEQUAL);
		glEnable(GL_BLEND);
		
		glColorMask(1, 1, 1, 0);
		
		if (false) //underwater == false)
		{
			// render clouds
			skyrenderer.renderClouds(-playerY, camera, renderer.frametick);
		}
		
		// render particles
		particleSystem.render(snapWX, snapWZ);
		
		glColorMask(1, 1, 1, 1);
		glDisable(GL_BLEND);
		
		sceneFBO.unbind();
		
	} // render scene
	
}
