/**
 * 
 * 
 * 
**/

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
		void render(Renderer& renderer);
		
	private:
		double lastTime;
		
		// snapshots
		double iplayerX, iplayerY, iplayerZ;
		// used when rendering
		double playerX, playerY, playerZ;
		bool playerMoved, underwater;
		int playerSectorX, playerSectorZ;
		int snapWX, snapWZ;
		
		// camera bobbing
		double motionTimed = 0.0;
		double lastCameraDeviation = 0.0;
		
		// camera bobbing computatron
		double cameraDeviation(double frameCounter, double dtime);
		
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
		friend class GUIRenderer;
	};
}
