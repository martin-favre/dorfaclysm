#include "MainGame.h"
#include "Engine.h"
#include "Component.h"
#include "GridMap.h"
#include "Vector2DInt.h"

void foo()
{
	{
		//GameObject* gObj = Engine::addGameObject<GameObject>();
		
	} 	
}
void MainGame::run()
{
	GridMap map(Vector2DInt(100, 100));
	Engine::initialize(); 
	Engine::registerScene("scene1", foo);
	Engine::loadScene("scene1");
	Engine::start();
	Engine::teardown();
}