#include "render_scene.hpp"

#include "library/log.hpp"
#include "library/opengl/opengl.hpp"
#include "library/opengl/fbo.hpp"
#include "columns.hpp"
#include "drawq.hpp"
#include "camera.hpp"
#include "player.hpp"
#include "player_logic.hpp"
#include "renderman.hpp"
#include "render_fs.hpp"
#include "sectors.hpp"
#include "shaderman.hpp"
#include "sun.hpp"
#include "textureman.hpp"
#include "torchlight.hpp"
#include <cmath>

using namespace library;

namespace cppcraft
{
	static const double PI = 4 * atan(1);
	
	void SceneRenderer::initTerrain()
	{
		// initialize column drawing queue
		drawq.init();
	}
	
	void SceneRenderer::recalculateFrustum()
	{
		vec3 look = player.getLookVector();
		
		// recalculate view & mvp matrix
		// view matrix (rotation + translation)
		camera.setTranslation(-playerX, -playerY, -playerZ);
		
		recalculateFrustum(camera, drawq, look);
		
		// set reflection camera view
		Matrix matref = camera.getViewMatrix();
		matref.translated(0, RenderConst::WATER_LEVEL*2, 0);
		matref *= Matrix(1.0, -1.0, 1.0);
		
		reflectionCamera.setRotationMatrix(matref.rotation());
		reflectionCamera.setViewMatrix(matref);
		
		look.y = -look.y;
		
		recalculateFrustum(reflectionCamera, reflectionq, look);
	}
	
	void SceneRenderer::recalculateFrustum(Camera& camera, DrawQueue& drawq, const vec3& look)
	{
		// recalculate camera frustum
		camera.calculateFrustum();
		
		static const int safety_border = 2;
		#define visibility_border  DrawQueue::visibility_border
		
		static const float half_fov = 0.65; // sin(30 * degToRad) = +/- 0.5
		
		// major direction scheme
		int xstp = 1;
		int x0, x1;
		if (look.x >= 0)
		{
			if (look.x > half_fov)
				x0 = playerSectorX - safety_border;
			else
				x0 = playerSectorX - camera.cameraViewSectors;
			
			x1 = playerSectorX + camera.cameraViewSectors;
			
			if (x0 < visibility_border)
				x0 = visibility_border;
			if (x1 >= Sectors.getXZ() - visibility_border)
				x1 = Sectors.getXZ()-1-visibility_border;
		}
		else
		{
			x1 = playerSectorX - camera.cameraViewSectors;
			
			if (look.x < -half_fov)
				x0 = playerSectorX + safety_border;
			else
				x0 = playerSectorX + camera.cameraViewSectors;
			
			if (x1 < visibility_border)
				x1 = visibility_border;
			if (x0 >= Sectors.getXZ()-visibility_border)
				x0 = Sectors.getXZ()-1-visibility_border;
			
			xstp = -1;
		}
		
		int zstp = 1;
		int z0, z1;
		if (look.z >= 0)
		{
			if (look.z > half_fov)
				z0 = playerSectorZ - safety_border;
			else
				z0 = playerSectorZ - camera.cameraViewSectors;
			
			z1 = playerSectorZ + camera.cameraViewSectors;
			
			if (z0 < visibility_border)
				z0 = visibility_border;
			if (z1 >= Sectors.getXZ()-visibility_border)
				z1 = Sectors.getXZ()-1-visibility_border;
		}
		else
		{
			z1 = playerSectorZ - camera.cameraViewSectors;
			
			if (look.z < -half_fov)
				z0 = playerSectorZ + safety_border;
			else
				z0 = playerSectorZ + camera.cameraViewSectors;
			
			if (z1 < visibility_border)
				z1 = visibility_border;
			if (z0 >= Sectors.getXZ()-visibility_border)
				z0 = Sectors.getXZ()-1-visibility_border;
			
			zstp = -1;
		}
		
		int ystp = (look.y < 0.0) ? -1 : 1;
		
		int majority = 0;
		
		// determine iteration direction
		if (fabsf(look.x) > fabsf(look.z))
		{
			if (look.x >= 0)
				majority = 0; // +x
			else
				majority = 1; // -x
		}
		else if (look.z >= 0)
			majority = 2; // +z
		else
			majority = 3; // -z
		
		// reset drawqing queue
		drawq.reset();
		
		// set frustum culling settings
		DrawQueue::rendergrid_t rv;
		rv.xstp = xstp;
		rv.ystp = ystp;
		rv.zstp = zstp;
		rv.majority = majority;
		rv.playerY  = playerY;
		rv.frustum = &camera.getFrustum();
		
		// start at roomsize 1, avoiding "everything"
		drawq.uniformGrid(rv, x0, x1, z0, z1, 1);
	}
	
