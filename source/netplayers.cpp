#include "netplayers.hpp"

#include <library/log.hpp>
#include <library/opengl/oglfont.hpp>
#include <library/opengl/opengl.hpp>
#include "blockmodels.hpp"
#include "camera.hpp"
#include "network.hpp"
#include "sectors.hpp"
#include "sun.hpp"
#include "shaderman.hpp"
#include "textureman.hpp"
#include "torchlight.hpp"
#include "vertex_player.hpp"
#include <cmath>

using namespace library;

namespace cppcraft
{
	NetPlayers netplayers;
	
	void NetPlayers::remove(unsigned int uid)
	{
		int index = indexByUID(uid);
		if (index != -1)
		{
			std::swap(players[index], players.back());
			players.pop_back();
		}
	}
	
	int NetPlayers::indexByUID(unsigned int uid) const
	{
		for (size_t i = 0; i < players.size(); i++)
		{
			if (players[i].getUserID() == uid) return i;
		}
		return -1;
	}
	NetPlayer* NetPlayers::playerByUID(unsigned int uid)
	{
		for (NetPlayer& p : players)
		{
			if (p.getUserID() == uid) return &p;
		}
		return nullptr;
	}
	
	void NetPlayers::updatePosition(NetPlayer* np, UnpackCoordF& position)
	{
		np->setPosition(position.wc, position.bc);
		np->moving = true;
	}
	void NetPlayers::stopMoving(NetPlayer* np)
	{
		np->moving = false;
	}
	
	void NetPlayers::updateRotation(NetPlayer* p, library::vec2& rotation)
	{
		p->setRotation(rotation);
	}
	
	void NetPlayers::positionSnapshots(int wx, int wz)
	{
		for (NetPlayer& p : players)
		{
			p.gxyz = vec3(
				(p.wc.x - wx) * Sector::BLOCKS_XZ + p.pos.x,
				(p.wc.y)      * Sector::BLOCKS_Y  + p.pos.y,
				(p.wc.z - wz) * Sector::BLOCKS_XZ + p.pos.z
			);
		}
	}
	
	void NetPlayers::createTestPlayer()
	{
		NetPlayer nplayer(1234, "Test");
		
		w_coord wc;
		wc.x = world.getWX() + Sectors.getXZ() / 2;
		wc.z = world.getWZ() + Sectors.getXZ() / 2 - 1;
		wc.y = Sectors.getY() / 2 + 6;
		
		vec3 pos(-6, 2.45, 9);
		
		nplayer.setPosition(wc, pos);
		
		players.push_back(nplayer);
	}
	void NetPlayers::modulateTestPlayer(double frametime)
	{
		NetPlayer* pl = playerByUID(1234);
		if (pl)
		{
			pl->setRotation(vec2(cos(frametime * 0.04), frametime * 0.08));
		}
	}
	
	static const double PI = 4 * atan(1);
	static const double PI2 = PI * 2;
	
	float interpolate_angle(double a1, double a2, double weight, double maxdelta)
	{
		float delta = (a2 + PI2) - (a1 + PI2);
		
		if (fabs(delta) > PI)
		{
			if (delta > 0) a2 += PI2;
			else           a2 -= PI2;
		}
		
		if (fabs(delta) > maxdelta)
			return a1 * (1.0 - weight) + a2 * weight;
		else
			return a1;
	}
	
	void NetPlayers::renderPlayers(double frameCounter, double dtime)
	{
		/// testing ///
		netplayers.modulateTestPlayer(frameCounter);
		
		// playermodel texture
		textureman[Textureman::T_PLAYERMODELS].bind(0);
		
		// playermodel shader
		Shader& shd = shaderman[Shaderman::PLAYERMODEL];
		shd.bind();
		
		// common stuff
		shd.sendVec3 ("lightVector", thesun.getRealtimeAngle());
		shd.sendFloat("daylight",    thesun.getRealtimeDaylight());
		shd.sendFloat("frameCounter", frameCounter);
		
		shd.sendFloat("modulation", torchlight.getModulation(frameCounter));
		
		int count = blockmodels.skinCubes.totalCount();
		
		// head mesh
		if (vao.isGood() == false)
		{
			// create ccube and copy all centerCube vertices to ccube
			player_vertex_t* ccube = new player_vertex_t[count];
			blockmodels.skinCubes.copyAll(ccube);
			
			// upload data
			vao.begin(sizeof(player_vertex_t), count, ccube);
			vao.attrib(0, 3, GL_FLOAT, false, offsetof(player_vertex_t, x));
			vao.attrib(1, 3, GL_BYTE,  true,  offsetof(player_vertex_t, nx));
			vao.attrib(2, 4, GL_BYTE,  false, offsetof(player_vertex_t, u));
			vao.end();
			
			delete[] ccube;
		}
		
		// render each player
		for (size_t i = 0; i < players.size(); i++)
		{
			mat4 matview = camera.getViewMatrix();
			matview.translate(players[i].gxyz);
			
			//logger << Log::INFO << "Rendering player: " << i << " at " << players[i].gxyz << Log::ENDL;
			vao.bind();
			
			// render head
			mat4 matv = matview;
			matv.rotateZYX(0.0, players[i].rotation.y, 0.0);
			matv.rotateZYX(players[i].rotation.x, 0.0, 0.0);
			shd.sendMatrix("matview", matv);
			
			vao.render(GL_QUADS, 0, 24);
			
			const float maxdelta = (players[i].moving) ? 0.05 : 0.8;
			const float weight = 0.25 * dtime;
			
			// interpolate body rotation
			players[i].bodyrot = interpolate_angle(players[i].bodyrot, players[i].rotation.y, weight, maxdelta);
			
			// render chest
			matv = matview;
			matv.translate_xy(0, -0.72);
			matv.rotateZYX(0.0, players[i].bodyrot, 0.0);
			shd.sendMatrix("matview", matv);
			
			vao.render(GL_QUADS, 24, 24);
			
			bool moving = players[i].moving;
			
			// render hands
			matv = matview;
			matv.rotateZYX(0.0, players[i].bodyrot, 0.0);
			matv.translate_xy(-0.25, -0.27);
			if (moving)
				matv.rotateZYX(sin(frameCounter * 0.1), 0.0, 0.0);
			shd.sendMatrix("matview", matv);
			
			vao.render(GL_QUADS, 48, 24);
			
			matv = matview;
			matv.rotateZYX(0.0, players[i].bodyrot, 0.0);
			matv.translate_xy(+0.25, -0.27);
			if (moving)
				matv.rotateZYX(sin(-frameCounter * 0.1), 0.0, 0.0);
			shd.sendMatrix("matview", matv);
			
			vao.render(GL_QUADS, 48, 24);
			
			// render legs
			matv = matview;
			matv.rotateZYX(0.0, players[i].bodyrot, 0.0);
			matv.translate_xy(-0.11, -0.75);
			if (moving)
				matv.rotateZYX(-sin(frameCounter * 0.1), 0.0, 0.0);
			shd.sendMatrix("matview", matv);
			
			vao.render(GL_QUADS, 72, 24);
			
			matv = matview;
			matv.rotateZYX(0.0, players[i].bodyrot, 0.0);
			matv.translate_xy(+0.11, -0.75);
			if (moving)
				matv.rotateZYX(sin(frameCounter * 0.1), 0.0, 0.0);
			shd.sendMatrix("matview", matv);
			
			vao.render(GL_QUADS, 72, 24);
			
		} // render each player
	} // render players
}
