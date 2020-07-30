#include "StringToComponent.h"

#include "AirDepthRenderer.h"
#include "CameraControllerComponent.h"
#include "Component.h"
#include "DorfController.h"
#include "FpsCounter.h"
#include "GridActor.h"
#include "GridMapLoader.h"
#include "GridMapRenderer.h"
#include "Inventory.h"
#include "ItemContainer.h"
#include "MineBlockComponent.h"
#include "PlayerControllerComponent.h"
#include "RequestPoolComponent.h"
#include "ShowPositionComponent.h"
#include "Sprite.h"
#include "SpriteComponent.h"
#include "TextComponent.h"
#include "debug_closegamecomponent.hpp"

#define ENTRY(type) \
  { type::getTypeString(), &StringToComponent::unserializeComponent<type> }

const std::map<std::string, void (*)(GameObject&, const SerializedObj&)>
    StringToComponent::unserializeComponentMap = {
        ENTRY(SpriteComponent),
        ENTRY(GridActor),
        ENTRY(DorfController),
        ENTRY(CameraControllerComponent),
        ENTRY(ItemContainer),
        ENTRY(TextComponent),
        ENTRY(AirDepthRenderer),
        ENTRY(ShowPositionComponent),
        ENTRY(FpsCounter),
        ENTRY(Debug_CloseGameComponent),
        ENTRY(GridMapRenderer),
        ENTRY(PlayerControllerComponent),
        ENTRY(Inventory),
        ENTRY(GridMapLoader),
        ENTRY(RequestPoolComponent),
        ENTRY(MineBlockComponent)};