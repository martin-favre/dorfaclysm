#include "MainGame.h"
#include "Engine.h"

void foo()
{

}
void MainGame::run()
{
	Engine::initialize(); 
	Engine::register_scene("scene1", foo);
	Engine::load_scene("scene1");
	Engine::start();
	Engine::teardown();
}