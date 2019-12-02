#include "Engine.h"

#include <algorithm>
#include <string>

#include "GraphicsManager.h"
#include "Helpers.h"
#include "InputManager.h"
#include "Timer.h"
#include "debug_closegamecomponent.hpp"
std::map<std::string, void (*)()> Engine::mScenes;
std::string Engine::mSceneToLoad = "";
bool Engine::mAboutToLoadScene = false;
bool Engine::mRunning = false;
bool Engine::mInitialized = false;
GAMEOBJECT_ID Engine::mLatestGameobjectId{0};
std::vector<std::unique_ptr<GameObject>> Engine::mGameobjects;
std::queue<std::unique_ptr<GameObject>> Engine::mGameobjectsToAdd;
std::set<GameObject*> Engine::mGameobjectsToRemove;

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
  clearAllGameObjects();
  GraphicsManager::teardown();
  Logging::log("Finished teardown Engine");
  Engine::mInitialized = false;
}

void Engine::start() {
  ASSERT(Engine::mInitialized, "You need to initialize engine first!");
  Engine::mRunning = true;
  Logging::log("Starting Engine");
  Engine::mainLoop();
}

void Engine::stop() { Engine::mRunning = false; }

void Engine::removeGameObject(GameObject* gObj) {
  Engine::mGameobjectsToRemove.insert(gObj);
}

size_t Engine::getGameObjectCount() { return Engine::mGameobjects.size(); }

void Engine::registerScene(const std::string& name, void (*scenecreator)()) {
  Engine::mScenes[name] = scenecreator;
}

void Engine::loadScene(const std::string& name) {
  ASSERT(mScenes.count(name), "No scene named " + name);
  mSceneToLoad = name;
  mAboutToLoadScene = true;
}
/* Private routines*/

void Engine::mainLoop() {
  double ms_p_frame = 0.016666667 * 1000;
  (void)ms_p_frame;
  while (Engine::mRunning) {
    if (mAboutToLoadScene) {
      mAboutToLoadScene = false;
      replaceScene();
    }
    GraphicsManager::prepareRendering();
    // Timer tim;
    // tim.start();
    Engine::putGameObjectsIntoWorld();
    Engine::removeGameObjectFromWorld();
    InputManager::readInputs();
    Engine::updateGameObjects();
    // int64_t time = tim.getElapsedMilliseconds();
    Engine::renderGameObjects();
  }
}

void Engine::replaceScene() {
  /*
          Removes all gameobjects
          calls the sceneloader function
          this function will add the
          intial gameobjects to the engine.
  */
  clearAllGameObjects();
  mScenes[mSceneToLoad]();
}

void Engine::updateGameObjects() {
  for (auto go = Engine::mGameobjects.begin(); go != Engine::mGameobjects.end();
       ++go) {
    (*go)->updateComponents();
  }
}

bool debug_render_colliders = true;

void Engine::renderGameObjects() {
  for (auto go = mGameobjects.begin(); go != mGameobjects.end(); ++go) {
    (*go)->render();
  }

  GraphicsManager::executeRendering();
}

void Engine::clearAllGameObjects() {
  mGameobjects.clear();
  std::queue<std::unique_ptr<GameObject>>().swap(mGameobjectsToAdd);
  mGameobjectsToRemove.clear();

  GameObject* g = Engine::addGameObject<GameObject>();
  g->addComponent<Debug_CloseGameComponent>();
  g->name() = "Debug_CloseGameComponent";
}

void Engine::putGameObjectsIntoWorld() {
  std::vector<GameObject*> added_items;
  while (!Engine::mGameobjectsToAdd.empty()) {
    std::unique_ptr<GameObject> new_item =
        std::move(Engine::mGameobjectsToAdd.front());
    Engine::mGameobjectsToAdd.pop();

    // store away a ptr because we have to run setups afterwards
    added_items.emplace_back(new_item.get());
    size_t indx = 0;
    /* Does insert sort based on z - position
       to decide render order.
       large z = rendered later = "on top" / "closer to camera"
    */
    for (; indx != mGameobjects.size(); ++indx) {
      if (new_item->getRenderDepth() <= mGameobjects[indx]->getRenderDepth()) {
        mGameobjects.insert(mGameobjects.begin() + indx, std::move(new_item));
        break;
      }
    }
    if (indx == mGameobjects.size()) {
      mGameobjects.push_back(std::move(new_item));
    }
  }

  Engine::runSetups(added_items);
}
void Engine::removeGameObjectFromWorld() {
  for (auto gObj_to_remove = mGameobjectsToRemove.begin();
       gObj_to_remove != mGameobjectsToRemove.end(); ++gObj_to_remove) {
    auto gObj_in_world = mGameobjects.begin();
    while (gObj_in_world != mGameobjects.end()) {
      if ((gObj_in_world->get()) == (*gObj_to_remove)) {
        (*gObj_in_world)->teardown();
        gObj_in_world = mGameobjects.erase(gObj_in_world);
        continue;
      }

      ++gObj_in_world;
    }
  }
}

void Engine::runSetups(std::vector<GameObject*>& gameobjects) {
  for (auto go = gameobjects.begin(); go != gameobjects.end(); ++go) {
    (*go)->setup();
  }
}
