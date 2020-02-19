#pragma once

#include <limits>
#include <map>
#include <memory>
#include <queue>
#include <set>
#include <vector>
#include <mutex>
#include "GameObject.h"
#include "Logging.h"
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

  template <typename gameObjType, class... Args>
  static gameObjType& addGameObject(Args&&... args) {
    GAMEOBJECT_ID id = Engine::mLatestGameobjectId;
    std::unique_ptr<gameObjType> newObject =
        std::make_unique<gameObjType>(id, std::forward<Args>(args)...);
    if (Engine::mLatestGameobjectId >=
        std::numeric_limits<unsigned long>::max()) {
      Logging::log("Warning, gameobject id overflow");
    }
    gameObjType* out = newObject.get();
    {
      std::scoped_lock lock(mMutex);
      Engine::mGameobjectsToAdd.push(std::move(newObject));
    }
    Engine::mLatestGameobjectId++;
    Logging::log("Added gameobject id " + std::to_string(id) + " type " +
                 typeid(gameObjType).name());
    if (!mRunning) putGameObjectsIntoWorld();
    return *out;
  }
  static void removeGameObject(GameObject* gObj);
  static void registerScene(const std::string& name, void (*scenecreator)());
  static void loadScene(const std::string& name);

 private:
  Engine();
  static void mainLoop();
  static void replaceScene();
  static void updateGameObjects();
  static void renderGameObjects();
  static void clearAllGameObjects();
  static void logicThreadMainLoop();
  /*Actually puts the changes in place*/
  static void putGameObjectsIntoWorld();
  static void removeGameObjectFromWorld();
  static void runSetups(std::vector<GameObject*>&);

  static std::map<std::string, void (*)()> mScenes;

  static std::string mSceneToLoad;
  static bool mAboutToLoadScene;
  static GAMEOBJECT_ID mLatestGameobjectId;
  static bool mRunning;
  static bool mInitialized;
  static std::vector<std::unique_ptr<GameObject>> mGameobjects;
  static std::queue<std::unique_ptr<GameObject>> mGameobjectsToAdd;
  static std::set<GameObject*> mGameobjectsToRemove;
  static std::mutex mMutex;
};