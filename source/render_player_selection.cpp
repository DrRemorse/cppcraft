#include "render_player_selection.hpp"

#include "library/math/vector.hpp"
#include "library/opengl/opengl.hpp"
#include "library/opengl/vao.hpp"
#include "blocks.hpp"
#include "blockmodels.hpp"
#include "vertex_block.hpp"
#include "camera.hpp"
#include "player.hpp"
#include "player_logic.hpp"
#include "renderconst.hpp"
#include "shaderman.hpp"
#include "textureman.hpp"
#include "threading.hpp"

using namespace library;

namespace cppcraft
{
	class PlayerSelection
	{
		VAO vao;
		
	public:
		void render();
	};
	PlayerSelection playerSelection;
	
	void renderPlayerSelection()
	{
		playerSelection.render();
	}
	
	block_t PlayerLogic::determineSelectionFacing(Block& block, vec3& ray, vec3& fracs, float stepSize)
	{
		block_t id = block.getID();
		
		// determine selection-box facing value
		if (id == _LADDER || id == _VINES)
		{
			return 10;
		}
		else if (id == _WOODPOLE)
		{
			if (fracs.y >= 1.0 - stepSize)
			{
				return 2; // top of pole
			}
			else return 9; // select entire pole
		}
		else if (isCross(id) || id == _LANTERN || id == _LANTERN_OFF)
		{
			return 6; // crosses box
		}
		else
		{
			// determine best-guess facing value for cube & halfblock
			
			vec3::vector_t minv = fracs.min();
			
			if (fracs.x == minv)
			{
				if (fracs.x <= stepSize)
					return 5; // left side of a cube
			}
			else if (fracs.y == minv)
			{
				if (fracs.y <= stepSize)
					return 3; // bottom of a cube
			}
			else
			{
				if (fracs.z <= stepSize)
					return 1; // backside of cube
			}
			vec3::vector_t maxv = fracs.max();
			
			if (fracs.x == maxv)
			{
				if (fracs.x >= 1.0 - stepSize)
					return 4; // right side of a cube
			}
			else if (fracs.y == maxv)
			{
				if (fracs.y >= 1.0 - stepSize)
					return 2; // top of a cube
			}
			if (fracs.z >= 1.0 - stepSize)
				return 0; // front of cube
			
			// since we didn't find any hits yet, it could be a halfblock
			// or a lowblock, which are at 1/2 and 1/8 respectively in height
			if (isHalfblock(id) || isLowblock(id))
				return 2; // half/low-block top
		}
		// no facing detected, return "something"
		return 0;
		
	} // selection box result
	
