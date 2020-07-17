#include "ItemContainerObject.h"

#include "GridActor.h"
#include "ItemContainer.h"
#include "Paths.h"
#include "RenderDepths.h"
#include "SpriteComponent.h"
#include "SpriteLoader.h"
ItemContainerObject::ItemContainerObject(GAMEOBJECT_ID id) : GameObject(id) {
  addComponent<GridActor>(GridActor::item);
  addComponent<ItemContainer>();
  addComponent<SpriteComponent>(SpriteLoader::loadSpriteByIndex(
      Paths::RG_TILE_TRANSPARENT, {8, 6}, Paths::SIZE_OF_RG_TILE_TRANSPARENT));
  setRenderDepth(RenderDepths::Actors);
  name() = "Item Container";
}
