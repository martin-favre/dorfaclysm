#include "ItemContainerObject.h"

#include "GridActor.h"
#include "ItemContainer.h"
#include "Paths.h"
#include "RenderDepths.h"
#include "SpriteComponent.h"
#include "SpriteLoader.h"
ItemContainerObject::ItemContainerObject() {
  addComponent<GridActor>(GridActor::item);
  addComponent<ItemContainer>();
  addComponent<SpriteComponent>(Paths::RG_TILE, Vector2DInt{8, 6});
  setRenderDepth(RenderDepths::Actors);
  setName("Item Container");
}
