#include "MineBlockObject.h"
#include "MineBlockComponent.h"
#include "SpriteComponent.h"
#include "Paths.h"
#include "SpriteLoader.h"
#include "GridActor.h"
#include "RenderDepths.h"
MineBlockObject::MineBlockObject(GAMEOBJECT_ID id) : GameObject(id){
  addComponent<MineBlockComponent>();
  addComponent<GridActor>(GridActor::job);
  addComponent<SpriteComponent>(SpriteLoader::loadSpriteByIndex(
      Paths::BROWN_SQUARE, {0, 0}, Paths::SIZE_OF_BROWN_SQUARE));
  name() = "MineBlockObject";
  setRenderDepth(RenderDepth::JobIndication);
}