	void SceneRenderer::compressRenderingQueue()
	{
		// reset after 2nd run
		if (camera.needsupd == 2) camera.needsupd = 0;
		
		// update and compress the draw queue
		// by counting visible entries for each shader line, and re-adding as we go
		for (int i = 0; i < RenderConst::MAX_UNIQUE_SHADERS; i++)
		{
			int count = drawq[i].count();
			// clear, which does only 1 thing: count = 0
			drawq[i].clear();
			
			// loop through this shader line
			for (int j = 0; j < count; j++)
			{
				Column* cv = drawq[i].get(j);
				
				// get/set occlusion status
				if (cv->occluded[i] == 1)
				{
					GLuint occlusion_result;
					glGetQueryObjectuiv(cv->occlusion[i], GL_QUERY_RESULT_AVAILABLE, &occlusion_result);
					
					if (occlusion_result)
					{
						// get result immediately
						glGetQueryObjectuiv(cv->occlusion[i], GL_QUERY_RESULT, &occlusion_result);
						
						if (occlusion_result)
						{
							// add if any samples passed
							cv->occluded[i] = 0;
						}
						else cv->occluded[i] = 2;
					}
					else camera.needsupd = 2; // we need to update again :(
				}
				/*else if (cv->occluded[i] == 3)
				{
					// uncontested from last frame --> 1
					cv->occluded[i] = 1;
				}*/
				
				// finally, as long as not completely occluded/discarded
				if (cv->occluded[i] != 2)
				{
					// add to new position, effectively compressing
					// and linearizing queue internally
					drawq[i].add(cv);
				}
				
			}
			
		} // next shaderline
		
	} // sort render queue
	
	void renderColumn(Column* cv, int i, vec3& position, GLint loc_vtrans, double dtime)
	{
		if (position.x != cv->pos.x || 
			position.y != cv->pos.y || 
			position.z != cv->pos.z)
		{
			// translate to new position
			glUniform3fv(loc_vtrans, 1, &cv->pos.x);
			
			// remember position
			position = cv->pos;
			
			// cool effect
			if (cv->pos.y < 0.0)
			{
				cv->pos.y += 0.25 * dtime;
				if (cv->pos.y > 0.0) cv->pos.y = 0.0;
			}
		}
		glBindVertexArray(cv->vao);
		glDrawArrays(GL_QUADS, cv->bufferoffset[i], cv->vertices[i]);
	}
	
	void handleSceneUniforms(
			double frameCounter, 
			Shader& shd, 
			GLint& location, 
			GLint& loc_vtrans, 
			vec3& position,
			const Matrix& matview
		)
	{
		// bind appropriate shader
		shd.bind();
		
		// camera updates
		if (camera.ref)
		{
			// modelview matrix
			shd.sendMatrix("matview", matview);
		}
		
		// translation
		loc_vtrans = shd.getUniform("vtrans");
		position = vec3(-1);
		
		// common stuff
		shd.sendVec3 ("lightVector", thesun.getRealtimeAngle());
		shd.sendFloat("daylight",    thesun.getRealtimeDaylight());
		shd.sendFloat("frameCounter", frameCounter);
		
		shd.sendFloat("modulation", torchlight.getModulation(frameCounter));
		
		// texrange
		location = shd.getUniform("texrange");
	}
	
