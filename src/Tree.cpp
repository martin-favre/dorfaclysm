#include "Tree.h"
#include "SpriteComponent.h"
#include "SpriteLoader.h"
#include "GridActor.h"
#include "RenderDepths.h"
Tree::Tree(GAMEOBJECT_ID id) : GameObject(id){
  addComponent<SpriteComponent>(SpriteLoader::loadSpriteByIndex(
      Paths::RG_TILE_TRANSPARENT, {2, 1}, Paths::SIZE_OF_RG_TILE_TRANSPARENT));
  setRenderDepth(RenderDepths::Actors);
  name() = "Tree";
}