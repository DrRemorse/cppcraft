#include "library/math/matrix.hpp"
#include "library/math/vector.hpp"

namespace cppcraft
{
	class Renderer;
	class SkyRenderer;
	class WorldManager;
	
	class SceneRenderer
	{
	public:
		void init(Renderer& renderer);
		void render(Renderer& renderer, WorldManager& worldman);
		
	private:
		// snapshots
		double playerX, playerY, playerZ;
		bool playerMoved;
		int playerSectorX, playerSectorZ;
		int snapWX, snapWZ;
		
		// camera bobbing
		double motionTimed = 0.0;
		double lastCameraDeviation = 0.0;
		
		// camera bobbing computatron
		bool cameraDeviation(double frameCounter, double dtime);
		
		// juggernauts
		void initTerrain();
		void recalculateFrustum();
		void compressRenderingQueue();
		// main scene rendering function
		void renderScene(Renderer& renderer);
		
		friend class SkyRenderer;
	};
}