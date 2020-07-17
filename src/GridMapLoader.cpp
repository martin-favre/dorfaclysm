#include "GridMapLoader.h"
#include "Component.h"
#include "GameObject.h"
#include "GridMap.h"
#include "Serializer.h"

GridMapLoader::GridMapLoader(GameObject& owner) : Component(owner) {}
GridMapLoader::GridMapLoader(GameObject& owner, const SerializedObj& serObj)
    : Component(owner, serObj[SerializeString_Parent])
{
  GridMap::getActiveMap().loadActiveMap(serObj["gridmap"]);
}

SerializedObj GridMapLoader::serialize() const {
  SerializedObj obj = createSerializedObj<GridMapLoader>();
  obj[SerializeString_Parent] = Component::serialize();
  obj["gridmap"] = GridMap::getActiveMap();
  return obj;
}

std::string GridMapLoader::getTypeString() { return "GridMapLoader"; }
