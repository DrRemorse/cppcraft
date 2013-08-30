#include "textureman.hpp"

#include "library/log.hpp"
#include "library/config.hpp"
#include "library/bitmap/bitmap.hpp"
#include "library/opengl/window.hpp"
#include "library/opengl/opengl.hpp"
#include "render_fs.hpp"
#include "sun.hpp"
#include "tiles.hpp"

using namespace library;

namespace cppcraft
{
	Textureman textureman;
	
	Textureman::Textureman() {}
	
	void Textureman::init(WindowClass& gamescr)
	{
		// create all textures
		logger << Log::INFO << "* Loading & processing textures" << Log::ENDL;
		
		// working bitmap object
		Bitmap bmp;
		
		/// TERRAIN regular tileset ///
		if (bmp.load(config.get("textures.diffuse", "bitmap/default/diffuse.bmp")))
		{
			bmp.parse2D(tiles.txW, tiles.txH);
			
			// set engine (smaller) tileset tiles in (x, y) directions
			tiles.tilesX = bmp.getTilesX();
			tiles.tilesY = bmp.getTilesY();
			
			textures[T_DIFFUSE] = Texture(GL_TEXTURE_2D_ARRAY);
			textures[T_DIFFUSE].create(bmp, true, GL_CLAMP_TO_EDGE, GL_NEAREST, GL_LINEAR_MIPMAP_LINEAR);
		}
		else throw std::string("Materials(1) missing source file: Diffuse");
		if (ogl.checkError()) throw std::string("Materials(1) texture2d array error");
		
		if (bmp.load(config.get("textures.tonemap", "bitmap/default/tonemap.bmp")))
		{
			bmp.parse2D(tiles.txW, tiles.txH);
			
			textures[T_TONEMAP] = Texture(GL_TEXTURE_2D_ARRAY);
			textures[T_TONEMAP].create(bmp, true, GL_CLAMP_TO_EDGE, GL_NEAREST, GL_LINEAR_MIPMAP_LINEAR);
		}
		else throw std::string("Materials(2) missing source file: Tonemap");
		if (ogl.checkError()) throw std::string("Materials(2) texture2d array error");
		
		/// TERRAIN bigger tileset ///
		if (bmp.load(config.get("textures.bigdiff", "bitmap/default/bigdiff.bmp")))
		{
			bmp.parse2D(tiles.bigW, tiles.bigH);
			
			// set engine (bigger) tileset tiles in (x, y) directions
			tiles.bigTilesX = bmp.getTilesX();
			tiles.bigTilesY = bmp.getTilesY();
			
			textures[T_BIG_DIFF] = Texture(GL_TEXTURE_2D_ARRAY);
			textures[T_BIG_DIFF].create(bmp, true, GL_REPEAT, GL_NEAREST, GL_LINEAR_MIPMAP_LINEAR);
		}
		else throw std::string("Materials(3) missing source file: Bigdiff");
		if (ogl.checkError()) throw std::string("Materials(3) texture2d array error");
		
		if (bmp.load(config.get("textures.bigtone", "bitmap/default/bigtone.bmp")))
		{
			bmp.parse2D(tiles.bigW, tiles.bigH);
			
			textures[T_BIG_TONE] = Texture(GL_TEXTURE_2D_ARRAY);
			textures[T_BIG_TONE].create(bmp, true, GL_REPEAT, GL_NEAREST, GL_LINEAR_MIPMAP_LINEAR);
		}
		else throw std::string("Materials(4) missing source file: Bigtone");
		if (ogl.checkError()) throw std::string("Materials(4) texture2d array error");
		
		/// PLAYER MODELS tileset ///
		if (bmp.load(config.get("textures.playerskins", "bitmap/default/playerskins.bmp")))
		{
			bmp.parse2D(tiles.skinSize, tiles.skinSize);
			
			textures[T_PLAYERMODELS] = Texture(GL_TEXTURE_2D_ARRAY);
			textures[T_PLAYERMODELS].create(bmp, true, GL_CLAMP_TO_EDGE, GL_NEAREST, GL_LINEAR_MIPMAP_LINEAR);
		}
		else throw std::string("Missing source file: Player skins");
		if (ogl.checkError()) throw std::string("Player skins texture2d array error");
		
		// selection texture
		if (bmp.load(config.get("textures.selection", "bitmap/default/selection.bmp")))
		{
			textures[T_SELECTION] = Texture(GL_TEXTURE_2D);
			textures[T_SELECTION].create(bmp, true, GL_CLAMP_TO_EDGE, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR);
		}
		else throw std::string("Missing source file: Selection texture");
		if (ogl.checkError()) throw std::string("Selection texture2d error");
		
		
		/// Sky renderer ///
		
		// sun texture
		if (bmp.load(config.get("textures.sun", "bitmap/default/sun.bmp")))
		{
			textures[T_SUN] = Texture(GL_TEXTURE_2D);
			textures[T_SUN].create(bmp, true, GL_CLAMP_TO_EDGE, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR);
		}
		// moon texture
		if (bmp.load(config.get("textures.moon", "bitmap/default/moon.bmp")))
		{
			textures[T_MOON] = Texture(GL_TEXTURE_2D, GL_COMPRESSED_RGBA);
			textures[T_MOON].create(bmp, true, GL_CLAMP_TO_EDGE, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR);
		}
		else throw std::string("Materials(5) missing source file: Moon texture");
		if (ogl.checkError()) throw std::string("Materials(5) texture2d error");
		
		// stars texture
		if (bmp.load(config.get("textures.stars", "bitmap/default/stars.bmp")))
		{
			textures[T_STARS] = Texture(GL_TEXTURE_2D, GL_COMPRESSED_RGBA);
			textures[T_STARS].create(bmp, true, GL_REPEAT, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR);
		}
		else throw std::string("Materials(6) missing source file: Starmap");
		if (ogl.checkError()) throw std::string("Materials(6) texture2d error");
		
		// sky cubemap
		if (bmp.load(config.get("textures.skybox", "bitmap/default/skybox.bmp")))
		{
			textures[T_SKYBOX] = Texture(GL_TEXTURE_CUBE_MAP, GL_COMPRESSED_RGBA);
			textures[T_SKYBOX].create(bmp, false, GL_CLAMP_TO_EDGE, GL_LINEAR, GL_LINEAR);
		}
		else throw std::string("Missing source file: Skybox cubemap");
		if (ogl.checkError()) throw std::string("Skybox cubemap error");
		
		// clouds texture
		if (bmp.load(config.get("textures.clouds", "bitmap/default/clouds.bmp")))
		{
			textures[T_CLOUDS] = Texture(GL_TEXTURE_2D);
			textures[T_CLOUDS].create(bmp, true, GL_REPEAT, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR);
		}
		else throw std::string("Missing source file: Clouds texture");
		if (ogl.checkError()) throw std::string("Clouds texture error");
		
		/// FS Renderer ///
		
		// dirty lens texture
		if (bmp.load(config.get("textures.lens", "bitmap/default/lensdirt_lowc.bmp")))
		{
			textures[T_LENSDIRT] = Texture(GL_TEXTURE_2D, GL_COMPRESSED_RGBA);
			textures[T_LENSDIRT].create(bmp, false, GL_CLAMP_TO_EDGE, GL_LINEAR, GL_LINEAR);
		}
		else throw std::string("Missing source file: Lensdirt texture");
		if (ogl.checkError()) throw std::string("Lensdirt texture error");
		
		// lens buffers
		for (int i = 0; i < 3; i++)
		{
			textures[(int)T_LENSFLARE + i] = Texture(GL_TEXTURE_2D, GL_RGBA16F_ARB);
			textures[(int)T_LENSFLARE + i].create(false, 0, screenspace.flareTxW, screenspace.flareTxH);
		}
		
		// blur buffers
		for (int i = 0; i < 2; i++)
		{
			textures[(int)T_BLURBUFFER1 + i] = Texture(GL_TEXTURE_2D, GL_RGBA16F_ARB);
			textures[(int)T_BLURBUFFER1 + i].create(false, 0, screenspace.blurTxW, screenspace.blurTxH);
		}
		
		/// GUI Renderer ///
		
		// compass
		if (bmp.load(config.get("textures.compass", "bitmap/default/compass.data")))
		{
			textures[T_COMPASS] = Texture(GL_TEXTURE_2D);
			textures[T_COMPASS].create(bmp, true, GL_CLAMP_TO_EDGE, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR);
		}
		else throw std::string("Missing source file: Compass texture");
		if (ogl.checkError()) throw std::string("Compass texture error");
		
		// quickbar
		if (bmp.load(config.get("textures.quickbar", "bitmap/default/quickbar.data")))
		{
			textures[T_QUICKBAR] = Texture(GL_TEXTURE_2D);
			textures[T_QUICKBAR].create(bmp, true, GL_CLAMP_TO_EDGE, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR);
		}
		else throw std::string("Missing source file: Quickbar texture");
		if (ogl.checkError()) throw std::string("Quickbar texture error");
		
		// generated textures
		generateTextures();
		
		/// Fullscreen Buffers ///
		
		// fullscreen colorbuffer
		textures[T_RENDERBUFFER] = Texture(GL_TEXTURE_2D);
		textures[T_RENDERBUFFER].create(false, 0, gamescr.SW, gamescr.SH);
		// fullscreen depthbuffer
		textures[T_DEPTHBUFFER] = Texture(GL_TEXTURE_2D);
		textures[T_DEPTHBUFFER].createDepth(true, gamescr.SW, gamescr.SH);
		// fullscreen skybuffer
		textures[T_SKYBUFFER] = Texture(GL_TEXTURE_2D);
		textures[T_SKYBUFFER].create(false, 0, gamescr.SW, gamescr.SH);
		// fullscreen underwater texture
		textures[T_UNDERWATERMAP] = Texture(GL_TEXTURE_2D);
		textures[T_UNDERWATERMAP].create(false, 0, gamescr.SW, gamescr.SH);
	}
	
	void Textureman::bind(int unit, named_textures_t tx)
	{
		textures[tx].bind(unit);
	}
	
	void Textureman::unbind(int unit)
	{
		textures[0].unbind(0);
	}
	
	GLuint Textureman::get(named_textures_t tx)
	{
		return textures[tx].getHandle();
	}
	
	void Textureman::copyScreen(WindowClass& gamescr, named_textures_t tx)
	{
		textures[tx].copyScreen(gamescr.SW, gamescr.SH);
	}
	
}