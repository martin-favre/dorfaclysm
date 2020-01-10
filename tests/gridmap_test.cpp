#include "GridMap.h"

#include <gtest/gtest.h>

#include "AirBlock.h"
#include "DefaultBlock.h"
#include "Engine.h"
#include "GrassBlock.h"
#include "Vector2DInt.h"

class UnPassableBlock : public DefaultBlock {
 public:
  bool mayPassThrough() const override { return false; }
};

class PassableBlock : public DefaultBlock {
 public:
  bool mayPassThrough() const override { return true; }
};

void generateFlatWorld(GridMap& gridMap, const Vector3DInt& size) {
  for (int z = 0; z < size.z; ++z) {
    for (int y = 0; y < size.y; ++y) {
      for (int x = 0; x < size.x; ++x) {
        const Vector3DInt pos{x, y, z};
        if (z < size.z / 2) {
          gridMap.setBlockAt(pos, std::make_unique<UnPassableBlock>());
        } else {
          gridMap.setBlockAt(pos, std::make_unique<PassableBlock>());
        }
      }
    }
  }
}

class TestGridMap : public ::testing::Test {
 public:
  TestGridMap() : mGridmap(GridMap::getActiveMap()) {}
  void SetUp() override {
    if (mGridmap.getSize() != mSize)
      GridMap::generateActiveMap(mSize, generateFlatWorld);
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
