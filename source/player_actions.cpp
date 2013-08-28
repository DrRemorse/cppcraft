#include "player_logic.hpp"

#include "library/log.hpp"
#include "library/math/vector.hpp"
#include "frustum.hpp"
#include "items.hpp"
#include "player.hpp"
#include "sectors.hpp"
#include "spiders.hpp"
#include "threading.hpp"
#include <cmath>

using namespace library;

namespace cppcraft
{
	void PlayerLogic::activate(InventoryItem& item)
	{
		// with no selection in the world, we can't activate anything
		if (plogic.hasSelection() == false) return;
		playerselect_t& selection = plogic.selection;
		
		// local grid coordinates
		int ddx = int(selection.pos.x);
		int ddy = int(selection.pos.y);
		int ddz = int(selection.pos.z);
		
		Block& selectedBlock = Spiders::getBlock(ddx, ddy, ddz);
		block_t id = selectedBlock.getID();
		
		if (isDoor(id))
		{
			block_t newState = (selectedBlock.getSpecial() >> 1) xor 1;
			
			// modulate bitfield
			selectedBlock.setSpecial( (selectedBlock.getSpecial() & 1) + (newState << 1) );
			
			// collect into 6 bits
			block_t bitfield = selectedBlock.getFacing() + (selectedBlock.getSpecial() << 2);
			
			Spiders::updateBlock(ddx, ddy, ddz, bitfield, true);
			//networkSetBlock(ddx, ddy, ddz, b->id, bitfield);
			
			const block_t door_ul_bit = 1 << 2;
			
			if (selectedBlock.getSpecial() & 1) // lower
			{
				// update upper also
				Spiders::updateBlock(ddx, ddy+1, ddz, bitfield - door_ul_bit, true);
				//networkSetBlock(ddx, ddy+1, ddz, b->id, bitfield - door_ul_bit);
			}
			else
			{
				// update lower also
				Spiders::updateBlock(ddx, ddy-1, ddz, bitfield + door_ul_bit, true);
				//networkSetBlock(ddx, ddy-1, ddz, b->id, bitfield + door_ul_bit);
			}
			
			#ifdef USE_SOUND
				if newstate = 0 then
					inetSoundEx(S_FX_DOOR_OPEN, 1.0, true)
				else
					inetSoundEx(S_FX_DOOR_CLOSE, 1.0, true)
				endif
			#endif
		}
		
	}
	
	void PlayerLogic::itemAction(InventoryItem& item)
	{
		if (item.id == IT_PORTALGUN || item.id == IT_CREATORGUN)
		{
			// items you can shoot with
			int FIXME_player_shoot;
			//PlayerShoot(actionitem);
		}
		else //if (item.id >= 1 && item.id <= 49)
		{
			// items you can't build with, but you can swing with
			swingTool(item);
		}
	}
	
