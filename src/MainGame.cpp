#include "MainGame.h"

#include "AirDepthRenderer.h"
#include "Camera.h"
#include "CameraControllerComponent.h"
#include "Component.h"
#include "DorfController.h"
#include "Engine.h"
#include "Font.h"
#include "FpsCounter.h"
#include "GameObject.h"
#include "GridActor.h"
#include "GridMap.h"
#include "GridMapRenderer.h"
#include "ItemContainerObject.h"
#include "MapGenerator.h"
#include "Paths.h"
#include "PlayerControllerComponent.h"
#include "RenderDepths.h"
#include "ShowPositionComponent.h"
#include "Sprite.h"
#include "SpriteComponent.h"
#include "SpriteLoader.h"
#include "TextComponent.h"
#include "Vector2DInt.h"
#include "Serializer.h"
#include "RequestPoolComponent.h"
#include "GridMapLoader.h"

void foo() {
  int mapSize = 128;
  {
    GridMap::generateActiveMap({mapSize, mapSize, 64}, MapGenerator::generate);
  }
  Camera::get().move({0, 0, mapSize / 4});
  // {
  //   GameObject& gObj = Engine::addGameObject<GameObject>();
  //   std::unique_ptr<TextComponent> text =
  //       std::make_unique<TextComponent>(gObj, Paths::UBUNTU_FONT);
  //   text->setText("Hello");
  //   text->setFontSize(24);
  //   gObj.setPosition(Vector2D(10, 10));
  //   gObj.setScale(Vector2D(5,2.5));
  //   gObj.addComponent(std::move(text));
  // }
  {
    GameObject& gObj = Engine::addGameObject<GameObject>();
    gObj.addComponent<GridMapLoader>();
    gObj.setName("GridMapLoader");
  }
  {
    GameObject& gObj = Engine::addGameObject<GameObject>();
    gObj.addComponent<TextComponent>(Paths::UBUNTU_FONT, 24);
    gObj.setPosition({20, 20});
    gObj.setScale({2, 2});
    gObj.addComponent<FpsCounter>();
    gObj.setRenderDepth(RenderDepths::GUI);
    gObj.setName("Fpscounter");
  }
  {
    GameObject& gObj = Engine::addGameObject<GameObject>();
    gObj.addComponent<GridMapRenderer>();
    gObj.setRenderDepth(RenderDepths::GridMap);
    gObj.setName("Gridmaprenderer");
  }
  {
    GameObject& gObj = Engine::addGameObject<GameObject>();
    gObj.addComponent<AirDepthRenderer>();
    gObj.setRenderDepth(RenderDepths::AirDepth);
    gObj.setName("Airdepthrenderer");
  }
  {
    GameObject& gObj = Engine::addGameObject<GameObject>();
    gObj.addComponent<CameraControllerComponent>();
    gObj.setPosition({0, 0, mapSize / 2});
    gObj.setName("Cameracontroller");
  }
  {
    GameObject& gObj = Engine::addGameObject<GameObject>();
    gObj.addComponent<ShowPositionComponent>();
    gObj.addComponent<TextComponent>(Paths::UBUNTU_FONT, 24);
    gObj.setScale({2, 2});
    gObj.setPosition({20, 80});
    gObj.setRenderDepth(RenderDepths::GUI);
    gObj.setName("Show position text");
  }
  {
    GameObject& gObj = Engine::addGameObject<GameObject>();
    gObj.addComponent<RequestPoolComponent>();
    gObj.setName("RequestPoolObject");
  }
  {
    GameObject& gObj = Engine::addGameObject<GameObject>();
    gObj.setPosition({500, 0});
    gObj.addComponent<PlayerControllerComponent>();
    gObj.addComponent<TextComponent>(Paths::UBUNTU_FONT, 24);
    gObj.setRenderDepth(RenderDepths::GUI);
    gObj.setName("Playercontroller");
  }
  {
    for (int i = 0; i < 1; ++i) {
      GameObject gObj;
      gObj.addComponent<DorfController>();
      gObj.addComponent<GridActor>(GridActor::dorf);
      gObj.addComponent<SpriteComponent>(Paths::RG_TILE, Vector2DInt{25, 0});
      gObj.setPosition({0, 0, 1});
      gObj.setRenderDepth(RenderDepths::Actors);
      gObj.setName("Dorf");
      SerializedObj j = gObj.serialize();
      LOG(j);
      Engine::addGameObject(j);
    }
  }
  {
    for (int i = 0; i < 1; ++i) {
      GameObject& gObj = Engine::addGameObject<GameObject>();
      gObj.addComponent<DorfController>();
      gObj.addComponent<GridActor>(GridActor::dorf);
      gObj.addComponent<SpriteComponent>(Paths::RG_TILE, Vector2DInt{25, 0});
      gObj.setPosition({0, 0, 1});
      gObj.setRenderDepth(RenderDepths::Actors);
      gObj.setName("Dorf");
    }
  }

  {
    // GameObject& gObj = Engine::addGameObject<GameObject>();
    // gObj.addComponent<DrawLineComponent>();
    // gObj.addComponent<TextComponent>(Paths::UBUNTU_FONT, 24);
    // gObj.setPosition({20, 200});
    // gObj.setRenderDepth(RenderDepths::GUI);
  }
}

void MainGame::run() {
  Engine::initialize();
  Engine::registerScene("scene1", foo);
  Engine::loadScene("scene1");
  Engine::start();
  Engine::teardown();
}