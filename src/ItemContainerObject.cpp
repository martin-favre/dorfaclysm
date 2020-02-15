#include "ItemContainerObject.h"
#include "GridActor.h"
#include "ItemContainer.h"
#include "SpriteComponent.h"
#include "SpriteLoader.h"
#include "Paths.h"
ItemContainerObject::ItemContainerObject(GAMEOBJECT_ID id) : GameObject(id) {
  addComponent<GridActor>(GridActor::item);
  addComponent<ItemContainer>();
  addComponent<SpriteComponent>(SpriteLoader::loadSpriteByIndex(Paths::DF_TILE, {7, 1}, Paths::SIZE_OF_DF_TILE));
  name() = "Item Container";
}
