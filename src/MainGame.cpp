#include "MainGame.h"

#include "Component.h"
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
void foo() {
  { GridMap::generateActiveMap({500, 500}, MapGenerator::generate); }
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
  }
  {
    GameObject& gObj = Engine::addGameObject<GameObject>();
    gObj.addComponent<GridMapRenderer>();
    // auto comp = Component::create<GridMapRenderer>(gObj);
    // gObj.addComponent<GridMapRenderer>(std::move(comp));
  }
  {
    // GameObject& gObj = Engine::addGameObject<GameObject>();
    // auto sprite = std::make_unique<SpriteComponent>(gObj,
    // SpriteLoader::loadSprite(Paths::GRASS_TILE, {64, 64, 64, 64})); auto
    // sprite = std::make_unique<SpriteComponent>(gObj,
    // SpriteLoader::loadSpriteByIndex(Paths::GRASS_TILE, {1,1},
    // Paths::SIZE_OF_GRASS_TILE)); gObj.addComponent(std::move(sprite));
    // gObj.setPosition(Vector2D(200, 200));
  }
}

void MainGame::run() {
  Engine::initialize();
  Engine::registerScene("scene1", foo);
  Engine::loadScene("scene1");
  Engine::start();
  Engine::teardown();
}