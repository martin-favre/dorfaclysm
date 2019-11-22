#include <algorithm>

#include "Engine.h"
#include "InputManager.h"
#include "GraphicsManager.h"
#include "Timer.h"
#include "debug_closegameComponent.hpp"
std::map<std::string, void(*)()> Engine::mScenes;
std::string Engine::mSceneToLoad = "";
bool Engine::mAboutToLoadScene = false;
bool Engine::mRunning = false;
bool Engine::mInitialized = false;
GAMEOBJECT_ID Engine::mLatestGameobjectId{0};
std::vector<std::unique_ptr<GameObject>> Engine::mGameobjects;
std::queue<std::unique_ptr<GameObject>> Engine::mGameobjectsToAdd;
std::set<GameObject*> Engine::mGameobjectsToRemove;
ResourceArchive Engine::mEngineResources("EngineResources");

/* Public routines */
void Engine::initialize() {
	ASSERT(!Engine::mInitialized, "You can't initialize engine twice!");
	GraphicsManager::initialize();
	InputManager::initialize();
	Engine::mInitialized = true;
	Logging::log("Finished initialize Engine");
}

void Engine::teardown() {
	ASSERT(!mRunning, "You need to stop the engine first");
	clear_all_gameobjects();
	GraphicsManager::teardown();
	Logging::log("Finished teardown Engine");
	Engine::mInitialized = false;
}

void Engine::start() {
	ASSERT(Engine::mInitialized, "You need to initialize engine first!");
	Engine::mRunning = true;
	Logging::log("Starting Engine");
	Engine::main_loop();
}

void Engine::stop() {
	Engine::mRunning = false;
}

void Engine::remove_gameobject(GameObject * gObj) {

	Engine::mGameobjectsToRemove.insert(gObj);
}

size_t Engine::get_gameobject_count() {
	return Engine::mGameobjects.size();
}

void Engine::register_scene(const std::string & name, void (*scenecreator)()) {
	Engine::mScenes[name] = scenecreator;
}

void Engine::load_scene(const std::string & name) {
	ASSERT(mScenes.count(name), "No scene named " + name);
	mSceneToLoad = name;
	mAboutToLoadScene = true;
}
/* Private routines*/

void Engine::main_loop() {
	double ms_p_frame = 0.016666667 * 1000;
	(void)ms_p_frame;
	while (Engine::mRunning) {
		if (mAboutToLoadScene) {
			mAboutToLoadScene = false;
			replace_scene();
		}
		GraphicsManager::prepare_rendering();
		// Timer tim;
		// tim.start();
		Engine::put_gameobjects_into_world();
		Engine::remove_gameobject_from_world();
		InputManager::read_inputs();
		Engine::update_gameobjects();
		// int64_t time = tim.get_elapsed_milliseconds();
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
mScenes[mSceneToLoad]();
}


void Engine::update_gameobjects() {
	for (auto go = Engine::mGameobjects.begin();
		go != Engine::mGameobjects.end(); ++go) {
		(*go)->update_components();
	}
}

bool debug_render_colliders = true;

void Engine::render_gameobjects() {

	for (auto go = mGameobjects.begin();
		go != mGameobjects.end(); ++go)
	{
		(*go)->render();
	}

	GraphicsManager::execute_rendering();
}

void Engine::clear_all_gameobjects() {
	mGameobjects.clear();
	std::queue<std::unique_ptr<GameObject>>().swap(mGameobjectsToAdd);
	mGameobjectsToRemove.clear();

	GameObject * g = Engine::add_gameobject<GameObject>();
	g->add_component<Debug_CloseGameComponent>();
	g->name() = "Debug_CloseGameComponent";
}

void Engine::put_gameobjects_into_world() {
	std::vector<GameObject*> added_items(Engine::mGameobjectsToAdd.size());
	size_t added_objects = 0;
	while (!Engine::mGameobjectsToAdd.empty()) {
		std::unique_ptr<GameObject> new_item = std::move(Engine::mGameobjectsToAdd.front());
		Engine::mGameobjectsToAdd.pop();


		/* Does insert sort based on z - position
		   to decide render order.
		   large z = rendered later = "on top" / "closer to camera"
		*/
		size_t indx = 0;
		for (; indx != mGameobjects.size(); ++indx)
		{
			if (new_item->getRenderDepth() <= mGameobjects[indx]->getRenderDepth())
			{
				mGameobjects.insert(mGameobjects.begin() + indx, std::move(new_item));
				break;
			}
		}
		if (indx == mGameobjects.size())
		{
			mGameobjects.push_back(std::move(new_item));
		}
		added_items[added_objects] = new_item.get();
		added_objects++;
	}

	Engine::run_setups(added_items);
}
void Engine::remove_gameobject_from_world() {

	for (auto gObj_to_remove = mGameobjectsToRemove.begin();
		gObj_to_remove != mGameobjectsToRemove.end();
		++gObj_to_remove
		)
	{
		auto gObj_in_world = mGameobjects.begin();
		while (gObj_in_world != mGameobjects.end()) {
			if ((gObj_in_world->get()) == (*gObj_to_remove)) 
			{
				(*gObj_in_world)->teardown();
				gObj_in_world = mGameobjects.erase(gObj_in_world);
				continue;
			}

			++gObj_in_world;
		}
	}

}

void Engine::run_setups(std::vector<GameObject*> & gameobjects) {

	for (auto go = gameobjects.begin(); go != gameobjects.end(); ++go) {
		(*go)->setup();
	}
}

