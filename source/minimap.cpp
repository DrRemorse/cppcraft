#include "minimap.hpp"

#include "library/config.hpp"
#include "library/log.hpp"
#include "library/bitmap/bitmap.hpp"
#include "library/bitmap/colortools.hpp"
#include "library/opengl/opengl.hpp"
#include "library/opengl/vao.hpp"
#include "library/opengl/texture.hpp"
#include "biome.hpp"
#include "camera.hpp"
#include "flatlands.hpp"
#include "player.hpp"
#include "sectors.hpp"
#include "seamless.hpp"
#include "shaderman.hpp"
#include "spiders.hpp"
#include "worldbuilder.hpp"
#include <cstring>
#include <mutex>

using namespace library;

namespace cppcraft
{
	// the one and only Minimap(TM)
	Minimap minimap;
	VAO minimapVAO;
	std::mutex minimapMutex;
	
	// constants
	const float HEIGHTMAP_FACTOR = 2.5;
	
	Minimap::Minimap()
	{
		this->bitmap  = nullptr;
		this->texture = nullptr;
	}
	
	void Minimap::init()
	{
		logger << Log::INFO << "* Initializing minimap" << Log::ENDL;
		
		// 32-bits, one pixel per sector on (X, Z) axes
		bitmap = new Bitmap(Sectors.getXZ() * 2, Sectors.getXZ() * 2, 32);
		// create texture
		texture = new Texture(GL_TEXTURE_2D);
		texture->create(*bitmap, true, GL_CLAMP_TO_EDGE, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR);
		
		struct minimap_vertex_t
		{
			GLfloat x, y, z;
		};
		
		// vertices
		minimap_vertex_t vertices[4] =
		{
			{ -0.5, -0.5, 0.0 },
			{  0.5, -0.5, 0.0 },
			{  0.5,  0.5, 0.0 },
			{ -0.5,  0.5, 0.0 }
		};
		
		// vertex array object
		minimapVAO.begin(sizeof(minimap_vertex_t), 4, vertices);
		minimapVAO.attrib(0, 3, GL_FLOAT, GL_FALSE, 0);
		minimapVAO.end();
	}
	
	void Minimap::update()
	{
		// minimap subpixel offset
		this->ofsX = (player.X - (Sectors.getXZ() * Sector::BLOCKS_XZ / 2)) / Seamless::OFFSET * 2;
		this->ofsY = (player.Z - (Sectors.getXZ() * Sector::BLOCKS_XZ / 2)) / Seamless::OFFSET * 2;
		
		// update synchronization
		if (this->needs_update)
		{
			this->needs_update = false;
			
			// bind minimap texture
			texture->bind(0);
			// re-upload pixel data (and auto-generate mipmaps)
			texture->uploadBGRA8(*bitmap);
			// done
		}
	}
	
	void Minimap::render(Matrix& mvp)
	{
		Shader& shd = shaderman[Shaderman::MINIMAP];
		
		shd.bind();
		shd.sendMatrix("matprojview", mvp);
		
		// position intra-block offset
		vec2 offset(0.5 + this->ofsX / bitmap->getwidth(), 0.5 + this->ofsY / bitmap->getheight());
		shd.sendVec2("offset", offset);
		
		// bind minimap texture
		texture->bind(0);
		// render minimap
		minimapVAO.render(GL_QUADS);
	}
	
	int fgetSkylevel(Sector& s, int x, int z)
	{
		//return Flatlands::Flatsector::skylevel
		return Flatlands(s.x, s.z)(x, z).skyLevel;
	}
	
	Bitmap::rgba8_t fgetColor(Sector& s, int x, int z, int clid)
	{
		return Flatlands(s.x, s.z)(x, z).fcolor[clid];
	}
	
	Bitmap::rgba8_t mixColor(Bitmap::rgba8_t a, Bitmap::rgba8_t b, float mixlevel)
	{
		if (a == b) return a;
		
		unsigned char* p = (unsigned char*)&a;
		unsigned char* q = (unsigned char*)&b;
		
		Bitmap::rgba8_t ret;
		unsigned char* retp = (unsigned char*)&ret;
		
		for (unsigned int i = 0; i < sizeof(ret); i++)
		{
			retp[i] = (int)(p[i] * (1.0 - mixlevel) + q[i] * mixlevel);
		}	
		return ret;
	}
	
	Bitmap::rgba8_t lowColor(Bitmap::rgba8_t c)
	{
		unsigned char* p = (unsigned char*)&c;
		p[0] /= HEIGHTMAP_FACTOR;
		p[1] /= HEIGHTMAP_FACTOR;
		p[2] /= HEIGHTMAP_FACTOR;
		
		return p[0] + (p[1] << 8) + (p[2] << 16) + (p[3] << 24);
	}
	
