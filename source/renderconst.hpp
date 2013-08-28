#ifndef RENDERCONST_HPP
#define RENDERCONST_HPP

namespace cppcraft
{
	class RenderConst
	{
	public:
		static const int MAX_FACES_PER_SECTOR   = 10404;
		static const int MAX_INDICES_PER_SECTOR =  4096;
		
		enum shaderline_t
		{
			TX_REPEAT = 0,
			TX_SOLID  = 1,
			TX_TRANS,
			TX_2SIDED,
			TX_CROSS,
			TX_WATER,
			
			MAX_UNIQUE_SHADERS
		};
		
		static const short VERTEX_SCALE = 32; // valid: 16, 32, 64  (128 makes the world too tall)
		static const short VERTEX_SHL   = 5;
		
		static const short WATER_LEVEL  = 64;
		static const short WATER_VX_LEVEL = (WATER_LEVEL - 1) * RenderConst::VERTEX_SCALE;
		
		static const int   SKY_LEVEL    = 220;
	};
}

#endif
