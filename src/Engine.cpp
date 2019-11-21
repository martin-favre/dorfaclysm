#include <algorithm>

#include "Engine.h"
#include "InputManager.h"
#include "GraphicsManager.h"
#include "Timer.h"
#include "debug_closegameComponent.hpp"
std::map<std::string, void(*)()> Engine::m_scenes;
std::string Engine::m_scene_to_load = "";
bool Engine::m_about_to_load_scene = false;
bool Engine::m_running = false;
bool Engine::m_initialized = false;
GAMEOBJECT_ID Engine::m_latest_gameobject_id = 0;
std::vector<GameObject *> Engine::m_gameobjects;
std::queue<GameObject*> Engine::m_gameobjects_to_add;
std::set<GameObject*> Engine::m_gameobjects_to_remove;
ResourceArchive Engine::m_engine_resources("EngineResources");

/* Public routines */
void Engine::initialize() {
	ASSERT(!Engine::m_initialized, "You can't initialize engine twice!");
	GraphicsManager::initialize();
	InputManager::initialize();
	Engine::m_initialized = true;
	Logging::log("Finished initialize Engine");
}

void Engine::teardown() {

	clear_all_gameobjects();
	GraphicsManager::teardown();
	Logging::log("Finished teardown Engine");
	Engine::m_initialized = false;
}

void Engine::start() {
	ASSERT(Engine::m_initialized, "You need to initialize engine first!");
	Engine::m_running = true;
	Logging::log("Starting Engine");
	Engine::main_loop();
}

void Engine::stop() {
	Engine::m_running = false;
}

//GameObject * Engine::get_gameobject(const GAMEOBJECT_ID id) {
//	if (!Engine::m_gameobjects.count(id)) return nullptr;
//	return Engine::m_gameobjects[id];
//}

void Engine::remove_gameobject(GameObject * gObj) {

	Engine::m_gameobjects_to_remove.insert(gObj);
}

size_t Engine::get_gameobject_count() {
	return Engine::m_gameobjects.size();
}

void Engine::register_scene(const std::string & name, void (*scenecreator)()) {
	Engine::m_scenes[name] = scenecreator;
}

void Engine::load_scene(const std::string & name) {
	ASSERT(m_scenes.count(name), "No scene named " + name);
	m_scene_to_load = name;
	m_about_to_load_scene = true;
}
/* Private routines*/

void Engine::main_loop() {
	double ms_p_frame = 0.016666667 * 1000;
	(void)ms_p_frame;
	while (Engine::m_running) {
		if (m_about_to_load_scene) {
			m_about_to_load_scene = false;
			replace_scene();
		}
		GraphicsManager::prepare_rendering();
		Timer tim;
		tim.start();
		Engine::put_gameobjects_into_world();
		Engine::remove_gameobject_from_world();
		InputManager::read_inputs();
		Engine::update_gameobjects();
		int64_t time = tim.get_elapsed_milliseconds();
		Engine::render_gameobjects();
	}
}

void Engine::replace_scene() {
/*
	Removes all gameobjects
	calls the sceneloader function
	this function will add the
	intial gameobjects to the engine.
*/
clear_all_gameobjects();
m_scenes[m_scene_to_load]();
}


void Engine::update_gameobjects() {
	for (auto go = Engine::m_gameobjects.begin();
		go != Engine::m_gameobjects.end(); ++go) {
		(*go)->update_components();
	}
}

bool debug_render_colliders = true;

void Engine::render_gameobjects() {

	for (auto go = m_gameobjects.begin();
		go != m_gameobjects.end(); ++go)
	{
		(*go)->render();
	}

	GraphicsManager::execute_rendering();
}

void Engine::clear_all_gameobjects() {
	m_gameobjects.clear();
	std::queue<GameObject*>().swap(m_gameobjects_to_add);
	m_gameobjects_to_remove.clear();

	auto g = Engine::add_gameobject<GameObject>();
	g->add_component<Debug_CloseGameComponent>();
	g->name() = "Debug_CloseGameComponent";
	ResourceFile* font = m_engine_resources.get_item("Fonts\\calibri.ttf");
}

void Engine::put_gameobjects_into_world() {
	std::vector<GameObject*> added_items(Engine::m_gameobjects_to_add.size());
	size_t added_objects = 0;
	while (!Engine::m_gameobjects_to_add.empty()) {
		GameObject* new_item = Engine::m_gameobjects_to_add.front();
		Engine::m_gameobjects_to_add.pop();


		/* Does insert sort based on z - position
		   to decide render order.
		   large z = rendered later = "on top" / "closer to camera"
		*/
		int indx = 0;
		for (; indx != m_gameobjects.size(); ++indx)
		{
			if (new_item->getRenderDepth() <= m_gameobjects[indx]->getRenderDepth())
			{
				m_gameobjects.insert(m_gameobjects.begin() + indx, new_item);
				break;
			}
		}
		if (indx == m_gameobjects.size())
		{
			m_gameobjects.push_back(new_item);
		}
		added_items[added_objects] = new_item;
		added_objects++;
	}

	Engine::run_setups(added_items);
}
void Engine::remove_gameobject_from_world() {

	for (auto gObj_to_remove = m_gameobjects_to_remove.begin();
		gObj_to_remove != m_gameobjects_to_remove.end();
		++gObj_to_remove
		)
	{

		auto gObj_in_world = m_gameobjects.begin();
		while (gObj_in_world != m_gameobjects.end()) {
			if ((*gObj_in_world) == (*gObj_to_remove)) 
			{
				(*gObj_in_world)->teardown();
				delete* gObj_in_world;
				gObj_in_world = m_gameobjects.erase(gObj_in_world);
				continue;
			}

			++gObj_in_world;
		}
	}

	//while (!Engine::m_gameobjects_to_remove.empty()) {
	//	GameObject * gObj = Engine::m_gameobjects_to_remove.front();
	//	ASSERT(gObj != nullptr, "Trying to remove null gObject");
	//	gObj->teardown();
	//	Engine::m_gameobjects_to_remove.pop();
	//	for (auto go = Engine::m_gameobjects.begin();
	//		go != Engine::m_gameobjects.end(); ++go)
	//	{
	//		if ((*go) == gObj)
	//		{
	//			Engine::m_gameobjects.erase
	//		}
	//	}
	//		Engine::m_gameobjects.
	//}
}


void Engine::run_setups(std::vector<GameObject*> & gameobjects) {

	for (auto go = gameobjects.begin(); go != gameobjects.end(); ++go) {
		(*go)->setup();
	}
}

