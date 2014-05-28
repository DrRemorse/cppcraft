#include "render_scene.hpp"

#include <library/log.hpp>
#include <library/opengl/fbo.hpp>
#include <library/opengl/opengl.hpp>
#include <library/opengl/window.hpp>
#include <library/opengl/camera.hpp>

//#define TIMING
#ifdef TIMING
#include <library/timing/timer.hpp>
#endif

#include "drawq.hpp"
#include "gameconf.hpp"
#include "minimap.hpp"
#include "netplayers.hpp"
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

using namespace library;

namespace cppcraft
{
	FBO skyFBO;
	FBO sceneFBO;
	FBO reflectionFBO;
	FBO underwaterFBO;
	FBO copyScreenFBO;
	FBO screenFBO;
	
	void SceneRenderer::init(Renderer& renderer)
	{
		// initialize members
		this->lastTime = 0.0;
		this->playerX = this->playerY = this->playerZ = 0.0;
		
		// initialize terrain renderer
		initTerrain();
		
		// initialize sky renderer
		skyrenderer.init();
		
		Texture& sceneTex = textureman[Textureman::T_SCENEBUFFER];
		
		// the FBO we render the sky to
		skyFBO.create();
		skyFBO.bind();
		skyFBO.attachColor(0, textureman[Textureman::T_SKYBUFFER]);
		
		// the FBO we render the main scene to
		sceneFBO.create();
		sceneFBO.bind();
		sceneFBO.attachColor(0, sceneTex);
		sceneFBO.attachDepth(textureman[Textureman::T_DEPTHBUFFER]);
		//sceneFBO.createDepthRBO(sceneTex.getWidth(), sceneTex.getHeight(), GL_DEPTH_COMPONENT24);
		
		underwaterFBO.create();
		underwaterFBO.bind();
		underwaterFBO.attachColor(0, textureman[Textureman::T_UNDERWATERMAP]);
		
		copyScreenFBO.create();
		copyScreenFBO.bind();
		copyScreenFBO.attachColor(0, textureman[Textureman::T_RENDERBUFFER]);
		
		screenFBO.create();
		screenFBO.bind();
		screenFBO.attachColor(0, textureman[Textureman::T_FINALBUFFER]);
		screenFBO.attachDepth(textureman[Textureman::T_DEPTHBUFFER]);
		//screenFBO.attachDepthRBO(sceneFBO);
		
		if (gameconf.reflections)
		{
			Texture& reflections = textureman[Textureman::T_REFLECTION];
			
			// the FBO we render reflection to
			reflectionFBO.create();
			reflectionFBO.bind();
			reflectionFBO.attachColor(0, reflections);
			reflectionFBO.createDepthRBO(reflections.getWidth(), reflections.getHeight(), GL_DEPTH_COMPONENT24);
			
			// initialize reflection camera to be the same as regular camera,
			// except it will be mirrored on Y-axis from water-plane level
			reflectionCamera.init(renderer.gamescr);
		}
		
		// initialize some shaders with (late) texture sizes
		vec3 sceneSize = vec3(sceneTex.getWidth(), sceneTex.getHeight(), renderer.gamescr.SA);
		
		shaderman[Shaderman::BLOCKS_WATER].bind();
		shaderman[Shaderman::BLOCKS_WATER].sendVec3("screendata", sceneSize);
		shaderman[Shaderman::BLOCKS_LAVA].bind();
		shaderman[Shaderman::BLOCKS_LAVA].sendVec3("screendata", sceneSize);
		
		shaderman[Shaderman::BLOCKS_DEPTH].bind();
		shaderman[Shaderman::BLOCKS_DEPTH].sendVec3("screensize", sceneSize);
		
		
		if (OpenGL::checkError())
		{
			logger << Log::ERR << "SceneRenderer::init(): OpenGL error. Line: " << __LINE__ << Log::ENDL;
			throw std::string("SceneRenderer::init(): General openGL error");
		}
	}
	
