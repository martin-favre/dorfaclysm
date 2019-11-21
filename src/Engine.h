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
	Engine::load_scene("scene1);
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
	template <class gameobject_type>
	static gameobject_type * get_gameobject(const GAMEOBJECT_ID id);
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
	
	static std::map<std::string, void(*)()> m_scenes;

	static std::string m_scene_to_load;
	static bool m_about_to_load_scene;

	static bool m_running;
	static bool m_initialized;
	static std::vector<GameObject *> m_gameobjects;
	static std::queue<GameObject*> m_gameobjects_to_add;
	static std::set<GameObject*> m_gameobjects_to_remove;
	static GAMEOBJECT_ID m_latest_gameobject_id;
	static ResourceArchive m_engine_resources;
};

template <class gameobject_type>
gameobject_type * Engine::add_gameobject() {
	GAMEOBJECT_ID id = Engine::m_latest_gameobject_id;
	gameobject_type * new_object = new gameobject_type(id);
	if (Engine::m_latest_gameobject_id == std::numeric_limits<int>::max()) {
		Logging::log("Warning, gameobject id overflow");
	}
	Engine::m_gameobjects_to_add.push(new_object);
	Engine::m_latest_gameobject_id++;
	Logging::log("Added gameobject id " + std::to_string(id) + " type " + typeid(gameobject_type).name());
	if (!m_running) put_gameobjects_into_world();
	return new_object;
}

template <class gameobject_type>
gameobject_type * Engine::get_gameobject(const GAMEOBJECT_ID id) {
	for (auto gObj_in_world = m_gameobjects.begin();
		gObj_in_world != m_gameobjects.end();
		++gObj_in_world
		)
	{
		if ((*gObj_in_world)->id() == id)
		{
			std::static_pointer_cast<gameobject_type>(*gObj_in_world);
		}
	}
	return nullptr;
}
