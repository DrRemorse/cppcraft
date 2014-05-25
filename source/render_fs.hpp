#ifndef SCREENSPACE_HPP
#define SCREENSPACE_HPP

namespace library
{
	class WindowClass;
	class vec2;
	class mat4;
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
		void renderBlur(int w, int h);
		
	public:
		void init(library::WindowClass& gamescr);
		void initFlare();
		void blur(library::WindowClass& gamescr);
		void fog(double time);
		void terrain();
		
		void render(library::WindowClass& gamescr, double frameCounter);
		
		library::vec2 getSunVector(const library::mat4& matsun);
		
		friend class Textureman;
	};
	extern FSRenderer screenspace;
}

#endif
