#ifndef VOXELIZER_HPP
#define VOXELIZER_HPP

#include "../math/vector.hpp"

namespace library
{
	class Bitmap;
	typedef unsigned int xcolor_t;
	
	typedef struct
	{
		float x, y, z;
		char nx, ny, nz, p1;
		xcolor_t c;
		
	} xvertex_t;
	
	class XModel
	{
	public:
		static void initVoxelizer();
		
		XModel();
		XModel(XModel& mod);
		
		xvertex_t* data();
		int        vertices();
		
		void extrude(Bitmap& fromImage, vec3& offset, vec3& scale);
		
	private:
		static const int VOXELIZER_MAX_VERTICES = 32768;
		static xvertex_t* xv_dump;
		
		bool culltest(xcolor_t value);
		short cull2D(Bitmap& img, int x, int y);
		void putv2D(vec3& offset, vec3& scale, int x, int y, int fid, int vid, xcolor_t vcolor);
		
		xvertex_t* vdata;
		int verts;
	};
	
}

#endif
