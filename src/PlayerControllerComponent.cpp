#include "PlayerControllerComponent.h"
#include "GameObject.h"
#include "Camera.h"
#include "InputManager.h"
#include "GridMap.h"
#include "WorldItemComponent.h"
#include "WorldTile.h"
#include "JobPool.h"

void PlayerControllerComponent::setup()
{
  mTextComponent = owner().getComponent<TextComponent>();
}
/*
void PlayerControllerComponent::renderText()
{
  if(mTextComponent)
  {
    Vector2DInt mousePos = InputManager::getMousePosition();
    mousePos += Camera::get().getPosition();
    mousePos = Camera::renderPosToTilePos(mousePos);

    const GridMap& gridMap = GridMap::getActiveMap();
    std::string outStr;
    if(gridMap.isPosInMap(mousePos))
    {
      const WorldTile& worldTile = gridMap.getWorldTile(mousePos);

      const TileFloor& floor = worldTile.getFloor();
      outStr += floor.getName(); 
      const Tile* tile = worldTile.getTile();
      if(tile)
      {
        outStr += '\n' + tile->getName();
      }

      const std::vector<WorldItemComponent*>& comps = worldTile.getComponents();
      for(const auto& comp : comps)
      {
        ASSERT(comp, "Received null WorldItemComponent ptr");
        outStr += '\n' + comp->getName();
      }
      mTextComponent->setText(outStr);
    }  
  }
}
*/
/*
void PlayerControllerComponent::handleClick()
{
  Vector2DInt mousePos = InputManager::getMousePosition();
  mousePos += Camera::get().getPosition();
  mousePos = Camera::renderPosToTilePos(mousePos);
  const WorldTile& worldTile = GridMap::getActiveMap().getWorldTile(mousePos);
  const Tile* tile = worldTile.getTile();
  if(tile)
  {
    if(tile->supportsJob(PlayerRequestedJob::mine))
    {
      JobPool::addJob(PlayerRequestedJob(PlayerRequestedJob::mine, mousePos));
    }
  }
}
*/
void PlayerControllerComponent::update()
{
  // renderText();
  if(InputManager::getMouseDown(1))
  {
    // handleClick();
  }
}