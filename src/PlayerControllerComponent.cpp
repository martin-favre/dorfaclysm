#include "PlayerControllerComponent.h"

#include "Block.h"
#include "BlockBuildObject.h"
#include "BlockBuildingRequestPool.h"
#include "Camera.h"
#include "Engine.h"
#include "GameObject.h"
#include "GridActor.h"
#include "GridMap.h"
#include "GridMapHelpers.h"
#include "MineBlockObject.h"
#include "MiningRequestPool.h"
#include "PlayerRequestType.h"
#include "RockBlockItem.h"
#include "PlayerInputRequests.h"
#include "InputManager.h"

void PlayerControllerComponent::setup() {
  mTextComponent = owner().getComponent<TextComponent>();
}

void PlayerControllerComponent::renderText() {
  if (mTextComponent) {
    Vector3DInt mousePos = InputManager::getMousePosition();
    mousePos += Camera::get().getPosition();
    mousePos = Camera::renderPosToTilePos(mousePos);

    const GridMap& gridMap = GridMap::getActiveMap();
    std::string outStr;
    if (gridMap.isPosInMap(mousePos)) {
      const Block& block = gridMap.getBlockAt(mousePos);
      outStr += block.getName();
      const std::list<GridActor*>& comps = gridMap.getGridActorsAt(mousePos);
      for (const auto& comp : comps) {
        ASSERT(comp, "Received null GridActor ptr");
        outStr += '\n' + comp->getName();
      }
      if (mMode == mine) outStr += "\nMining";
      if (mMode == place) outStr += "\nPlacing";
      if (mMode == clear) outStr += "\nClearing";
      mTextComponent->setText(outStr);
    }
  }
}

void PlayerControllerComponent::handleClick() {
  MouseClickInfo mouseInfo = PlayerInputRequests::dequeueMouseClick();
  if(mouseInfo.getButton() != 1) return;
  Vector3DInt mousePos = mouseInfo.getPos();
  mousePos += Camera::get().getPosition();
  mousePos = Camera::renderPosToTilePos(mousePos);
  GridMap& gridMap = GridMap::getActiveMap();
  if (!gridMap.isPosInMap(mousePos)) return;

  if (mMode == Mode::mine) {
    const Block& block = gridMap.getBlockAt(mousePos);
    if (block.supportsJob(requestTypeMining)) {
      for (const auto& actor : gridMap.getGridActorsAt(mousePos)) {
        // if we're not already building here
        if (dynamic_cast<MineBlockObject*>(&actor->owner()) != nullptr) return;
      }
      GameObject& gObj = Engine::addGameObject<MineBlockObject>();
      gObj.setPosition(mousePos);
    }
  } else if (mMode == Mode::place) {
    std::weak_ptr<Block> block = gridMap.getBlockPtrAt(mousePos);
    if (block.lock()->supportsJob(requestTypePlacing)) {
      for (const auto& actor : gridMap.getGridActorsAt(mousePos)) {
        // if we're not already building here
        if (dynamic_cast<BlockBuildObject*>(&actor->owner()) != nullptr) return;
      }
      GameObject& gObj = Engine::addGameObject<BlockBuildObject>(
          generateItemType<RockBlockItem>());
      gObj.setPosition(mousePos);
    }
  } else if (mMode == Mode::clear) {
    const auto& items = gridMap.getGridActorsAt(mousePos);
    for (auto& item : items) {
      if (dynamic_cast<BlockBuildObject*>(&item->owner()) != nullptr ||
          dynamic_cast<MineBlockObject*>(&item->owner()) != nullptr) {
        Engine::removeGameObject(&item->owner());
      }
    }
  }
}

void PlayerControllerComponent::update() {
  if (PlayerInputRequests::hasMouseClicks()) handleClick();

  if (PlayerInputRequests::hasKeyPresses()) {
    int key = PlayerInputRequests::dequeueKeyPress();
    if (key == SDL_SCANCODE_TAB) {
      if (mMode == mine) {
        mMode = place;
      } else if (mMode == place) {
        mMode = clear;
      } else {
        mMode = mine;
      }
    }
  }
}

void PlayerControllerComponent::render() { renderText(); }
