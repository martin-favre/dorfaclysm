#include "BlockBuildObject.h"

#include "BlockBuildComponent.h"
#include "GridActor.h"
#include "Paths.h"
#include "RenderDepths.h"
#include "SpriteComponent.h"
#include "SpriteLoader.h"
BlockBuildObject::BlockBuildObject(const ItemType& item) {
  addComponent<BlockBuildComponent>(item);
  addComponent<GridActor>(GridActor::building);
  addComponent<SpriteComponent>(Paths::RG_TILE, Vector2DInt{23, 3});
  name() = "Block Building Site";
  setRenderDepth(RenderDepths::JobIndication);
}