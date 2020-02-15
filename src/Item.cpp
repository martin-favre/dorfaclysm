#include "Item.h"

Item::Item(const Sprite& sprite, const std::string& name,
           const std::string& desc, uint worth, const ItemType& type)
    : mSprite(sprite), mName(name), mDesc(desc), mWorth(worth), mType(type) {}

const std::string& Item::getName() const { return mName; }
const std::string& Item::getDescription() const { return mDesc; }
const Sprite& Item::getSprite() const { return mSprite; }
uint Item::getWorth() const { return mWorth; }
