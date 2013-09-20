#include "vertex_block.hpp"
#include "vertex_selection.hpp"

#include <vector>

namespace cppcraft
{
	class Mesh
	{
	protected:
		void* meshdata;
		int   vertices;
		int   vsize;
		
	public:
		// constructors
		Mesh();
		Mesh(const Mesh&);
		
		inline void* getData() const
		{
			return meshdata;
		}
		inline int getVertices() const
		{
			return vertices;
		}
		inline int getTotalBytes() const
		{
			return vertices * vsize;
		}
	};
	
	class SelectionMesh : public Mesh
	{
	public:
		SelectionMesh(int verts);
		~SelectionMesh();
		
		inline selection_vertex_t& operator[] (int i)
		{
			return ((selection_vertex_t*) meshdata)[i];
		}
		
		// destroy data
		void dispose()  { delete (selection_vertex_t*) meshdata; }
	};
	
	class BlockMesh : public Mesh
	{
	public:
		BlockMesh(int verts);
		~BlockMesh();
		
		inline vertex_t& operator[] (int i)
		{
			return ((vertex_t*) meshdata)[i];
		}
		
		// destroy data
		void dispose()  { delete (vertex_t*) meshdata; }
	};
	
	class MeshContainer
	{
	private:
		std::vector<Mesh> meshes;
		
	public:
		// main functions
		void add(const Mesh& bm);
		int copyTo(int meshid, void* dest) const;
		int copyAll(void* dest) const;
		
		int getCount(int meshid) const
		{
			return meshes[meshid].getVertices();
		}
		int totalCount() const
		{
			int vertices = 0;
			for (std::size_t i = 0; i < meshes.size(); i++)
				vertices += meshes[i].getVertices();
			
			return vertices;
		}
		
		// special function, making the assumption that this container only has BlockMeshes
		vertex_t& get(int meshid, int vertex);
	};
	
	class BlockModels
	{
	public:
		typedef enum
		{	// cube mesh types
			MI_BLOCK,
			MI_LOWBLOCK,
			MI_HALFBLOCK,
			MI_INSET,
			MI_MODEL_COUNT
		} modid_t;
		
		// 6 faces * 4 verts/face = 24 vertices
		// general 6-sided block models
		// each "model" has 6 sides, in this SPECIAL element
		MeshContainer cubes[MI_MODEL_COUNT];
		// sloped leafs [4 sides] with 5 faces x 4 vertices
		MeshContainer slopedLeafs[4];
		// 1. simple cross (2 crossed faces)
		// 2. simple cross with bottom face (floating flowers)
		// 3. bottom face only (simple floating flower)
		MeshContainer crosses;
		// door 2 states x 4 sides x 6 faces x 4 vertices
		MeshContainer doors;
		// horizontal and vertical (x, z) poles (6-sided cubes)
		// 3 poles total: Y, Z, X
		MeshContainer poles;
		// ladder 4 sides x 1 face
		MeshContainer ladders;
		// fence 2 directions x 4 parts x 6 faces x 4 vertices
		MeshContainer fences;
		// stair meshes
		MeshContainer stairs;
		// lantern meshes 6 sides x 4 vertices
		MeshContainer lanterns;
		
		// cubes with various center-offsets, used in direct rendering
		MeshContainer centerCube;
		MeshContainer ctopCube; // cube offset centered top
		MeshContainer cbotCube; // cube offset centered bottom
		
		MeshContainer guiSprite; // single face used to display a sprite
		MeshContainer guiDoors; // simplification of doors, rendering thread
		
		MeshContainer selectionCube[MI_MODEL_COUNT];
		MeshContainer selecionCross; // selection box for crosses, lanterns etc.
		MeshContainer selectionLadder;
		MeshContainer selectionPole;
		MeshContainer selectionStair;
		MeshContainer selectionDoor;
		
		// initialization function
		void init();
		
	private:
		void initCubes();
		void initSlopedLeafs();
		void initLadders();
		void initPoles();
		void initStairs();
		void initFences();
		void initDoors();
		void initLanterns();
		void initCrosses();
		
		void extrude(selection_vertex_t& select, float dist, int face);
	};
	extern BlockModels blockmodels;
}