	Bitmap::rgba8_t highColor(Bitmap::rgba8_t c)
	{
		unsigned char* p = (unsigned char*)&c;
		// overflow checks
		p[0] = (p[0] * HEIGHTMAP_FACTOR > 255) ? 255 : p[0] * HEIGHTMAP_FACTOR;
		p[1] = (p[1] * HEIGHTMAP_FACTOR > 255) ? 255 : p[1] * HEIGHTMAP_FACTOR;
		p[2] = (p[2] * HEIGHTMAP_FACTOR > 255) ? 255 : p[2] * HEIGHTMAP_FACTOR;
		
		return p[0] + (p[1] << 8) + (p[2] << 16) + (p[3] << 24);
	}
	
	Bitmap::rgba8_t getBlockColor(Sector& s, int x, int y, int z)
	{
		if (y == 0) return RGBA8(48, 48, 48, 255);
		
		Block b = Spiders::getBlock(s, x, y, z);
		Bitmap::rgba8_t c;
		
		if (isStone(b.getID()))
		{
			//c = fgetColor(s, x, z, Biomes::CL_STONE);
			c = RGBA8(64, 64, 64, 255);
		}
		else if (b.getID() == _SNOWGRASS || b.getID() == _SNOWGRASS_S || b.getID() == _LOWSNOW)
		{
			c = RGBA8(230, 230, 230, 255); // snow white
		}
		else if (b.getID() == _GRAVEL1)
		{
			c = RGBA8(148, 100, 64, 255); // brown-grey
		}
		else if (b.getID() == _GRAVEL2)
		{
			c = RGBA8(130, 130, 130, 255); // grey
		}
		else if (b.getID() == _CLAY)
		{
			c = RGBA8(55, 55, 140, 255); // grey-blue
		}
		else if (b.getID() == _CLAYRED)
		{
			c = RGBA8(140, 55, 5, 255); // grey-red
		}
		else if (b.getID() >= SOIL_START && b.getID() <= SOIL_END)
		{
			if (isDirtSoil(b.getID()))
				c = RGBA8(97, 57, 14, 255); // soil
			else
			{
				c = fgetColor(s, x, z, Biomes::CL_GRASS);
				c = mixColor(c, RGBA8(80, 200, 0, 255), 0.25);
			}
		}
		else if (isCross(b.getID()))
		{
			c = fgetColor(s, x, z, Biomes::CL_GRASS);
			c = mixColor(c, RGBA8(80, 200, 0, 255), 0.25);
		}
		else if (b.getID() == _LEAF_NEEDLE)
		{
			c = RGBA8(20, 80, 20, 255);
		}
		else if (isLeaf(b.getID()))
		{
			c = fgetColor(s, x, z, Biomes::CL_TREES);
			c = mixColor(c, RGBA8(80, 200, 0, 255), 0.25);
		}
		else if (isSand(b.getID()))
		{
			c = RGBA8(220, 210, 174, 255); // sandy
		}
		else if (b.getID() >= 200 && b.getID() <= 231)
		{
			// wood + pumpkin
			c = RGBA8(111, 63, 16, 255); // brown-wood
		}
		else if (b.getID() >= _GIANTSHROOMCORE && b.getID() <= _GIANTSHROOMTOPSPECLE)
		{
			c = Biomes::getSpecialColorRGBA(b.getSpecial());
		}
		else if (b.getID() == _LOWICE)
		{
			c = RGBA8(0, 191, 239, 255); // icy blue
		}
		else if (b.getID() == _CACTUS || b.getID() == _CACTUSBLOCK)
		{
			c = RGBA8(0, 96, 0, 255); // cactus green
		}
		else if (b.getID() == _WATER)
		{
			c = RGBA8(42, 106, 177, 255); // deep ocean
		}
		else if (b.getID() == _AIR)
		{
			c = RGBA8(0, 255, 255, 255); // cyan (error color)
		}
		else
		{
			c = RGBA8(255, 0, 255, 255); // magenta (error color)
		}
		
		// basic height coloring
		const int HEIGHT_BIAS = 128;
		
		if (y < HEIGHT_BIAS)
		{
			// downwards
			c = mixColor(c, lowColor(c), 0.0075 * (HEIGHT_BIAS - y));
		}
		else
		{
			// upwards
			c = mixColor(c, highColor(c), 0.0075 * (y - HEIGHT_BIAS));
		}
		
		return c;
	}
	
