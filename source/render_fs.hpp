#ifndef SCREENSPACE_HPP
#define SCREENSPACE_HPP

namespace library
{
	class WindowClass;
	class vec2;
	class Matrix;
}

namespace cppcraft
{
	class FSRenderer
	{
	private:
		// flare texture
		int flareTxW, flareTxH;
		unsigned int flareFBO;
		// blur texture
		int blurTxW, blurTxH;
		unsigned int blurFBO;
		
		void renderLensflare(library::WindowClass& gamescr);
		void renderBlur();
		
	public:
		void init(library::WindowClass& gamescr);
		void initFlare();
		void render(library::WindowClass& gamescr);
		void blur(library::WindowClass& gamescr);
		void fog(library::WindowClass& gamescr);
		void terrain(library::WindowClass& gamescr);
		
		library::vec2 getSunVector(const library::Matrix& matsun);
		
		friend class Textureman;
	};
	extern FSRenderer screenspace;
}

#endif