	void SceneRenderer::renderScene(Renderer& renderer, library::Camera& renderCam)
	{
		GLint loc_vtrans, location;
		vec3  position(-1);
		
		// bind skybox at slot 4
		textureman.bind(4, Textureman::T_SKYBOX);
		
		// bind standard shader
		handleSceneUniforms(renderer.frametick, 
							shaderman[Shaderman::STD_BLOCKS], 
							location, 
							loc_vtrans, 
							position, renderCam.getViewMatrix());
		// check for errors
		if (OpenGL::checkError())
		{
			logger << Log::ERR << "Renderer::renderScene(): OpenGL error. Line: " << __LINE__ << Log::ENDL;
			throw std::string("Renderer::renderScene(): OpenGL state error");
		}
		
		// render all nonwater shaders
		int nonwaterShaders = (int) RenderConst::MAX_UNIQUE_SHADERS - 1;
		
		for (int i = 0; i < nonwaterShaders; i++)
		{
			switch (i)
			{
			case RenderConst::TX_REPEAT: // repeatable solids (most terrain)
				
				// enable face culling
				glEnable(GL_CULL_FACE);
				// change to repeatable textures
				textureman.bind(0, Textureman::T_BIG_DIFF);
				textureman.bind(1, Textureman::T_BIG_TONE);
				break;
				
			case RenderConst::TX_SOLID: // solid stuff (most blocks)
				
				// change to clamped textures
				textureman.bind(0, Textureman::T_DIFFUSE);
				textureman.bind(1, Textureman::T_TONEMAP);
				break;
				
			case RenderConst::TX_TRANS: // culled alpha (tree leafs etc.)
				
				// change shader-set
				handleSceneUniforms(renderer.frametick, 
									shaderman[Shaderman::CULLED_BLOCKS], 
									location, 
									loc_vtrans, 
									position, renderCam.getViewMatrix());
				break;
				
			case RenderConst::TX_2SIDED: // 2-sided faces (torches, vines etc.)
				
				// disable face culling (for 2-sidedness)
				glDisable(GL_CULL_FACE);
				
				// change shader-set
				handleSceneUniforms(renderer.frametick, 
									shaderman[Shaderman::ALPHA_BLOCKS], 
									location, 
									loc_vtrans, 
									position, renderCam.getViewMatrix());
				// safe to increase step from this -->
				if (drawq[i].count() == 0)
				{
					i += 1;
					break;
				}
				// <-- safe to increase step from this
				
				// set texrange
				glUniform1i(location, i);
				break;
				
			case RenderConst::TX_CROSS:
				// set new texrange
				glUniform1i(location, i);
				
				break;
			}
			
			if (camera.needsupd == 1)
			{
				// render and count visible samples
				for (int j = 0; j < drawq[i].count(); j++)
				{
					Column* cv = drawq[i].get(j);
					
					// start counting samples passed
					glBeginQuery(GL_ANY_SAMPLES_PASSED, cv->occlusion[i]);
					
					renderColumn(cv, i, position, loc_vtrans, renderer.dtime);
					
					// end counting
					glEndQuery(GL_ANY_SAMPLES_PASSED);
					
					// set this as having been sampled
					cv->occluded[i] = 1;
				}
			}
			else
			{
				// direct render
				for (int j = 0; j < drawq[i].count(); j++)
				{
					Column* cv = drawq[i].get(j);
					renderColumn(cv, i, position, loc_vtrans, renderer.dtime);
				}
			}
			
		} // next shaderline
		
	}
	