	void PlayerSelection::render()
	{
		// determine selection
		mtx.playerselection.lock();
		
		// exit if we have no selection
		if (plogic.hasSelection() == false)
		{
			mtx.playerselection.unlock();
			return;
		}
		// build vertices
		int vertices = 0;
		selection_vertex_t* vertexData = nullptr;
		
		int selection = plogic.selection.facing;
		int vx = (int)plogic.selection.pos.x;
		int vy = (int)plogic.selection.pos.y;
		int vz = (int)plogic.selection.pos.z;
		
		int model  = Block::blockModel(plogic.selection.block->getID());
		int facing = plogic.selection.block->getFacing();
		
		mtx.playerselection.unlock();
		
		if (selection < 6)
		{
			// we are to render some cube model, specifically only 1 quad
			
			vertices = 4;
			vertexData = new selection_vertex_t[vertices];
			// regular cube
			if (vertices != blockmodels.selectionCube[model].copyTo(selection, vertexData))
				throw std::string("PlayerSelection::render(): selection == 6 vertex count mismatch");
		}
		else if (selection == 6) // cross selection box
		{
			vertices = 24;
			vertexData = new selection_vertex_t[vertices];
			// cross selection box
			if (vertices != blockmodels.selecionCross.copyTo(0, vertexData))
				throw std::string("PlayerSelection::render(): selection == 6 vertex count mismatch");
		}
		else if (selection == 9) // pole selection box
		{
			vertices = 24;
			vertexData = new selection_vertex_t[vertices];
			// cross selection box
			if (vertices != blockmodels.selectionPole.copyTo(0, vertexData))
				throw std::string("PlayerSelection::render(): selection == 9 vertex count mismatch");
		}
		else if (selection == 10) // ladders selection box
		{
			vertices = 24;
			vertexData = new selection_vertex_t[vertices];
			// ladder selection box
			if (vertices != blockmodels.selectionLadder.copyTo(facing, vertexData))
				throw std::string("PlayerSelection::render(): selection == 10 vertex count mismatch");
		}
		
		
		// nothing to render? quit
		if (vertices == 0) return;
		
		// upload selection rendering data
		vao.begin(sizeof(selection_vertex_t), vertices, vertexData);
		vao.attrib(0, 3, GL_FLOAT, GL_FALSE, offsetof(selection_vertex_t, x));
		vao.attrib(1, 2, GL_FLOAT, GL_FALSE, offsetof(selection_vertex_t, u));
		vao.end();
		
		/// render player selection ///
		
		// selection shader
		Shader& shd = shaderman[Shaderman::SELECTION];
		shd.bind();
		shd.sendMatrix("matmvp", camera.getMVP_Matrix());
		
		// position in space
		shd.sendVec3("vtrans", vec3(vx, vy, vz));
		
		// enable blending
		glEnable(GL_BLEND);
		glDepthMask(GL_FALSE);
		
		textureman.bind(0, Textureman::T_SELECTION);
		
		// render quad(s)
		vao.render(GL_QUADS);
		
		glDisable(GL_BLEND);
		glDepthMask(GL_TRUE);
		
		delete vertexData;
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER_ARB, 0);
	}
	
		//handle mining timing & results
		/*
		if plogic.Action = paction.mineblock Then
			
			//get current atomic time
			Dim As Double currenttime = Timer()
			
			if currenttime > plogic.MineTiming + plogic.mine_speed Then
				plogic.MineTiming = currenttime //reset offset
				
				//make crc of internal position
				Dim As Integer mintest = ddx * blocksz * blocksz + ddz * blocksz + ddy
				
				if plogic.Minimizer <> mintest Then
					
					// make a change in mining stuff
					plogic.MineTimer = getMiningTimes(selectionid->id)
					if plogic.minimizer = -2 then
						// we have mined before
						plogic.MineTimer += 6 // extra wait time
					endif
					// set the rest, and prepare for battle =)
					plogic.MineMax = plogic.MineTimer
					plogic.Minimizer = mintest
				endif
				
				if plogic.MineTimer > 0 Then
					
					// propagate start of mining
					if network.mymine = false then
						#ifdef USE_INET
							networkSend("PMINE 1")
						#endif
						network.mymine = true
					endif
					
					//create mining debris, and propagate over inet
					#ifdef USE_OBJECTS
						dim itm as inventoryitem = TransformParticle(selectionid)
						itm.count = 2 * (plogic.MineMax - plogic.MineTimer) / plogic.MineMax
						if itm.count <> 0 And itm.id <> 0 then
							CreateObjectMining(s, ddx, ddy, ddz, selface, itm)
							networkSendObject(s, ddx, ddy, ddz, selface, @itm)
						endif
					#endif
					
					// create particles using fractionals
					// newParticleB(s, bx, by, bz, PARTICLE_M_GENER, 3)
					
					//play mining sound based on material
					if plogic.MineTimer Mod plogic.MINE_SOUNDMOD = 0 Then
						#ifdef USE_SOUND
							PlaySound(selectionid->id, sound_mine) //mining sound!
						#endif
					endif
					
					plogic.MineTimer -= 1
					
				else
					plogic.Action = paction.remblock
				endif
			endif
		End if
	*/
}