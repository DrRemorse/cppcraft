#ifndef SCREENSPACE_HPP
#define SCREENSPACE_HPP

namespace library
{
	class WindowClass;
}

namespace cppcraft
{
	class FSRenderer
	{
	private:
		unsigned int screenVAO;
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
		void render(library::WindowClass& gamescr);
		
		friend class Textureman;
	};
	extern FSRenderer screenspace;
}

#endif
