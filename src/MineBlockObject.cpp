#include "MineBlockObject.h"

#include "GridActor.h"
#include "MineBlockComponent.h"
#include "Paths.h"
#include "RenderDepths.h"
#include "SpriteComponent.h"
#include "SpriteLoader.h"
MineBlockObject::MineBlockObject(GAMEOBJECT_ID id) : GameObject(id) {
  addComponent<MineBlockComponent>();
  addComponent<GridActor>(GridActor::job);
  addComponent<SpriteComponent>(SpriteLoader::loadSpriteByIndex(
      Paths::RG_TILE_TRANSPARENT, {23, 3}, Paths::SIZE_OF_RG_TILE_TRANSPARENT));
  name() = "MineBlockObject";
  setRenderDepth(RenderDepths::JobIndication);
}