	void SceneRenderer::renderReflectedScene(Renderer& renderer, library::Camera& renderCam)
	{
		GLint loc_vtrans, location;
		vec3  position(-1);
		
		// bind standard shader
		handleSceneUniforms(renderer.frametick, 
							shaderman[Shaderman::STD_REFLECT], 
							location, 
							loc_vtrans, 
							position, renderCam.getViewMatrix());
		
		// check for errors
		if (OpenGL::checkError())
		{
			logger << Log::ERR << "Renderer::renderReflectedScene(): OpenGL error. Line: " << __LINE__ << Log::ENDL;
			throw std::string("Renderer::renderReflectedScene(): OpenGL state error");
		}
		
		// render all nonwater shaders
		int nonwaterShaders = (int) RenderConst::MAX_UNIQUE_SHADERS - 1;
		
		for (int i = 0; i < nonwaterShaders; i++)
		{
			switch (i)
			{
			case RenderConst::TX_REPEAT: // repeatable solids (most terrain)
				
				glDisable(GL_CULL_FACE);
				// change to repeatable textures
				textureman.bind(0, Textureman::T_BIG_DIFF);
				textureman.bind(1, Textureman::T_BIG_TONE);
				break;
				
			case RenderConst::TX_SOLID: // solid stuff (most blocks)
				
				// change to clamped textures
				textureman.bind(0, Textureman::T_DIFFUSE);
				textureman.bind(1, Textureman::T_TONEMAP);
				break;
				
			case RenderConst::TX_2SIDED: // 2-sided faces (torches, vines etc.)
				
				// disable face culling (for 2-sidedness)
				glDisable(GL_CULL_FACE);
				break;
				
			}
			
			// direct render
			for (int j = 0; j < reflectionq[i].count(); j++)
			{
				Column* cv = reflectionq[i].get(j);
				if (cv->aboveWater)
				{
					renderColumn(cv, i, position, loc_vtrans, renderer.dtime);
				}
			}
			
		} // next shaderline
		
	}
	
	void SceneRenderer::renderSceneWater(Renderer& renderer)
	{
		GLint loc_vtrans, location;
		vec3  position(-1);
		
		// bind underwater scene
		textureman.bind(0, Textureman::T_UNDERWATERMAP);
		// bind world-reflection
		textureman.bind(1, Textureman::T_REFLECTION);
		// bind skybox at slot 2
		textureman.bind(2, Textureman::T_SKYBOX);
		
		// water shader
		int i = RenderConst::TX_WATER;
		
		// change shader-set
		handleSceneUniforms(renderer.frametick, 
							shaderman[Shaderman::BLOCKS_WATER], 
							location, 
							loc_vtrans, 
							position, camera.getViewMatrix());
		// send player submerged status
		shaderman[Shaderman::BLOCKS_WATER].sendInteger("playerSubmerged", plogic.FullySubmerged);
		// update world offset
		shaderman[Shaderman::BLOCKS_WATER].sendVec3("worldOffset", camera.getWorldOffset());
		
		// check for errors
		if (OpenGL::checkError())
		{
			logger << Log::ERR << "Renderer::renderSceneWater(): OpenGL error. Line: " << __LINE__ << Log::ENDL;
			throw std::string("Renderer::renderSceneWater(): OpenGL state error");
		}
		
		if (camera.needsupd == 1)
		{
			// render and count visible samples
			for (int j = 0; j < drawq[i].count(); j++)
			{
				Column* cv = drawq[i].get(j);
				
				// start counting samples passed
				glBeginQuery(GL_ANY_SAMPLES_PASSED, cv->occlusion[i]);
				
				renderColumn(cv, i, position, loc_vtrans, renderer.dtime);
				
				// end counting
				glEndQuery(GL_ANY_SAMPLES_PASSED);
				
				// set this as having been sampled
				cv->occluded[i] = 1;
			}
		}
		else
		{
			// direct render
			for (int j = 0; j < drawq[i].count(); j++)
			{
				Column* cv = drawq[i].get(j);
				renderColumn(cv, i, position, loc_vtrans, renderer.dtime);
			}
		}
		
	} // renderSceneWater
	
}
