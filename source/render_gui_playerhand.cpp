#include "render_gui.hpp"

#include "library/log.hpp"
#include "library/bitmap/colortools.hpp"
#include "library/opengl/opengl.hpp"
#include "library/opengl/vao.hpp"
#include "library/math/toolbox.hpp"
#include "library/math/vector.hpp"
#include "camera.hpp"
#include "player.hpp"
#include "player_actions.hpp"
#include "player_logic.hpp"
#include "sun.hpp"
#include "shaderman.hpp"
#include "textureman.hpp"
#include "torchlight.hpp"
#include <cmath>

using namespace library;

namespace cppcraft
{
	static const double PI = 4 * atan(1);
	
	class PlayerHand
	{
		VAO vao;
		vec3 lastHand;
		double lastTime;
		int lastMode;
		
		struct phand_vertex_t
		{
			float x, y, z;
			float nx, ny, nz;
			float u, v;
		};
		
		phand_vertex_t vertices[2][4] = 
		{
			{ // top face
				{ //  xyz       norm       uvw
					1, 1, 1,   0, 1, 0,   0, 1.5
				},
				{
					1, 1, 0,   0, 1, 0,   0, 0.0
				},
				{
					0, 1, 0,   0, 1, 0,   1, 0.0
				},
				{
					0, 1, 1,   0, 1, 0,   1, 1.5
				}
			},
			{ // left face
				{
					0, 1, 1,  -1, 0, 0,   0, 1.5
				},
				{
					0, 1, 0,  -1, 0, 0,   0, 0.0
				},
				{
					0, 0, 0,  -1, 0, 0,   1, 0.0
				},
				{
					0, 0, 1,  -1, 0, 0,   1, 1.5
				}
			}
		};
		
	public:
		PlayerHand();
		void render(double frameCounter);
		
	};
	PlayerHand playerHand;
	
	void GUIRenderer::renderPlayerhand(double frameCounter)
	{
		playerHand.render(frameCounter);
	}
	
	PlayerHand::PlayerHand()
	{
		lastMode = -1;
		lastTime = 0.0;
	}
	
