#include "DorfController.h"

#include <gtest/gtest.h>

#include "GameObject.h"
#include "GridActor.h"
#include "MiningRequestPool.h"
#include "helpers/gridmap_testhelpers.h"
#include "Logging.h"
class TestGridMap : public ::testing::Test {
 public:
  TestGridMap() : mGridmap(GridMap::getActiveMap()) {}
  void SetUp() override {
    std::function<void(GridMap&, const Vector3DInt&)> func =
        [](GridMap& map, const Vector3DInt& size) {
          generateFlatWorld(map, size);
        };
    GridMap::generateActiveMap(mSize, func);
  }

  int getValidZ() { return mSize.z / 2; }

  void TearDown() override {}

  Vector3DInt mSize{100, 100, 100};
  GridMap& mGridmap;
};

bool areSerObjsEqual(const GameObject& g){
  SerializedObj obj = g.serialize();
  GameObject g2{obj};
  SerializedObj obj2 = g2.serialize();
  std::cout << obj.dump() << std::endl;
  std::cout << obj2.dump() << std::endl;
  return obj == obj2;
}

TEST_F(TestGridMap, serializeBasic) {
  GameObject g;
  g.addComponent<DorfController>();
  g.setup();
  ASSERT_TRUE(areSerObjsEqual(g));
}

TEST_F(TestGridMap, serializeMiningJob) {
  GameObject g;
  g.addComponent<DorfController>();
  g.addComponent<GridActor>(GridActor::dorf);
  g.setup();
  Vector3DInt pos{mSize.x / 2, mSize.y / 2, getValidZ()};
  BlockIdentifier blockident = mGridmap.getBlockIdentifier(pos);
  MiningRequestPool::getInstance().addRequest(
      std::make_unique<MiningRequest>(blockident, pos, g.getIdentifier()));
  g.updateComponents();
  SerializedObj obj = g.serialize();
  GameObject g2{obj};
  g2.setup();
  g2.updateComponents();
  SerializedObj obj2 = g2.serialize();
  
  std::cout << obj.dump() << std::endl;
  std::cout << obj2.dump() << std::endl;
  ASSERT_TRUE(obj == obj2);
}