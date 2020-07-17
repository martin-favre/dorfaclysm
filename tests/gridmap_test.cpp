#include "GridMap.h"

#include <gtest/gtest.h>

#include "Engine.h"
#include "Vector2DInt.h"
#include "helpers/gridmap_testhelpers.h"
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

  void TearDown() override {}

  Vector3DInt mSize{100, 100, 100};
  GridMap& mGridmap;
};

TEST_F(TestGridMap, getLowestPassablePositionFrom_TopOfWorld) {
  Vector3DInt from{0, 0, mSize.z - 1};
  Vector3DInt out;
  bool success = mGridmap.getLowestPassablePositionFrom(from, out);
  ASSERT_TRUE(success);
  Vector3DInt expected{0, 0, mSize.z / 2};
  ASSERT_EQ(out, expected);
}

TEST_F(TestGridMap, getLowestPassablePositionFrom_FromLastPassableBlock) {
  Vector3DInt from{0, 0, mSize.z / 2};
  Vector3DInt out;
  bool success = mGridmap.getLowestPassablePositionFrom(from, out);
  ASSERT_TRUE(success);
  Vector3DInt expected{0, 0, mSize.z / 2};
  ASSERT_EQ(out, expected);
}

TEST_F(TestGridMap, getLowestPassablePositionFrom_BelowLastPassableBlock) {
  Vector3DInt from{0, 0, mSize.z / 2 - 1};
  Vector3DInt out;
  bool success = mGridmap.getLowestPassablePositionFrom(from, out);
  ASSERT_FALSE(success);
}

TEST_F(TestGridMap, getLowestPassablePositionFrom_OneAboveBottomOfWorld) {
  Vector3DInt from{0, 0, 1};
  Vector3DInt out;
  bool success = mGridmap.getLowestPassablePositionFrom(from, out);
  ASSERT_FALSE(success);
}

TEST_F(TestGridMap, getLowestPassablePositionFrom_AtBottomOfWorld) {
  Vector3DInt from{0, 0, 0};
  Vector3DInt out;
  bool success = mGridmap.getLowestPassablePositionFrom(from, out);
  ASSERT_FALSE(success);
}
