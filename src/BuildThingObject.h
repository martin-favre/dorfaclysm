#pragma once

#include "Block.h"
#include "BuildThingComponent.h"
#include "GameObject.h"
#include "GridActor.h"
#include "ItemType.h"
#include "Paths.h"
#include "RenderDepths.h"
#include "SpriteComponent.h"
#include "Vector2DInt.h"

class BuildThingObject : public GameObject {
 public:
  BuildThingObject(std::list<std::pair<ItemType, uint>>&& requiredItems,
                   std::function<void(const Vector3DInt&)> createThing) {
    addComponent<BuildThingComponent>(std::move(requiredItems), createThing);
    addComponent<Inventory>();
    int size = 3;
    addComponent<GridActor>(GridActor::ActorType::building,
                            Vector2DInt(size, size));
    setName("Building Site");
    setRenderDepth(RenderDepths::JobIndication);
    SpriteSheetInfo ssInfo{Paths::RG_TILE.mPath,
                           Paths::RG_TILE.mTileSize * size};
    addComponent<SpriteComponent>(ssInfo, Vector2DInt{7, 0});
  }
};