	void PlayerLogic::build(InventoryItem& item)
	{
		// add a block to the opposite of the selected face
		playerselect_t& selection = plogic.selection;
		
		// the cube facing that would face player
		block_t facing = player.getBlockFacing();
		
		bool placement_test = (item.count != 0 && item.type == 0);
		
		if (item.id == _LADDER)
		{
			placement_test &= (selection.facing != 2 && selection.facing != 3);
			
			// override facing
			facing = Block::cubeToFacing(facing);
		}
		else if (isStair(item.id))
		{
			// nothing for the moment =)
			
		}
		else if (isHalfblock(item.id))
		{
			// top face
			if (selection.facing == 2)
			{
				// do nothing
			}
			// bottom face
			else if (selection.facing == 3)
			{
				// set halfblock to reside on top under the block
				facing |= 4;
			}
			else
			{
				// decide with y fractional part
				float fr = selection.pos.y - int(selection.pos.y);
				if (fr > 0.5)
				{
					// top level
					facing |= 4;
				}
			}
		}
		else if (isLowblock(item.id))
		{
			/// low blocks, don't care ///
			//ddy = int( frac(plogic.selected(1)) * 8 )
			//facing or= ddy shl 2
		}
		
		// make sure we can place <here>
		int ddx = selection.pos.x;
		int ddy = selection.pos.y;
		int ddz = selection.pos.z;
		Block& selectedBlock = Spiders::getBlock(ddx, ddy, ddz);
		
		placement_test &= Block::blockPlacementAllowed(selectedBlock.getID());
		
		if (placement_test)
		{
			// move from target block to the one opposite of selected face
			switch (selection.facing)
			{
				case 0: ddz += 1; // +z
				break;
				case 1: ddz -= 1; // -z
				break;
				case 2: ddy += 1; // +y
				break;
				case 3: ddy -= 1; // -y
				break;
				case 4: ddx += 1; // +x
				break;
				case 5: ddx -= 1; // -x
				break;
			}
			
			Block& newBlock = Spiders::getBlock(ddx, ddy, ddz);
			if (Block::blockPlacement(newBlock.getID()))
			{
					logger << "Placing block... plstest" << Log::ENDL;
				// finally, we can place the block
				
				if (isDoor(item.id))
				{
					// place a door
					Block& top = Spiders::getBlock(ddx, ddy+1, ddz);
					if (Block::blockPlacement(top.getID()))
					{
						// upper
						Spiders::addblock(ddx, ddy + 1, ddz, item.id, facing + (0 << 2), true);
						// lower = special bit 0 set to 1
						Spiders::addblock(ddx, ddy + 0, ddz, item.id, facing + (1 << 2), true);
						
						item.count--; // decrease count (directly)!
						
						#ifdef USE_SOUND
							PlaySound(curitem->id, sound_place)
						#endif
						
						//networkSetBlock(s, ddx, ddy+1, ddz, curitem->id, facing)		   // upper
						//networkSetBlock(s, ddx, ddy, ddz, curitem->id, facing + (1 shl 2)) // lower
						
					} // upper door test
				}
				else // place regular blocks
				{
					// bitfield value
					block_t bfield = facing + (item.special << 2);
					// add block to world
					bool placed = Spiders::addblock(ddx, ddy, ddz, item.id, bfield, true);
					logger << "Placed block: " << item.id << Log::ENDL;
					
					if (placed)
					{
						item.count--; //decrease count (directly)!
						
						#ifdef USE_SOUND
							PlaySound(curitem->id, sound_place)
						#endif
						
						//networkAdd(s, ddx, ddy, ddz, curitem->id, bfield)
						
						// update frustum
						frustum.recalc = true;
					}
					
				}
				
			}
			
		} // placement tests
		
	} // build
	
	void PlayerLogic::swingTool(InventoryItem& item)
	{
		plogic.action = playeraction_t::PA_Swingtool;
		plogic.actionTimer = 8;
		
	}
	
