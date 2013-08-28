#ifndef MINIMAP_HPP
#define MINIMAP_HPP

namespace library
{
	class Bitmap;
	class Texture;
	class Matrix;
}

namespace cppcraft
{
	class Sector;
	
	class Minimap
	{
	public:
		Minimap();
		void init();
		void update();
		void render(library::Matrix& mvp);
		
		void addSector(Sector& s);
		void roll(int x, int z);
		
	private:
		library::Bitmap*  bitmap;
		library::Texture* texture;
		
		bool needs_update;
		
		float ofsX, ofsY;
		unsigned int vao, vbo;
	};
	extern Minimap minimap;
}
#endif