	void PlayerHand::render(double frameCounter)
	{
		const double update_speed = 0.005;
		
		// manipulate hand
		double period = (frameCounter - lastTime) * 0.25 / PI;
		vec3 hand(0.85, -0.75, -1.50);
		int mode = 0;
		
		if (paction.getAction() == PlayerActions::PA_Mineblock)
		{
			// mining animation
			hand.x -= fabs(sin(period * 1.5) * 0.5);
			hand.y += sin(period * 3) * 0.1;
			if (false) //helditem.count)
				hand.z = -1.25 - fabs(sin(period * 1.5)) * 1.25;
			else
				hand.z = -1.25 - fabs(sin(period * 1.5)) * 1.5;
			
			mode = 1;
		}
		else if (paction.getAction() == PlayerActions::PA_Swingtool)
		{
			// swinging animation
			hand.x -= fabs(sin(period * 1.0)) * 0.75;
			hand.y = -0.5 - period * 0.1;
			
			hand.z = -1.25 - fabs(sin(period * 1.0)) * 1.2;
			mode = 2;
		}
		else if (paction.getAction() == PlayerActions::PA_Cooldown)
		{
			// cooldown animation
			hand.y += sin(period / PI * 2) * 0.1;
			if (false) //helditem.count)
				hand.z = -1.25 - fabs(sin(period)) * 1.25;
			else
				hand.z = -1.25 - fabs(sin(period)) * 1.5;
			
			if (lastMode != mode) lastTime = frameCounter - paction.cooldownTime;
			mode = 3;
		}
		else
		{
			if (plogic.FullySubmerged != PlayerLogic::PS_None)
			{
				hand.y += sin(period * 1.0) * 0.1;
				hand.z -= sin(period * 0.5) * 0.1;
				mode = 15;
			}
			else if (plogic.freefall)
			{
				float dy = toolbox::clamp(-0.4, 0.4, -player.pay);
				if (plogic.Submerged != PlayerLogic::PS_None) dy = 0.0;
				
				hand.y += dy + sin(period * 1.0) * 0.1;
				hand.z -=      sin(period * 0.5) * 0.1;
				mode = 16;
			}
			else if (player.JustMoved)
			{	
				// normal animation (based on player speed)
				
				if (player.Flying)
				{
					hand.y += sin(period * 2.0) * 0.1;
					hand.z -= sin(period * 1.0) * 0.1;
					mode = 18;
				}
				else if (plogic.movestate == PMS_Sprint)
				{
					hand.y += sin(period * 3.00) * 0.1;
					hand.z -= sin(period * 1.50) * 0.1;
					mode = 19;
				}
				else
				{
					hand.y += sin(period * 2.0) * 0.1;
					hand.z -= sin(period * 1.0) * 0.1;
					mode = 20;
				}
			}
			else if (plogic.movestate == PMS_Crouch)
			{
				// resting crouch
				hand.y += sin(period * 1.0) * 0.05;
				hand.z -= sin(period * 0.5) * 0.05;
				mode = 21;
			}
			else
			{
				// resting normal
				hand.y += sin(period * 0.25) * 0.1;
				hand.z -= sin(period * 0.5) * 0.05;
				mode = 22;
			}
			
		} // player action
		
		if (lastMode == -1 || lastMode == mode)
		{
			lastHand = hand.mix(lastHand, 0.5);
			
			if (lastMode == -1)
			{
				lastMode = mode;
				lastTime = frameCounter;
			}
		}
		else
		{
			// interpolate slowly to move the hand to the new position
			lastHand = hand.mix(lastHand, 0.75);
			// only set new mode once we get close enough to the new position
			if (fabsf(hand.y - lastHand.y) < 0.01 &&
				fabsf(hand.z - lastHand.z) < 0.01)
			{
				lastMode = mode;
				lastTime = frameCounter;
			}
		}
		
		// render player hand
		Shader& shd = shaderman[Shaderman::PLAYERHAND];
		shd.bind();
		
		shd.sendVec3("lightVector", thesun.getRealtimeAngle());
		shd.sendFloat("daylight", thesun.getRealtimeDaylight());
		
		// send rotation matrix when camera has been rotated
		if (camera.rotated)
		{
			shd.sendMatrix("matrot", camera.getRotationMatrix());
		}
		
		// view matrix
		Matrix matview(1.0);
		matview.translate(lastHand.x, lastHand.y, lastHand.z);
		matview *= Matrix(0.4, 0.3, 2.0);
		
		shd.sendMatrix("matview", matview);
		
		// player shadow & torchlight color
		// convert shadow color to a 4-vector
		vec4 color = library::colorToVector(plogic.shadowColor);
		shd.sendVec4("lightdata", color);
		color = library::colorToVector(plogic.torchColor);
		shd.sendVec4("torchlight", color);
		// torchlight modulation
		shd.sendFloat("modulation", torchlight.getModulation(frameCounter));
		
		// playerskin * scanline + 9 (which is ARM tile)
		shd.sendFloat("handTileID", 9);
		
		// bind player models texture
		textureman.bind(0, Textureman::T_PLAYERMODELS);
		
		// update vertex data
		if (vao.isGood() == false)
		{
			vao.begin(sizeof(phand_vertex_t), 8, &vertices[0][0]);
			vao.attrib(0, 3, GL_FLOAT, GL_FALSE, offsetof(phand_vertex_t, x));
			vao.attrib(1, 3, GL_FLOAT, GL_FALSE, offsetof(phand_vertex_t, nx));
			vao.attrib(2, 2, GL_FLOAT, GL_FALSE, offsetof(phand_vertex_t, u));
			vao.end();
		}
		
		// finally, render...
		vao.render(GL_QUADS);
	}
}