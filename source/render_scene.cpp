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

#include "camera.hpp"
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
#include "sector.hpp"
#include "shaderman.hpp"
#include "textureman.hpp"
#include "threading.hpp"
#include "world.hpp"
#include <cmath>

using namespace library;

namespace cppcraft
{
	FBO skyFBO;
	FBO sceneFBO;
	FBO reflectionFBO;
	FBO underwaterFBO;
	FBO fboResolveColor;
	FBO fboResolveNormals;
	FBO fogFBO, finalFBO;
	
	void SceneRenderer::init(Renderer& renderer)
	{
		// initialize members
		this->lastTime = 0.0;
		this->iplayerX = player.X;
		this->iplayerY = player.Y;
		this->iplayerZ = player.Z;
		
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
		if (gameconf.ssao)
		{
			sceneFBO.attachColor(1, textureman[Textureman::T_NORMALBUFFER]);
		}
		sceneFBO.attachDepth(textureman[Textureman::T_DEPTHBUFFER]);
		
		// the FBO we copy the main scene to before rendering water
		underwaterFBO.create();
		underwaterFBO.bind();
		underwaterFBO.attachColor(0, textureman[Textureman::T_UNDERWATERMAP]);
		underwaterFBO.attachDepth(textureman[Textureman::T_UNDERWDEPTH]);
		
		// multisampling resolver
		if (gameconf.multisampling)
		{
			logger << Log::INFO << "* Multisampling: " << gameconf.multisampling << "x" << Log::ENDL;
			fboResolveColor.create();
			fboResolveColor.bind();
			fboResolveColor.attachColor(0, textureman[Textureman::T_FINALBUFFER]);
			fboResolveColor.attachDepth(textureman[Textureman::T_FINALDEPTH]);
			
			if (gameconf.ssao)
			{
				fboResolveNormals.create();
				fboResolveNormals.bind();
				fboResolveNormals.attachColor(0, textureman[Textureman::T_FINALNORMALS]);
			}
		}
		
		fogFBO.create();
		fogFBO.bind();
		fogFBO.attachColor(0, textureman[Textureman::T_RENDERBUFFER]);
		
		finalFBO.create();
		finalFBO.bind();
		finalFBO.attachColor(0, textureman[Textureman::T_FINALBUFFER]);
		
		if (gameconf.multisampling)
			finalFBO.attachDepth(textureman[Textureman::T_FINALDEPTH]);
		else
			finalFBO.attachDepth(textureman[Textureman::T_DEPTHBUFFER]);
		
		// reflection FBO & init
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
		vec3 sceneSize = vec3(sceneTex.getWidth(), sceneTex.getHeight(), renderer.getScreen().getAspect());
		
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
					GL_COLOR_BUFFER_BIT, GL_NEAREST);
		
		
		/// /// /// /// /// /// /// /// /// /// /// /// /// /// /// /// /// ///
		/// render physical scene w/depth
		/// /// /// /// /// /// /// /// /// /// /// /// /// /// /// /// /// ///
		
		sceneFBO.bind();
		glViewport(0, 0, sceneTex.getWidth(), sceneTex.getHeight());
		
		if (gameconf.ssao)
		{
			std::vector<int> dbuffers;
			dbuffers.push_back(GL_COLOR_ATTACHMENT0);
			dbuffers.push_back(GL_COLOR_ATTACHMENT1);
			sceneFBO.drawBuffers(dbuffers);
		}
		
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glDepthMask(GL_TRUE);
		
		// clear depth texture (or depth renderbuffer)
		glClear(GL_DEPTH_BUFFER_BIT);
		
		// disable double-sided faces
		glEnable(GL_CULL_FACE);
		
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
				
				double WEIGHT = std::min(1.0, 0.15 / (renderer.FPS / 120.0));
				//logger << Log::INFO << WEIGHT << " <-- " << 120.0 / renderer.FPS << Log::ENDL;
				
