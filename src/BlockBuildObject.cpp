#include "BlockBuildObject.h"

#include "BlockBuildComponent.h"
#include "GridActor.h"
#include "Paths.h"
#include "SpriteComponent.h"
#include "SpriteLoader.h"
#include "RenderDepths.h"
BlockBuildObject::BlockBuildObject(GAMEOBJECT_ID id, const ItemType& item)
    : GameObject(id) {
  addComponent<BlockBuildComponent>(item);
  addComponent<GridActor>(GridActor::building);
  addComponent<SpriteComponent>(SpriteLoader::loadSpriteByIndex(
      Paths::BROWN_SQUARE, {0, 0}, Paths::SIZE_OF_BROWN_SQUARE));
  name() = "Block Building Site";
  setRenderDepth(RenderDepth::JobIndication);
}