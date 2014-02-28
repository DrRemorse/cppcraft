#include "netplayers.hpp"

#include <library/log.hpp>
#include <library/opengl/oglfont.hpp>
#include <library/opengl/opengl.hpp>
#include "blockmodels.hpp"
#include "camera.hpp"
#include "network.hpp"
#include "sector.hpp"
#include "sun.hpp"
#include "shaderman.hpp"
#include "textureman.hpp"
#include "torchlight.hpp"
#include "vertex_block.hpp"

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
	
	void NetPlayers::updatePosition(NetPlayer* p, UnpackCoordF& position)
	{
		p->setPosition(position.wc, position.bc);
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
	
	void NetPlayers::renderPlayers(double frameCounter, double dtime)
	{
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
		
		int count = blockmodels.centerCube.totalCount();
		
		// head mesh
		if (vao.isGood() == false)
		{
			// create ccube and copy all centerCube vertices to ccube
			vertex_t* ccube = new vertex_t[count];
			blockmodels.centerCube.copyAll(ccube);
			
			// set ccube tile id
			vertex_t* v = ccube;
			
			for (int face = 0; face < 6; face++)
			for (int vert = 0; vert < 4; vert++)
			{
				switch (face)
				{
				case 0: // front
					v->w = 0; break;
				case 1: // back
					v->w = 2; break;
				case 2: // top
					v->w = 3; break;
				case 3: // bottom
					v->w = 11; break;
				case 4:
				case 5: // right & left
					v->w = 1; break;
				}
				v->c = 0;
				v++;
			}
			
			// upload data
			vao.begin(sizeof(vertex_t), count, ccube);
			vao.attrib(0, 3, GL_SHORT, false, offsetof(vertex_t, x));
			vao.attrib(1, 3, GL_BYTE, true, offsetof(vertex_t, nx));
			vao.attrib(2, 4, GL_SHORT, false, offsetof(vertex_t, u));
			vao.attrib(3, 4, GL_UNSIGNED_BYTE, true, offsetof(vertex_t, biome));
			vao.attrib(4, 4, GL_UNSIGNED_BYTE, true, offsetof(vertex_t, c));
			vao.attrib(5, 4, GL_UNSIGNED_BYTE, true, offsetof(vertex_t, c) + 4);
			vao.end();
			
			delete[] ccube;
		}
		
		// render each player
		for (size_t i = 0; i < players.size(); i++)
		{
			mat4 matview = camera.getViewMatrix();
			matview.translate(players[i].gxyz);
			matview.rotateZYX(players[i].rotation.x, players[i].rotation.y, 0.0);
			matview *= mat4(0.6);
			shd.sendMatrix("matview", matview);
			
			//logger << Log::INFO << "Rendering player: " << i << " at " << players[i].gxyz << Log::ENDL;
			vao.bind();
			vao.render(GL_QUADS);
		}
	}
}