	void PlayerLogic::handleActions()
	{
		block_t facing  = player.getBlockFacing();
		vec3 lookVector = player.getLookVector();
		
		/////////////////////////////////////
		/// set up raycaster / integrator ///
		/////////////////////////////////////
		#ifdef DBG_SUPERFOCAL
			const double action_range = 16;
		#else
			const double action_range = 5;
		#endif
		const double action_step  = 0.005;
		const double action_bigstep = 0.2;
		
		vec3 rayBigStep = lookVector * action_bigstep;
		vec3 rayStep = lookVector * action_step;
		
		vec3 playerPos = vec3(player.X, player.Y, player.Z);
		// account for crouching
		if (plogic.movestate == PMS_Crouch) playerPos.y -= 0.25;
		
		/////////////////////////////////////
		/// item currently held by player ///
		/////////////////////////////////////
		int FIXME_held_item;
		InventoryItem item(_STONE, 64); // = items.Inventory(menu.invcycle, menu.quickbar)
		
		if (plogic.action == playeraction_t::PA_Addblock)
		{
			// we need to check if we can perform an action instead of "placing a block"
			// so to do that we see if the current item is an actionable item
			// we also have the ability to activate blocks in the world, if so that block hasActivation()
			if (false) //item.isActionItem())
			{
				itemAction(item);
			}
			else if (plogic.hasSelection())
			{
				if (selection.block->hasActivation())
				{
					activate(item);
					return;
				}
				else // otherwise just 'build'
				{	// aka place block
					build(item);
				}
			}
			
			// handle hand animation & cooldown timing
			int FIXME_cooldown_handling;
			/*
			if plogic.Action <> paction.nothing then
				plogic.Action = paction.cooldown
				plogic.MineTimer = plogic.MINE_COOLDOWN
				plogic.cooldowntime = fru.uptimed
				return
			endif
			*/
			plogic.action = playeraction_t::PA_Nothing;
		}
		else if (plogic.action == playeraction_t::PA_Remblock)
		{
			// remove selected block
			if (plogic.hasSelection())
			{
				int ddx = int(selection.pos.x);
				int ddy = int(selection.pos.y);
				int ddz = int(selection.pos.z);
				
				if (isDoor(selection.block->getID()))
				{
					// remove the other doorpiece
					
					if (selection.block->getSpecial() & 1)
					{
						// we're at lower, remove upper
						Spiders::removeBlock(ddx, ddy+1, ddz, true);
						//networkRemove(plogic.SelectedSector, ddx, ddy+1, ddz);
					}
					else
					{
						// upper, remove lower
						Spiders::removeBlock(ddx, ddy-1, ddz, true);
						//networkRemove(plogic.SelectedSector, ddx, ddy-1, ddz);
					}
				}
				
				// now, remove the block we wanted to remove to begin with
				Block removed = Spiders::removeBlock(ddx, ddy, ddz, true);
				
				if (removed.getID() != _AIR)
				{
					// play sound
					int FIXME_play_removed_sound;
					
					// propagate query removal of block
					//networkRemove(plogic.SelectedSector, ddx, ddy, ddz)
					// --------------------------------
					
					int FIXME_create_some_smoke_n_shits;
					
				} // block was removed
				
			} // hasSelection()
			
			/*
			plogic.Action = paction.mineblock //dont more blocks!
			plogic.MineTimer = 2000
			plogic.MineMax   = 2000
			plogic.Minimizer = -2
			*/
		}
		else if (plogic.action == playeraction_t::PA_Swingtool)
		{
			plogic.actionTimer -= 1;
			if (plogic.actionTimer == 0)
			{
				plogic.cancelDig();
			}
		}
		else // selection
		{
			// here we are going to raycast our way to the closest block we are pointing at
			// if no block is found, we need to set selection.block to null, so that we know no block is selected
			
			// start at player position
			vec3 ray = playerPos;
			bool foundSelection = false;
			
			// integrate forward
			for (float forward = 0.0; forward <= action_range; forward += action_bigstep)
			{
				// increase ray by one big step
				ray += rayBigStep;
				Block& found = Spiders::getBlock(ray.x, ray.y, ray.z);
				
				if (Block::fluidToAir(found.getID()) != _AIR)
				{
					// create fractionals from ray
					vec3 fracs = ray.frac();
					
					if (found.selectionHitbox3D(found, fracs.x, fracs.y, fracs.z))
					{
						// for loop to make sure we don't trace ourselves back behind the player
						for (float backward = forward; backward >= 0.0; backward -= action_step)
						{
							// glean backward slowly until we exit completely, but we still retain our closest valid position
							ray -= rayStep;
							Block& bfound = Spiders::getBlock(ray.x, ray.y, ray.z);
							
							fracs = ray.frac();
							
							// glean until we hit air, then break
							if (Block::selectionHitbox3D(bfound, fracs.x, fracs.y, fracs.z) == false) break;
							
						}
						// now that we are out, increase ray by one step ahead to get back inside
						ray += rayStep;
						// create new fractionals
						fracs = ray.frac();
						
						int ddx = ray.x;
						int ddy = ray.y;
						int ddz = ray.z;
						// find position in local grid
						Sector* sector = Spiders::spiderwrap(ddx, ddy, ddz);
						
						// outside of local grid? nothing to select
						if (sector == nullptr) break;
						
						Block& selectionBlock = Spiders::getBlock(ray.x, ray.y, ray.z);
						
						// set selection results
						mtx.playerselection.lock();
						{
							selection.block = &selectionBlock;
							selection.sector = sector;
							selection.pos = ray;
							selection.facing = determineSelectionFacing(selectionBlock, ray, fracs, action_step);
							foundSelection = true;
						}
						mtx.playerselection.unlock();
						
						// break out of loop
						break;
						
					} // selection hitboxing
					
				} // air & fluid test
				
			} // ray integrator
			
			if (foundSelection == false)
			{
				// we have no selection
				mtx.playerselection.lock();
				{
					selection.block = nullptr;
					selection.sector = nullptr;
				}
				mtx.playerselection.unlock();
			}
			
			if (plogic.hasSelection() == false)
			if (plogic.action == playeraction_t::PA_Mineblock)
			{
				// reset any mining process, when we lost selection
				plogic.mineTimer = 2000;
				plogic.mineMax   = 2000;
				plogic.minimizer = -1;
			}
			
			if (plogic.action == playeraction_t::PA_Cooldown)
			{
				//Dim As Double currenttime = Timer()
				/*
				if currenttime > plogic.MineTiming + plogic.MINE_SPEED Then
					plogic.MineTiming = currenttime
					
					if plogic.MineTimer Then
						plogic.MineTimer -= 1
					else
						plogic.Action = paction.nothing
						plogic.cancelDig() // cancel after action <> cooldown
						plogic.cooldowntime = fru.uptimed
					endif
				endif
				*/
			}
			
		} // selection hitboxing
	}
	
}
