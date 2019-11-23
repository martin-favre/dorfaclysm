#pragma once


#include <vector>
#include <limits>
#include <queue>
#include <map>
#include <set>
#ifdef __GNUC__
#include <execinfo.h>
#endif


#include "GameObject.h"
#include "ResourceArchive.h"
#include "Logging.h"
#include <memory>
/*
	Usage:
	void foo()
	{
	  Add gameobjects here
	}

	Engine::initialize(); 
	Engine::registerScene("scene1", foo);
	Engine::loadScene("scene1");
	Engine::start();
	Engine::teardown();

*/

class GameObject;
class Engine {
public:
	static void initialize();
	static void teardown();
	static void start();
	static void stop();
	template <class gameObjectType>
	static gameObjectType* addGameObject();
	static void removeGameObject(GameObject * gObj);
	static size_t getGameObjectCount();
	static void registerScene(const std::string & name,  void (*scenecreator)());
	static void loadScene(const std::string & name);

private:
	Engine();
	static void mainLoop();
	static void replaceScene();
	static void updateGameObjects();
	static void renderGameObjects();
	static void clearAllGameObjects();
	
	/*Actually puts the changes in place*/
	static void putGameObjectsIntoWorld();
	static void removeGameObjectFromWorld();
	static void runSetups(std::vector<GameObject*> & );
	
	static std::map<std::string, void(*)()> mScenes;

	static std::string mSceneToLoad;
	static bool mAboutToLoadScene;
	static GAMEOBJECT_ID mLatestGameobjectId;
	static bool mRunning;
	static bool mInitialized;
	static std::vector<std::unique_ptr<GameObject>> mGameobjects;
	static std::queue<std::unique_ptr<GameObject>> mGameobjectsToAdd;
	static std::set<GameObject*> mGameobjectsToRemove;
	static ResourceArchive mEngineResources;
};

template <class gameObjectType>
gameObjectType * Engine::addGameObject() {
	GAMEOBJECT_ID id = Engine::mLatestGameobjectId;
	std::unique_ptr<GameObject> newObject = std::make_unique<GameObject>(id);
	if (Engine::mLatestGameobjectId >= std::numeric_limits<int>::max()) {
		Logging::log("Warning, gameobject id overflow");
	}
	gameObjectType * out = newObject.get();
	Engine::mGameobjectsToAdd.push(std::move(newObject));
	Engine::mLatestGameobjectId++;
	Logging::log("Added gameobject id " + std::to_string(id) + " type " + typeid(gameObjectType).name());
	if (!mRunning) putGameObjectsIntoWorld();
	return out;
}