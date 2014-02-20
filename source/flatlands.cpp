#include "flatlands.hpp"

#include <library/log.hpp>
#include <library/math/toolbox.hpp>
#include <library/opengl/opengl.hpp>

using namespace library;

namespace cppcraft
{
	FlatlandsContainer flatlands;
	
	// returns a const flatland_t struct
	FlatlandSector::flatland_t& FlatlandSector::operator() (int x, int z)
	{
		return this->fdata[x][z];
	}
	
	FlatlandSector::flatland_t& FlatlandsContainer::getData(int x, int z) const
	{
		// find flatland sector
		int fx = x / Sector::BLOCKS_XZ;
		int fz = z / Sector::BLOCKS_XZ;
		// clamp to within local grid
		fx = toolbox::clamp(0, Sectors.getXZ()-1, fx);
		fz = toolbox::clamp(0, Sectors.getXZ()-1, fz);
		// find internal position
		int bx = x & (Sector::BLOCKS_XZ-1);
		int bz = z & (Sector::BLOCKS_XZ-1);
		// return data structure
		return this[0](fx, fz)(bx, bz);
	}
	
	void FlatlandsContainer::init()
	{
		logger << Log::INFO << "* Initializing flatlands" << Log::ENDL;
		
		// allocate sector pointers
		fsectors = new FlatlandSector*[Sectors.getXZ() * Sectors.getXZ()];
		// iterate sectors
		for (int x = 0; x < Sectors.getXZ(); x++)
		{
			for (int z = 0; z < Sectors.getXZ(); z++)
			{
				// get reference to flatland pointer
				FlatlandSector*& f = manipulate(x, z);
				// set reference (FlatlandSector*) to new FlatlandSector(x, z)
				f = new FlatlandSector;
				f->rebuild  = false;
			} // z
		} // x
		
		biomeArray = nullptr;
		upload = false;
	}
	FlatlandsContainer::~FlatlandsContainer()
	{
		delete fsectors;
		delete[] biomeArray;
	}
	
	void FlatlandsContainer::initTextures()
	{
		int xz = Sectors.getXZ() * Sector::BLOCKS_XZ;
		
		// create biome 3d texture
		#define GL_TEXTURE_3D 0x806F
		biomeTexture = Texture(GL_TEXTURE_3D);
		biomeTexture.create3d(0, xz, xz, FlatlandSector::FLATCOLORS);
		
		if (OpenGL::checkError())
		{
			logger << Log::ERR << "FlatlandsContainer::initTextures(): OpenGL error. Line: " << __LINE__ << Log::ENDL;
			throw std::string("FlatlandsContainer::initTextures(): OpenGL state error");
		}
	}
	
	// build 3d texture containing all colors
	void FlatlandsContainer::buildTexture(int fx, int fz)
	{
		const int xz = Sectors.getXZ() * Sector::BLOCKS_XZ;
		const int VOL = xz * xz * FlatlandSector::FLATCOLORS;
		
		if (biomeArray == nullptr) biomeArray = new unsigned int[VOL];
		unsigned int* p = biomeArray + (fx * Sectors.getXZ() + fz) * Sector::BLOCKS_XZ * FlatlandSector::FLATCOLORS;
		
		FlatlandSector& fsector = this[0](fx, fz);
		
		for (int x = 0; x < Sector::BLOCKS_XZ; x++)
		for (int z = 0; z < Sector::BLOCKS_XZ; z++)
		for (int b = 0; b < FlatlandSector::FLATCOLORS; b++)
		{
			if (x == Sector::BLOCKS_XZ || z == Sector::BLOCKS_XZ)
			{
				// temporary fix, until older and wiser
				*p = fsector(x-1, z-1).fcolor[b];
			}
			else *p = fsector(x, z).fcolor[b];
			p++;
		}
		// no longer need to rebuild
		this[0](fx, fz).rebuild = false;
		// but we need to upload the texture (at some point)
		this->upload = true;
	}
	
	void FlatlandsContainer::bindTexture(int unit)
	{
		biomeTexture.bind(unit);
	}
	void FlatlandsContainer::uploadTexture()
	{
		this->upload = false;
		
		glActiveTexture(GL_TEXTURE0 + 2);
		int xz = Sectors.getXZ() * Sector::BLOCKS_XZ;
		biomeTexture.upload3D(xz, xz, FlatlandSector::FLATCOLORS, biomeArray);
		
		#ifdef DEBUG
		if (OpenGL::checkError())
		{
			logger << Log::ERR << "FlatlandsContainer::uploadTexture(): OpenGL error. Line: " << __LINE__ << Log::ENDL;
			throw std::string("FlatlandsContainer::uploadTexture(): OpenGL state error");
		}
		#endif
	}
	
}
