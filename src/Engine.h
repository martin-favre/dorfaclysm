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
	Engine::register_scene("scene1", foo);
	Engine::load_scene("scene1");
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
	template <class gameobject_type>
	static gameobject_type* add_gameobject();
	static void remove_gameobject(GameObject * gObj);
	static size_t get_gameobject_count();
	static void register_scene(const std::string & name,  void (*scenecreator)());
	static void load_scene(const std::string & name);

private:
	Engine();
	static void main_loop();
	static void replace_scene();
	static void update_gameobjects();
	static void render_gameobjects();
	static void clear_all_gameobjects();
	
	/*Actually puts the changes in place*/
	static void put_gameobjects_into_world();
	static void remove_gameobject_from_world();
	static void run_setups(std::vector<GameObject*> & );
	
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

template <class gameobject_type>
gameobject_type * Engine::add_gameobject() {
	GAMEOBJECT_ID id = Engine::mLatestGameobjectId;
	std::unique_ptr<GameObject> newObject = std::make_unique<GameObject>(id);
	if (Engine::mLatestGameobjectId >= std::numeric_limits<int>::max()) {
		Logging::log("Warning, gameobject id overflow");
	}
	gameobject_type * out = newObject.get();
	Engine::mGameobjectsToAdd.push(std::move(newObject));
	Engine::mLatestGameobjectId++;
	Logging::log("Added gameobject id " + std::to_string(id) + " type " + typeid(gameobject_type).name());
	if (!mRunning) put_gameobjects_into_world();
	return out;
}