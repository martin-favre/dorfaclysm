#include "Engine.h"

#include <algorithm>
#include <string>
#include <thread>

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
std::mutex Engine::mMutex;

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
  std::scoped_lock lock(mMutex);
  Engine::mGameobjectsToRemove.insert(gObj);
}

void Engine::registerScene(const std::string& name, void (*scenecreator)()) {
  Engine::mScenes[name] = scenecreator;
}

void Engine::loadScene(const std::string& name) {
  ASSERT(mScenes.count(name), "No scene named " + name);
  mSceneToLoad = name;
  mAboutToLoadScene = true;
}
/* Private routines*/

void Engine::logicThreadMainLoop() {
  Timer timer;
  timer.start();
  int maxFps = 60;
  int msPerFrame = 1000 * 1 / maxFps;
  while (Engine::mRunning) {
    if (mAboutToLoadScene) {
      mAboutToLoadScene = false;
      replaceScene();
    }
    Engine::putGameObjectsIntoWorld();
    Engine::removeGameObjectFromWorld();
    Engine::updateGameObjects();
    auto elapsedMs = timer.getElapsedMilliseconds();
    auto timeLeft = msPerFrame - elapsedMs;
    if (timeLeft > 0)
      std::this_thread::sleep_for(std::chrono::milliseconds(timeLeft));
  }
}

void Engine::mainLoop() {
  std::thread logicThread(logicThreadMainLoop);
  while (Engine::mRunning) {
    InputManager::readInputs();
    GraphicsManager::prepareRendering();
    Engine::renderGameObjects();
  }
  logicThread.join();
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
  for (auto& go : mGameobjects) {
    go->updateComponents();
  }
}

bool debug_render_colliders = true;

void Engine::renderGameObjects() {
  std::scoped_lock lock(mMutex);
  for (auto& go : mGameobjects) {
    go->render();
  }

  GraphicsManager::executeRendering();
}

void Engine::clearAllGameObjects() {
  {
    std::scoped_lock lock(mMutex);
    mGameobjects.clear();
    std::queue<std::unique_ptr<GameObject>>().swap(mGameobjectsToAdd);
    mGameobjectsToRemove.clear();
  }
  GameObject& g = Engine::addGameObject<GameObject>();
  g.addComponent<Debug_CloseGameComponent>();
  g.name() = "Debug_CloseGameComponent";
}

void Engine::putGameObjectsIntoWorld() {
  std::vector<GameObject*> addedItems;
  {
    std::scoped_lock lock(mMutex);
    while (!Engine::mGameobjectsToAdd.empty()) {
      std::unique_ptr<GameObject> newItem =
          std::move(Engine::mGameobjectsToAdd.front());
      Engine::mGameobjectsToAdd.pop();

      // store away a ptr because we have to run setups afterwards
      addedItems.emplace_back(newItem.get());
      size_t indx = 0;
      /* Does insert sort based on renderdepth
         to decide render order.
         large renderdepth = rendered later = "on top" / "closer to camera"
      */
      for (; indx != mGameobjects.size(); ++indx) {
        if (newItem->getRenderDepth() <= mGameobjects[indx]->getRenderDepth()) {
          mGameobjects.insert(mGameobjects.begin() + indx, std::move(newItem));
          break;
        }
      }
      if (indx == mGameobjects.size()) {
        mGameobjects.push_back(std::move(newItem));
      }
    }
  }
  Engine::runSetups(addedItems);
}
void Engine::removeGameObjectFromWorld() {
  std::scoped_lock lock(mMutex);
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
  for (auto& go : gameobjects) {
    go->setup();
  }
}
