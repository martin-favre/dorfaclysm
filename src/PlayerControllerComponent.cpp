#include "PlayerControllerComponent.h"
#include "GameObject.h"
#include "Camera.h"
#include "InputManager.h"
#include "GridMap.h"
#include "WorldItemComponent.h"

void PlayerControllerComponent::setup()
{
  mTextComponent = owner().getComponent<TextComponent>();
}
void PlayerControllerComponent::update()
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
      
      const TileFloor& floor = gridMap.getTileFloor(mousePos);
      outStr += floor.getName(); 
      const Tile* tile = gridMap.getTile(mousePos);
      if(tile)
      {
        outStr += '\n' + tile->getName();
      }

      const std::vector<WorldItemComponent*>& comps = gridMap.getComponentsOnTile(mousePos);
      for(const auto& comp : comps)
      {
        ASSERT(comp, "Received null WorldItemComponent ptr");
        outStr += '\n' + comp->getName();
      }
      mTextComponent->setText(outStr);
    }  
  }
}