	// addSector: called from Generator::generate()
	// each time block data at skylevel is updated, this function COULD be called
	// one solution is to wait for 8 block changes, disregard skylevel, and update
	// the algorithm would be sectorblock::version % 8
	void Minimap::addSector(Sector& sector)
	{
		if (bitmap == nullptr) return;
		if (bitmap->isValid() == false) return;
		
		// read certain blocks from sector, and determine pixel value
		// set pixel value in the correct 2x2 position on pixel table
		
		// fetch sky levels
		int skylevel[8];
		skylevel[0] = fgetSkylevel(sector,  3,  3);
		skylevel[1] = fgetSkylevel(sector,  3, 12);
		skylevel[2] = fgetSkylevel(sector, 12,  3);
		skylevel[3] = fgetSkylevel(sector, 12, 12);
		
		skylevel[4] = fgetSkylevel(sector,  4,  4);
		skylevel[5] = fgetSkylevel(sector,  4, 11);
		skylevel[6] = fgetSkylevel(sector, 11,  4);
		skylevel[7] = fgetSkylevel(sector, 11, 11);
		
		Sector& s0 = Sectors(sector.x, 0, sector.z);
		
		// fetch blocks at skylevels
		// determine colors for skylevel blocks
		Bitmap::rgba8_t colors[4];
		
		colors[0] = getBlockColor(s0,  3, skylevel[0],  3);
		colors[0] = mixColor(colors[0],
					getBlockColor(s0,  4, skylevel[4],  4), 0.5);
		
		colors[1] = getBlockColor(s0,  3, skylevel[1], 12);
		colors[1] = mixColor(colors[1],
					getBlockColor(s0,  4, skylevel[5], 11), 0.5);
		
		colors[2] = getBlockColor(s0, 12, skylevel[2],  3);
		colors[2] = mixColor(colors[2],
					getBlockColor(s0, 11, skylevel[6],  4), 0.5);
		
		colors[3] = getBlockColor(s0, 12, skylevel[3], 12);
		colors[3] = mixColor(colors[3],
					getBlockColor(s0, 11, skylevel[7], 11), 0.5);
		
		// set final color @ pixel (px, pz)
		int px = bitmap->getwidth()  / 2 - Sectors.getXZ() + 2 * sector.x;
		int pz = bitmap->getheight() / 2 - Sectors.getXZ() + 2 * sector.z;
		
		Bitmap::rgba8_t* pixels = bitmap->data();
		if (pixels == nullptr) throw std::string("Minimap::addSector(): Bitmap had no buffer");
		
		int scan = bitmap->getwidth();
		
		pixels[  pz      * scan + px ] = colors[0];
		pixels[ (pz + 1) * scan + px ] = colors[1];
		pixels[  pz      * scan + px + 1 ] = colors[2];
		pixels[ (pz + 1) * scan + px + 1 ] = colors[3];
	}
	
	void Minimap::roll(int x, int z)
	{
		if (bitmap == nullptr) return;
		if (bitmap->isValid() == false) return;
		
		int page = bitmap->getwidth(); // size of scanline, aka. pitch
		Bitmap::rgba8_t* pixels = bitmap->data();
		
		if (x > 0)
		{
			for (int py = 0; py < bitmap->getheight(); py++)
			{
				for (int px = 0; px <= bitmap->getwidth()-2 - 2; px += 2)
				{
					int p = py * page + px;
					
					pixels[p + 0] = pixels[p + 2];
					pixels[p + 1] = pixels[p + 3];
				}
				// set to black
				pixels[ py * page + page-2 ] = 0;
				pixels[ py * page + page-1 ] = 0;
			}
		}
		else if (x < 0)
		{
			for (int py = 0; py < bitmap->getheight(); py++)
			{
				for (int px = bitmap->getwidth()-2; px >= 2; px -= 2)
				{
					int p = py * page + px;
					
					pixels[p + 0] = pixels[p - 2];
					pixels[p + 1] = pixels[p - 1];
				}
				// set to black
				pixels[ py * page + 0 ] = 0;
				pixels[ py * page + 1 ] = 0;
			}
		} // x-axis
		
		if (z > 0)
		{
			for (int py = 0; py <= bitmap->getheight()-2 - 2; py += 2)
			{
				int p = py * page;
				// to pixels + offset, from pixels + offset + 2 pages, copy 2 pages
				memcpy(pixels + p, pixels + p + page * 2, page * 2 * sizeof(Bitmap::rgba8_t));
			}
			// clear last 2 scanlines
			memset(pixels + (bitmap->getheight()-2) * page, 0, page * 2 * sizeof(Bitmap::rgba8_t));
		}
		else if (z < 0)
		{
			for (int py = bitmap->getheight()-2; py >= 2; py -= 2)
			{
				int p = py * page;
				// to pixels + offset, from pixels + offset - 2 pages, copy 2 pages
				memcpy(pixels + p, pixels + p - page * 2, page * 2 * sizeof(Bitmap::rgba8_t));
			}
			// clear first 2 scanlines
			memset(pixels, 0, page * 2 * sizeof(Bitmap::rgba8_t));
		}
		
		// mark as updated
		this->needs_update = true;
	}
}
