#pragma once
#include <list>
#include <memory>

template <typename ItemType>
class VisibleList {
 public:
  void addItem(std::unique_ptr<ItemType>&& item) {
    mItems.emplace_back(std::move(item));
  }
  const std::list<std::unique_ptr<ItemType>>& getItems() const {
    return mItems;
  }

  std::unique_ptr<ItemType> claimItem(
      std::list<std::unique_ptr<int>>::const_iterator which) {
    ASSERT(which != mItems.end(), "iter out of bounds");
    int index = which - mItems.begin();
    std::unique_ptr<ItemType> item = std::move(mItems[index]);
    mItems.erase(which);
    return item;
  }

 private:
  std::list<std::unique_ptr<ItemType>> mItems;
};