	// render normal scene
	void SceneRenderer::render(Renderer& renderer)
	{
		bool frustumRecalc = false;
		
		Texture& skyTex = textureman[Textureman::T_SKYBUFFER];
		Texture& sceneTex = textureman[Textureman::T_SCENEBUFFER];
		Texture& renderBuffer = textureman[Textureman::T_RENDERBUFFER];
		
		glDisable(GL_CULL_FACE); // because the lower half hemisphere is rotated
		glDisable(GL_DEPTH_TEST);
		glDepthMask(GL_FALSE);
		
		/// update frustum rotation matrix ///
		if (camera.rotated)
		{
			camera.setRotation(player.xrotrad, player.yrotrad, 0.0);
		}
		
		// bind the FBO that we are rendering the entire scene into
		skyFBO.bind();
		glViewport(0, 0, skyTex.getWidth(), skyTex.getHeight());
		
		/////////////////////////////////////////
		///   render atmosphere, moon, etc.   ///
		/////////////////////////////////////////
		skyrenderer.render(camera, -playerY, renderer.frametick, underwater ? 2 : 0);
		
		// copy sky to scene
		skyFBO.blitTo(sceneFBO, 
					skyTex.getWidth(),
					skyTex.getHeight(),
					sceneTex.getWidth(), 
					sceneTex.getHeight(), 
					GL_COLOR_BUFFER_BIT, GL_LINEAR);
		
		////////////////////////////////////////////////////
		/// take snapshots of player state               ///
		/// and recalculate rendering queue if necessary ///
		////////////////////////////////////////////////////
		
		//if (mtx.sectorseam.try_lock())
		mtx.sectorseam.lock();
		{
			mtx.playermove.lock();
			{
				//------------------------------------//
				//          player snapshot           //
				//------------------------------------//
				
				const double WEIGHT = 0.5;
				//logger << Log::INFO << WEIGHT << " <-- " << renderer.dtime << " / " << 0.0125 << Log::ENDL;
				
				// (cheap) movement interpolation
				playerY = playerY * (1.0 - WEIGHT) + player.snapY * WEIGHT;
				
				int dx = snapWX - world.getWX();
				int dz = snapWZ - world.getWZ();
				if (abs(dx) + abs(dz) < 3)
				{
					playerX = (playerX + dx * 16) * (1.0 - WEIGHT) + player.snapX * WEIGHT;
					playerZ = (playerZ + dz * 16) * (1.0 - WEIGHT) + player.snapZ * WEIGHT;
				}
				else
				{
					playerX = player.snapX;
					playerZ = player.snapZ;
				}
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
			minimap.update(playerX, playerZ);
			
			/// set player positions ///
			netplayers.positionSnapshots(snapWX, snapWZ, renderer.dtime);
			
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
			// frustum.needsupd = occlusion data gathering & processing
			// - once an update has happened, it is disabled
			
			if (frustumRecalc)
			{
				// -= recalculate frustum =-
				
				// do as little as possible this frame
				camera.needsupd = 1;
				// frustum was updated
				//camera.updated = true;
				camera.ref = true;
				
				#ifdef TIMING
				Timer timer;
				timer.startNewRound();
				#endif
				
				// process columns & modify occlusion
				recalculateFrustum();
				
				#ifdef TIMING
				logger << Log::INFO << "Time spent calculating: " << timer.getDeltaTime() * 1000.0 << Log::ENDL;
				#endif
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
		
		// reflections only happen on the exact water-plane anyways, 
		// so we just disable them completely when the player is below it
		if (gameconf.reflections)
		{
			if (playerY >= RenderConst::WATER_LEVEL && drawq[RenderConst::TX_WATER].count() != 0)
			{
				reflectionCamera.ref = camera.ref;
				reflectionCamera.rotated = camera.rotated;
				
				// render to reflection texture
				reflectionFBO.bind();
				
				Texture& reflections = textureman[Textureman::T_REFLECTION];
				
				// render at half size
				glViewport(0, 0, reflections.getWidth(), reflections.getHeight());
				
				glDepthMask(GL_TRUE);
				glClear(GL_DEPTH_BUFFER_BIT);
				
				glDisable(GL_DEPTH_TEST);
				glDepthMask(GL_FALSE);
				//glDisable(GL_CULL_FACE);
				
				// render sky (atmosphere, sun, moon, clouds)
				skyrenderer.render(reflectionCamera, playerY - RenderConst::WATER_LEVEL, renderer.frametick, 1);
				
				if (gameconf.reflectTerrain)
				{
					glEnable(GL_DEPTH_TEST);
					glDepthFunc(GL_LEQUAL);
					glDepthMask(GL_TRUE);
					glEnable(GL_CULL_FACE);
					
					renderReflectedScene(renderer, reflectionCamera);
				}
				
				// unbind and return to normal viewport
				reflectionFBO.unbind();
				glViewport(0, 0, sceneTex.getWidth(), sceneTex.getHeight());
			}
		}
		
		/// render physical scene w/depth ///
		
		sceneFBO.bind();
		glViewport(0, 0, sceneTex.getWidth(), sceneTex.getHeight());
		
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glDepthMask(GL_TRUE);
		
		// clear depth texture (or depth renderbuffer)
		glClear(GL_DEPTH_BUFFER_BIT);
		
		// disable double-sided faces
		glEnable(GL_CULL_FACE);
		
		#ifdef TIMING
		Timer timerScene;
		timerScene.startNewRound();
		#endif
		
		// scene
		renderScene(renderer, camera);
		
		glEnable(GL_CULL_FACE);
		
		// render networked players
		netplayers.renderPlayers(renderer.frametick, renderer.dtime);
		
		// render player selection
		renderPlayerSelection();
		
		// cull water faces if player is not (fully) submerged in water
		if (underwater == false)
			glEnable(GL_CULL_FACE);
		else
			glDisable(GL_CULL_FACE);
		
		// blit terrain to (downsampled) underwatermap
		Texture& underwaterTex = textureman[Textureman::T_UNDERWATERMAP];
		sceneFBO.blitTo(underwaterFBO, 
						sceneTex.getWidth(), sceneTex.getHeight(), 
						underwaterTex.getWidth(), underwaterTex.getHeight(), 
						GL_COLOR_BUFFER_BIT, GL_LINEAR);
		
		sceneFBO.bind();
		
		// finally, render scene water
		renderSceneWater(renderer);
		
		sceneFBO.unbind();
		
		#ifdef TIMING
		logger << Log::INFO << "Time spent on scene: " << timerScene.getDeltaTime() * 1000.0 << Log::ENDL;
		#endif
		
		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);
		glDepthMask(GL_FALSE);
		
		/////////////////////////////////
		// resolve supersampling
		/////////////////////////////////
		if (gameconf.supersampling > 1)
		{
			// back to normal screen-size
			glViewport(0, 0, renderBuffer.getWidth(), renderBuffer.getHeight());
			// render from scene texture into T_FINALBUFFER
			screenspace.renderSuperSampling(sceneTex);
			textureman.bind(0, Textureman::T_FINALBUFFER);
		}
		else
		{
			textureman.bind(0, Textureman::T_SCENEBUFFER);
		}
		
		/////////////////////////////////
		// create fog based on depth
		/////////////////////////////////
		textureman.bind(1, Textureman::T_SKYBUFFER);
		textureman.bind(2, Textureman::T_DEPTHBUFFER);
		// --> outputs T_RENDERBUFFER
		screenspace.fog(vec3(playerX, playerY, playerZ), renderer.frametick);
		
		/////////////////////////////////
		// blur the scene
		/////////////////////////////////
		renderBuffer.bind(0);
		// --> inputs  T_RENDERBUFFER
		// --> outputs T_BLURBUFFER2
		screenspace.blur(renderBuffer);
		
		screenFBO.bind();
		
		/////////////////////////////////
		// blur background
		/////////////////////////////////
		renderBuffer.bind(0);
		textureman.bind(1, Textureman::T_BLURBUFFER2);
		
		// render to final buffer from renderbuffer (screentex)
		screenspace.terrain();
		
		///  render clouds & particles  ///
		
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		
		glEnable(GL_BLEND);
		glColorMask(1, 1, 1, 0);
		
		/// update particles ///
		particleSystem.renderUpdate();
		/// render particles ///
		particleSystem.render(snapWX, snapWZ);
		
		/// render netplayer nametags ///
		netplayers.renderNameTags();
		
		glColorMask(1, 1, 1, 1);
		glDisable(GL_BLEND);
		
		screenFBO.unbind();
		
	} // render scene
	
}
