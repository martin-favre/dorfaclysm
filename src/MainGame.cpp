#include "MainGame.h"
#include "Engine.h"
#include "Component.h"
#include "GridMap.h"
class TextComponent:public Component
{
	public:
	TextComponent(GameObject& owner):Component(owner){}

	void setup() override
	{

	}
	
	void update() override
	{

	}
};

void foo()
{
	
}
void MainGame::run()
{
	GridMap map;
	//Engine::initialize(); 
	//Engine::registerScene("scene1", foo);
	//Engine::loadScene("scene1");
	//Engine::start();
	//Engine::teardown();
}