#include "MainGame.h"

#include "Component.h"
#include "Engine.h"
#include "Font.h"
#include "GridMap.h"
#include "Paths.h"
#include "Sprite.h"
#include "TextComponent.h"
#include "Vector2DInt.h"
#include "SpriteComponent.h"

void foo() {
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
    auto sprite = std::make_unique<SpriteComponent>(gObj, Paths::GRASS_TILE, Rect{64,64, 64, 64});
    gObj.addComponent(std::move(sprite));
    gObj.setPosition(Vector2D(200, 200));
  }
}

void MainGame::run() {
  GridMap map(Vector2DInt(100, 100));
  Engine::initialize();
  Engine::registerScene("scene1", foo);
  Engine::loadScene("scene1");
  Engine::start();
  Engine::teardown();
}