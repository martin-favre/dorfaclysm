#include "MineBlockObject.h"

#include "GridActor.h"
#include "MineBlockComponent.h"
#include "Paths.h"
#include "RenderDepths.h"
#include "SpriteComponent.h"
#include "SpriteLoader.h"
MineBlockObject::MineBlockObject() {
  addComponent<MineBlockComponent>();
  addComponent<GridActor>(GridActor::job);
  addComponent<SpriteComponent>(Paths::RG_TILE, Vector2DInt{23, 3});
  setName("MineBlockObject");
  setRenderDepth(RenderDepths::JobIndication);
}
