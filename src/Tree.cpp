#include "Tree.h"
#include "SpriteComponent.h"
#include "SpriteLoader.h"
#include "GridActor.h"
#include "RenderDepths.h"
Tree::Tree(){
  addComponent<SpriteComponent>(Paths::RG_TILE, Vector2DInt{2, 1});
  setRenderDepth(RenderDepths::Actors);
  name() = "Tree";
}