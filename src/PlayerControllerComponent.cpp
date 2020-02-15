#include "PlayerControllerComponent.h"

#include "Block.h"
#include "BlockBuildComponent.h"
#include "BlockBuildObject.h"
#include "BlockBuildingRequestPool.h"
#include "Camera.h"
#include "GameObject.h"
#include "GridActor.h"
#include "GridMap.h"
#include "GridMapHelpers.h"
#include "InputManager.h"
#include "MiningRequestPool.h"
#include "PlayerRequestType.h"
#include "RockBlockItem.h"

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
      outStr += mMode == Mode::mine ? "\nMining" : "\nPlacing";
      mTextComponent->setText(outStr);
    }
  }
}

void PlayerControllerComponent::handleClick() {
  Vector3DInt mousePos = InputManager::getMousePosition();
  mousePos += Camera::get().getPosition();
  mousePos = Camera::renderPosToTilePos(mousePos);
  GridMap& gridMap = GridMap::getActiveMap();
  if (!gridMap.isPosInMap(mousePos)) return;

  if (mMode == Mode::mine) {
    std::weak_ptr<Block> block = gridMap.getBlockPtrAt(mousePos);
    if (block.lock()->supportsJob(requestTypeMining)) {
      MiningRequestPool::getInstance().addRequest(
          std::make_unique<MiningRequest>(block, mousePos));
    }

    GridMapHelpers::exploreMap(gridMap, mousePos);
  } else {
    std::weak_ptr<Block> block = gridMap.getBlockPtrAt(mousePos);
    if (block.lock()->supportsJob(requestTypePlacing)) {
      for (const auto& actor : gridMap.getGridActorsAt(mousePos)) {
        // if we're not already building here
        if (actor->owner().getComponent<BlockBuildComponent>() != nullptr)
          return;
      }
      GameObject& gObj =
          Engine::addGameObject<BlockBuildObject>(getItemType<RockBlockItem>());
      gObj.setPosition(mousePos);
    }
  }
}

void PlayerControllerComponent::update() {
  renderText();
  if (InputManager::getMouseDown(1)) {
    handleClick();
  }
  if (InputManager::getKeyDown(SDL_SCANCODE_TAB)) {
    mMode = mMode == Mode::mine ? Mode::place : Mode::mine;
  }
}