				float dist = distance(vec3(iplayerX, iplayerY, iplayerZ), vec3(player.snapX, player.snapY, player.snapZ));
				
				// (cheap) movement interpolation
				iplayerY = iplayerY * (1.0 - WEIGHT) + player.snapY * WEIGHT;
				
				int dx = snapWX - world.getWX();
				int dz = snapWZ - world.getWZ();
				if (abs(dx) + abs(dz) < 3)
				{
					iplayerX = (iplayerX + dx * 16) * (1.0 - WEIGHT) + player.snapX * WEIGHT;
					iplayerZ = (iplayerZ + dz * 16) * (1.0 - WEIGHT) + player.snapZ * WEIGHT;
				}
				else
				{
					iplayerX = player.snapX;
					iplayerZ = player.snapZ;
				}
				this->playerMoved = player.JustMoved;
				
				this->playerX = iplayerX;
				this->playerY = iplayerY;
				this->playerZ = iplayerZ;
				this->playerSectorX = (int)playerX / Sector::BLOCKS_XZ;
				this->playerSectorZ = (int)playerZ / Sector::BLOCKS_XZ;
				
				underwater = plogic.FullySubmerged != PlayerLogic::PS_None;
				
				frustumRecalc = (dist > 0.01) || camera.recalc;
				camera.recalc = false;
			}
			mtx.playermove.unlock();
			
			if (frustumRecalc)
			{
				/// update matview matrix using player snapshot ///
				camera.setWorldOffset(playerX, playerY, playerZ);
				reflectionCamera.setWorldOffset(playerX, playerY, playerZ);
			}
			
			/// world coordinate snapshots ///
			snapWX = world.getWX();
			snapWZ = world.getWZ();
			
			/// update minimap ///
			minimap.update(playerX, playerZ);
			
			/// set player positions ///
			netplayers.positionSnapshots(snapWX, snapWZ, renderer.dtime);
			
			/// camera deviations ///
			double camDev = cameraDeviation(renderer.frametick, renderer.dtime);
			// modulate playerY when delta is high enough
			playerY += camDev;
			// update frustum if there was a change
			frustumRecalc |= (fabs(camDev) > 0.001);
			
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
		
		// compress rendering queue to minimal size by occlusion culling
		compressRenderingQueue();
		
		/// /// /// /// /// /// /// /// /// /// /// /// /// /// /// /// /// ///
		
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
				
				glDisable(GL_DEPTH_TEST);
				glDepthMask(GL_FALSE);
				//glEnable(GL_CULL_FACE);
				
				// render sky (atmosphere, sun, moon, clouds)
				skyrenderer.render(reflectionCamera, playerY - RenderConst::WATER_LEVEL, renderer.frametick, 1);
				
				if (gameconf.reflectTerrain)
				{
					glEnable(GL_DEPTH_TEST);
					glDepthMask(GL_TRUE);
					glClear(GL_DEPTH_BUFFER_BIT);
					
					renderReflectedScene(renderer, reflectionCamera);
				}
				
				// unbind and return to normal viewport
				reflectionFBO.unbind();
				glViewport(0, 0, sceneTex.getWidth(), sceneTex.getHeight());
			}
		}
		
		#ifdef TIMING
		Timer timerScene;
		timerScene.startNewRound();
		#endif
		
		//glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);
		//glEnable(GL_SAMPLE_COVERAGE);
		
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
		;{
			Texture& underwaterTex = textureman[Textureman::T_UNDERWATERMAP];
			sceneFBO.blitTo(underwaterFBO, 
						sceneTex.getWidth(), sceneTex.getHeight(), 
						underwaterTex.getWidth(), underwaterTex.getHeight(), 
						GL_COLOR_BUFFER_BIT, GL_LINEAR);
			sceneFBO.blitTo(underwaterFBO, 
						sceneTex.getWidth(), sceneTex.getHeight(), 
						underwaterTex.getWidth(), underwaterTex.getHeight(), 
						GL_DEPTH_BUFFER_BIT, GL_NEAREST);
		}
		sceneFBO.bind();
		
		// finally, render scene water
		renderSceneWater(renderer);
		
		#ifdef TIMING
		logger << Log::INFO << "Time spent on scene: " << timerScene.getDeltaTime() * 1000.0 << Log::ENDL;
		#endif
		
		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);
		glDepthMask(GL_FALSE);
		
		/////////////////////////////////
		// resolve super/multisampling
		/////////////////////////////////
		if (gameconf.supersampling > 1)
		{
			glViewport(0, 0, renderBuffer.getWidth(), renderBuffer.getHeight());
			screenspace.renderSuperSampling(textureman[Textureman::T_SCENEBUFFER], textureman[Textureman::T_FINALBUFFER]);
			textureman.bind(0, Textureman::T_FINALBUFFER);
			textureman.bind(1, Textureman::T_SKYBUFFER);
			textureman.bind(2, Textureman::T_DEPTHBUFFER);
			if (gameconf.ssao)
			{
				screenspace.renderSuperSampling(textureman[Textureman::T_NORMALBUFFER], textureman[Textureman::T_FINALNORMALS]);
				textureman.bind(3, Textureman::T_FINALNORMALS);
			}
		}
		else if (gameconf.multisampling)
		{
			sceneFBO.blitTo(fboResolveColor, sceneTex.getWidth(), sceneTex.getHeight(), GL_COLOR_BUFFER_BIT, GL_LINEAR);
			sceneFBO.blitTo(fboResolveColor, sceneTex.getWidth(), sceneTex.getHeight(), GL_DEPTH_BUFFER_BIT, GL_NEAREST);
			
			textureman.bind(0, Textureman::T_FINALBUFFER);
			textureman.bind(1, Textureman::T_SKYBUFFER);
			textureman.bind(2, Textureman::T_FINALDEPTH);
			
			if (gameconf.ssao)
			{
				glBindFramebuffer(GL_READ_FRAMEBUFFER, sceneFBO.getHandle());
				glReadBuffer(GL_COLOR_ATTACHMENT0 + 1);
				
				glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fboResolveNormals.getHandle());
				glBlitFramebuffer(0, 0, sceneTex.getWidth(), sceneTex.getHeight(), 0, 0, sceneTex.getWidth(), sceneTex.getHeight(), GL_COLOR_BUFFER_BIT, GL_LINEAR);
				
				glReadBuffer(GL_COLOR_ATTACHMENT0);
				// add normals to slot 3
				textureman.bind(3, Textureman::T_FINALNORMALS);
			}
		}
		else
		{
			textureman.bind(0, Textureman::T_SCENEBUFFER);
			textureman.bind(1, Textureman::T_SKYBUFFER);
			textureman.bind(2, Textureman::T_DEPTHBUFFER);
			if (gameconf.ssao)
			{
				textureman.bind(3, Textureman::T_NORMALBUFFER);
			}
		}
		
		/////////////////////////////////
		// create fog based on depth
		/////////////////////////////////
		// --> inputs  T_SCENEBUFFER (or T_FINALBUFFER)
		// --> outputs T_RENDERBUFFER
		fogFBO.bind();
		screenspace.fog(renderer.frametick);
		
		/////////////////////////////////
		// blur the scene
		/////////////////////////////////
		// --> inputs  T_RENDERBUFFER
		// --> outputs T_BLURBUFFER2
		screenspace.blur(renderBuffer);
		
		finalFBO.bind();
		
		/////////////////////////////////
		// apply blur to background
		/////////////////////////////////
		renderBuffer.bind(0);
		textureman.bind(1, Textureman::T_BLURBUFFER2);
		
		// render to final buffer from renderbuffer
		// --> inputs  T_RENDERBUFFER
		// --> outputs T_FINALBUFFER
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
		
		finalFBO.unbind();
		
	} // render scene
	
}
