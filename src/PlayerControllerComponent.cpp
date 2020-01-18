#include "PlayerControllerComponent.h"

#include "Block.h"
#include "Camera.h"
#include "GameObject.h"
#include "GridMap.h"
#include "InputManager.h"
#include "JobPool.h"
#include "WorldItem.h"
#include "GridMapHelpers.h"
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
      const std::list<WorldItem*>& comps = gridMap.getWorldItemsAt(mousePos);
      for (const auto& comp : comps) {
        ASSERT(comp, "Received null WorldItem ptr");
        outStr += '\n' + comp->getName();
      }
      mTextComponent->setText(outStr);
    }
  }
}

void PlayerControllerComponent::handleClick() {
  Vector3DInt mousePos = InputManager::getMousePosition();
  mousePos += Camera::get().getPosition();
  mousePos = Camera::renderPosToTilePos(mousePos);
  GridMap& gridMap = GridMap::getActiveMap();

  if (gridMap.isPosInMap(mousePos)) {
    Block& block = gridMap.getBlockAt(mousePos);
    if (block.supportsJob(jobTypeMine)) {
      JobPool::addJob(PlayerRequestedJob(jobTypeMine, mousePos));
      block.assignJob(jobTypeMine);
    }

    GridMapHelpers::exploreMap(gridMap, mousePos);

  }
}

void PlayerControllerComponent::update() {
  renderText();
  if (InputManager::getMouseDown(1)) {
    handleClick();
  }
}