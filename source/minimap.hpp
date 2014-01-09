#ifndef MINIMAP_HPP
#define MINIMAP_HPP

namespace library
{
	class Bitmap;
	class Texture;
	class mat4;
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
		void render(library::mat4& mvp);
		
		void addSector(Sector& s);
		void roll(int x, int z);
		
		inline void setUpdated()
		{
			// mark as updated
			this->needs_update = true;
		}
		
	private:
		library::Bitmap*  bitmap;
		library::Texture* texture;
		
		bool needs_update;
		float ofsX, ofsY;
		
	};
	extern Minimap minimap;
}
#endif
