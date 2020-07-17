#include "BlockBuildObject.h"

#include "BlockBuildComponent.h"
#include "GridActor.h"
#include "Paths.h"
#include "RenderDepths.h"
#include "SpriteComponent.h"
#include "SpriteLoader.h"
BlockBuildObject::BlockBuildObject(GAMEOBJECT_ID id, const ItemType& item)
    : GameObject(id) {
  addComponent<BlockBuildComponent>(item);
  addComponent<GridActor>(GridActor::building);
  addComponent<SpriteComponent>(SpriteLoader::loadSpriteByIndex(
      Paths::RG_TILE_TRANSPARENT, {23, 3}, Paths::SIZE_OF_RG_TILE_TRANSPARENT));
  name() = "Block Building Site";
  setRenderDepth(RenderDepths::JobIndication);
}