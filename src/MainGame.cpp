#include "MainGame.h"
#include "Engine.h"

void foo()
{

}
void MainGame::run()
{
	Engine::initialize(); 
	Engine::registerScene("scene1", foo);
	Engine::loadScene("scene1");
	Engine::start();
	Engine::teardown();
}