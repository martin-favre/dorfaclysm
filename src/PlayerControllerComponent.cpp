#include "PlayerControllerComponent.h"

#include <fstream>

#include "Block.h"
#include "BlockBuildComponent.h"
#include "BlockBuildObject.h"
#include "BlockBuildingRequestPool.h"
#include "Camera.h"
#include "Component.h"
#include "Engine.h"
#include "GameObject.h"
#include "GridActor.h"
#include "GridMap.h"
#include "GridMapHelpers.h"
#include "MineBlockComponent.h"
#include "MineBlockObject.h"
#include "MiningRequestPool.h"
#include "PlayerRequestType.h"
#include "Serializer.h"
const std::map<int, PlayerControllerComponent::Mode>
    PlayerControllerComponent::mKeyToMode = {
        {SDL_SCANCODE_1, mine},
        {SDL_SCANCODE_2, place},
        {SDL_SCANCODE_3, clear},
};

PlayerControllerComponent::PlayerControllerComponent(GameObject& gObj)
    : Component(gObj) {}

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
std::string PlayerControllerComponent::getTypeString() {
  return "PlayerControllerComponent";
}
PlayerControllerComponent::PlayerControllerComponent(
    GameObject& gObj, const SerializedObj& serObj)
    : Component(gObj, serObj["parent"]), mMode(serObj["mode"]) {}

SerializedObj PlayerControllerComponent::serialize() const {
  SerializedObj out = createSerializedObj<PlayerControllerComponent>();
  out["parent"] = Component::serialize();
  out["mode"] = mMode;
  return out;
}

void PlayerControllerComponent::handleClick() {
  if (!InputManager::getMouse(1)) return;
  Vector3DInt mousePos = InputManager::getMousePosition();
  mousePos += Camera::get().getPosition();
  mousePos = Camera::renderPosToTilePos(mousePos);
  GridMap& gridMap = GridMap::getActiveMap();
  if (!gridMap.isPosInMap(mousePos)) return;

  if (mMode == Mode::mine) {
    const Block& block = gridMap.getBlockAt(mousePos);
    if (block.supportsJob(requestTypeMining)) {
      for (const auto& actor : gridMap.getGridActorsAt(mousePos)) {
        // if we're not already building here
        if (actor->owner().hasComponent<MineBlockComponent>()) return;
      }
      GameObject& gObj = Engine::addGameObject<MineBlockObject>();
      gObj.setPosition(mousePos);
    }
  } else if (mMode == Mode::place) {
    Block& block = gridMap.getBlockAt(mousePos);
    if (block.supportsJob(requestTypePlacing)) {
      for (const auto& actor : gridMap.getGridActorsAt(mousePos)) {
        // if we're not already building here
        if (actor->owner().hasComponent<BlockBuildComponent>()) return;
      }
      GameObject& gObj = Engine::addGameObject<BlockBuildObject>(
          ItemType(ItemType::ItemType_RockBlockItem));
      gObj.setPosition(mousePos);
    }
  } else if (mMode == Mode::clear) {
    const auto& items = gridMap.getGridActorsAt(mousePos);
    for (auto& item : items) {
      if (item->owner().hasComponent<BlockBuildComponent>() ||
          item->owner().hasComponent<MineBlockComponent>()) {
        Engine::removeGameObject(&item->owner());
      }
    }
  }
}

void PlayerControllerComponent::update() {
  handleClick();

  if (InputManager::hasKeyEvents(mInputHandle)) {
    KeyEvent keyEvent = InputManager::dequeueKeyEvent(mInputHandle);
    if (keyEvent.mKeyDown) {
      if (mKeyToMode.count(keyEvent.mKey)) {
        mMode = mKeyToMode.at(keyEvent.mKey);
      } else if (keyEvent.mKey == SDL_SCANCODE_F5) {
        Engine::requestStateSave();
      } else if (keyEvent.mKey == SDL_SCANCODE_F9) {
        std::ifstream saveFile;
        saveFile.open("save.json", std::ios_base::binary);
        if (saveFile.is_open()) {
          std::stringstream ss;
          ss << saveFile.rdbuf();
          std::string contents = ss.str();
          Engine::requestLoadState(
              std::make_unique<SerializedObj>(SerializedObj::parse(contents)));
        }
        saveFile.close();
      }
    }
  }
  if (Engine::saveStateAvailable()) {
    std::unique_ptr<SerializedObj> save = Engine::getSaveState();
    std::ofstream saveFile;
    saveFile.open("save.json");
    saveFile << *save;
    saveFile.close();
  }
  InputManager::clearQueues(mInputHandle);
}

void PlayerControllerComponent::render() { renderText(); }
