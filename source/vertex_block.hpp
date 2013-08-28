#ifndef VERTEX_BLOCK_HPP
#define VERTEX_BLOCK_HPP

namespace cppcraft
{
	typedef float GLfloat;
	typedef signed char GLbyte;
	typedef short GLshort;
	typedef unsigned short GLushort;
	typedef int GLint;
	typedef unsigned int GLuint;
	
	#pragma pack(push, 4)
	struct vertex_t
	{
		GLshort x; // 0
		GLshort y;
		GLshort z;
		unsigned short face;
		
		GLbyte nx; // 8
		GLbyte ny;
		GLbyte nz;
		GLbyte p1;
		
		GLbyte tx;
		GLbyte ty;
		GLbyte tz;
		GLbyte p2;
		
		GLshort u; // 16
		GLshort v;
		GLshort w;
		GLshort p;
		
		unsigned long long c; // 24
		
		GLuint biome; // 32
		
	};  // 36
	#pragma pack(pop)
	
	typedef GLushort indice_t;
}

#endif
