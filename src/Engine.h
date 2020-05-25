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
  static GameObject& addGameObject(Args&&... args) {
    std::unique_ptr<gameObjType> newObject =
        std::make_unique<gameObjType>(std::forward<Args>(args)...);

    GameObject* out = newObject.get();
    {
      std::scoped_lock lock(mMutex);
      Engine::mGameobjectsToAdd.push(std::move(newObject));
    }
    LOG("Added gameobject" << out);
    if (!mRunning) putGameObjectsIntoWorld();
    return *out;
  }

  /**
   * Note that unserializing a gameobject will make it forget whatever inherited type 
   * it may have been.
   * It's now a GameObject.
   */
  static GameObject& addGameObject(const SerializedObj& serObj) {
    std::unique_ptr<GameObject> newObject =
        std::make_unique<GameObject>(serObj);
    GameObject* out = newObject.get();
    {
      std::scoped_lock lock(mMutex);
      Engine::mGameobjectsToAdd.push(std::move(newObject));
    }
    LOG("Unserialized gameobject : " << out);
    if (!mRunning) putGameObjectsIntoWorld();
    return *out;
  }

  static void removeGameObject(GameObject* gObj);
  static void registerScene(const std::string& name, void (*scenecreator)());
  static void loadScene(const std::string& name);

  static GameObject* getGameObject(const Uuid& identifier);

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
  static bool mRunning;
  static bool mInitialized;
  static std::vector<std::unique_ptr<GameObject>> mGameobjects;
  static std::queue<std::unique_ptr<GameObject>> mGameobjectsToAdd;
  static std::set<GameObject*> mGameobjectsToRemove;
  static std::mutex mMutex;
};