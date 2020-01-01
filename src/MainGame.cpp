#include "MainGame.h"

#include "CameraControllerComponent.h"
#include "Component.h"
#include "DorfController.h"
#include "Engine.h"
#include "Font.h"
#include "FpsCounter.h"
#include "GridMap.h"
#include "GridMapRenderer.h"
#include "MapGenerator.h"
#include "Paths.h"
#include "Sprite.h"
#include "SpriteComponent.h"
#include "SpriteLoader.h"
#include "TextComponent.h"
#include "Vector2DInt.h"
#include "PlayerControllerComponent.h"
#include "WorldItemComponent.h"
void foo() {
  { GridMap::generateActiveMap({100, 100}, MapGenerator::generate); }
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
    gObj.addComponent<TextComponent>(Paths::UBUNTU_FONT, 24);
    gObj.setPosition({20, 20});
    gObj.setScale({2, 2});
    gObj.addComponent<FpsCounter>();
    gObj.setRenderDepth(100);
  }
  {
    GameObject& gObj = Engine::addGameObject<GameObject>();
    gObj.addComponent<GridMapRenderer>();
    gObj.setRenderDepth(-100);
  }
  {
    GameObject& gObj = Engine::addGameObject<GameObject>();
    gObj.addComponent<CameraControllerComponent>();
  }
  {
    GameObject& gObj = Engine::addGameObject<GameObject>();
    gObj.setPosition({500, 0});
    gObj.addComponent<PlayerControllerComponent>();
    gObj.addComponent<TextComponent>(Paths::UBUNTU_FONT, 24);
  }
  {
    for (int i = 0; i < 100; ++i) {
      GameObject& gObj = Engine::addGameObject<GameObject>();
      gObj.addComponent<DorfController>();
      gObj.addComponent<WorldItemComponent>("Dorf");
      gObj.addComponent<SpriteComponent>(SpriteLoader::loadSpriteByIndex(
          Paths::NPC_TILE, {0, 0}, Paths::SIZE_OF_NPC_TILE));
      gObj.setScale({2,2});
    }
  }
}

void MainGame::run() {
  Engine::initialize();
  Engine::registerScene("scene1", foo);
  Engine::loadScene("scene1");
  Engine::start();
  Engine::teardown();
}