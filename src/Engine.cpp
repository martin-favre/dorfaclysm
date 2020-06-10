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
std::vector<std::unique_ptr<GameObject>> Engine::mGameobjects;
std::vector<std::unique_ptr<GameObject>> Engine::mGameobjectsToAdd;
std::set<GameObject*> Engine::mGameobjectsToRemove;
std::mutex Engine::mMutex;

bool Engine::mSavingRequested{false};
std::unique_ptr<SerializedObj> Engine::mSavedState;
std::unique_ptr<SerializedObj> Engine::mLoadState;

/* Public routines */
void Engine::initialize() {
  ASSERT(!Engine::mInitialized, "You can't initialize engine twice!");
  GraphicsManager::initialize();
  InputManager::initialize();
  Logging::initialize();
  Engine::mInitialized = true;
  LOGL("Finished initialize Engine", Logging::info);
}

void Engine::teardown() {
  ASSERT(!mRunning, "You need to stop the engine first");
  clearAllGameObjects();
  GraphicsManager::teardown();
  LOGL("Finished teardown Engine", Logging::info);
  Logging::teardown();
  Engine::mInitialized = false;
}

void Engine::start() {
  ASSERT(Engine::mInitialized, "You need to initialize engine first!");
  Engine::mRunning = true;
  LOGL("Starting Engine", Logging::info);
  Engine::mainLoop();
}

void Engine::stop() { Engine::mRunning = false; }

void Engine::removeGameObject(GameObject* gObj) {
  std::scoped_lock lock(mMutex);
  LOG("Removing GameObject " << gObj->name() << " " << gObj);
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

    if (mLoadState) {
      loadScene();
    }
    if (mSavingRequested) {
      saveScene();
    }

    auto elapsedMs = timer.getElapsedMilliseconds();
    auto timeLeft = msPerFrame - elapsedMs;
    if (timeLeft > 0)
      std::this_thread::sleep_for(std::chrono::milliseconds(timeLeft));
    timer.start();
  }
}

void Engine::mainLoop() {
  std::thread logicThread(logicThreadMainLoop);
  Timer timer;
  timer.start();
  int maxFps = 100;
  int msPerFrame = 1000 * 1 / maxFps;
  while (Engine::mRunning) {
    InputManager::readInputs();
    GraphicsManager::prepareRendering();
    Engine::renderGameObjects();
    auto elapsedMs = timer.getElapsedMilliseconds();
    auto timeLeft = msPerFrame - elapsedMs;
    if (timeLeft > 0)
      std::this_thread::sleep_for(std::chrono::milliseconds(timeLeft));
    timer.start();
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
  GameObject& g = Engine::addGameObject<GameObject>();
  g.addComponent<Debug_CloseGameComponent>();
  g.setName("Debug_CloseGameComponent");
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
    mGameobjectsToAdd.clear();
    mGameobjectsToRemove.clear();
  }
}

void Engine::putGameObjectsIntoWorld() {
  std::vector<GameObject*> addedItems;
  {
    std::scoped_lock lock(mMutex);
    for (auto it = mGameobjectsToAdd.begin(); it != mGameobjectsToAdd.end();
         it++) {
      std::unique_ptr<GameObject> newItem = std::move(*it);
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
  mGameobjectsToAdd.clear();
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
  mGameobjectsToRemove.clear();
}

void Engine::runSetups(std::vector<GameObject*>& gameobjects) {
  for (auto& go : gameobjects) {
    go->setup();
  }
}

GameObject* Engine::getGameObject(const Uuid& identifier) {
  for (auto& go : mGameobjects) {
    if (go->getIdentifier() == identifier) {
      return go.get();
    }
  }
  return nullptr;
}

void Engine::requestStateSave() {
  ASSERT(!mSavingRequested, "Already trying to save");
  mSavingRequested = true;
}
bool Engine::saveStateAvailable() { return mSavedState.get(); }

std::unique_ptr<SerializedObj> Engine::getSaveState() {
  return std::move(mSavedState);
}

void Engine::requestLoadState(std::unique_ptr<SerializedObj>&& loadState) {
  mLoadState = std::move(loadState);
}

void Engine::loadScene() {
  ASSERT(mLoadState.get(), "No load state available");
  {
    std::vector<SerializedObj> objsToAdd = mLoadState->at("objects_to_add");
    for (const auto& obj : objsToAdd) {
      mGameobjectsToAdd.emplace_back(std::make_unique<GameObject>(obj));
    }
  }
{
    // gently clear all gameobjects
    for (const auto& gObj : mGameobjects) {
      removeGameObject(gObj.get());
    }
    removeGameObjectFromWorld();
    ASSERT(mGameobjects.size() == 0, "Didn't clear all Gobjs");
    std::vector<SerializedObj> objsToAdd = mLoadState->at("objects");
    for (const auto& obj : objsToAdd) {
      addGameObject(obj);
    }
  }
  {
    mGameobjectsToRemove.clear();
    std::vector<SerializedObj> objsToAdd = mLoadState->at("objects_to_remove");
    for (const auto& obj : objsToAdd) {
      Uuid id = obj;
      GameObject* gObj = getGameObject(id);
      mGameobjectsToRemove.emplace(gObj);
    }
  }
  mLoadState.reset();
}

void Engine::saveScene() {
  mSavedState = std::make_unique<SerializedObj>();
  {
    std::vector<SerializedObj> objsToAdd;
    for (const auto& go : mGameobjectsToAdd) {
      objsToAdd.emplace_back(go->serialize());
    }
    (*mSavedState)["objects_to_add"] = objsToAdd;
  }
  {
    std::vector<SerializedObj> objs;
    for (const auto& go : mGameobjects) {
      objs.emplace_back(go->serialize());
    }
    (*mSavedState)["objects"] = objs;
  }
  {
    std::vector<SerializedObj> objsToRemove;
    for (const auto& go : mGameobjects) {
      objsToRemove.emplace_back(go->getIdentifier());
    }
    (*mSavedState)["objects_to_remove"] = objsToRemove;
  }
  mSavingRequested = false;
}