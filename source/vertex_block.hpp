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
		
		GLshort u; // 12
		GLshort v;
		GLshort w;
		GLshort p;
		
		GLuint biome; // 20
		
		unsigned long long c; // 24
		
	};  // 32
	#pragma pack(pop)
	
	#pragma pack(push, 4)
	struct vertex_fluid_t
	{
		GLshort x; // 0
		GLshort y;
		GLshort z;
		unsigned short face;
		
		GLbyte nx; // 8
		GLbyte ny;
		GLbyte nz;
		GLbyte p1;
		
		unsigned long long c; // 12
		
	};  // 20
	#pragma pack(pop)
	
	typedef GLushort indice_t;
}

#endif
