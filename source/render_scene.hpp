#include "camera.hpp"

namespace library
{
	class Camera;
	class vec3;
}

namespace cppcraft
{
	class Camera;
	class DrawQueue;
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
		bool playerMoved, underwater;
		int playerSectorX, playerSectorZ;
		int snapWX, snapWZ;
		
		// camera bobbing
		double motionTimed = 0.0;
		double lastCameraDeviation = 0.0;
		
		// camera bobbing computatron
		bool cameraDeviation(double frameCounter, double dtime);
		
		cppcraft::Camera reflectionCamera;
		
		// juggernauts
		void initTerrain();
		void recalculateFrustum();
		void recalculateFrustum(Camera& camera, DrawQueue& drawq, const library::vec3& look);
		void compressRenderingQueue();
		// main scene rendering function
		void renderScene(Renderer& renderer, library::Camera& camera);
		void renderReflectedScene(Renderer& renderer, library::Camera& camera);
		void renderSceneWater(Renderer& renderer);
		
		friend class SkyRenderer;
	};
}
