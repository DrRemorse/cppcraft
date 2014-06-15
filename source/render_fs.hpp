#ifndef SCREENSPACE_HPP
#define SCREENSPACE_HPP

namespace library
{
	class WindowClass;
	class Texture;
	class vec2;
	class vec3;
	class mat4;
}

namespace cppcraft
{
	class FSRenderer
	{
	public:
		void init(library::WindowClass& gamescr);
		void initFlare();
		void blur(library::Texture& texture);
		void fog(double time);
		void terrain();
		void renderSuperSampling(library::Texture& supersampled, library::Texture& texture);
		
		void render(library::WindowClass& gamescr, double frameCounter, bool underwater);
		
		library::vec2 getSunVector(const library::mat4& matsun);
		
	private:
		void renderLensflare(library::WindowClass& gamescr);
		
		// flare texture
		int flareTxW, flareTxH;
		unsigned int flareFBO;
		// blur texture
		int blurTxW, blurTxH;
		unsigned int blurFBO;
		
		int underwater;
		
		friend class Textureman;
	};
	extern FSRenderer screenspace;
}

#endif
