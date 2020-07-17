#include "StringToComponent.h"

#include "Component.h"
#include "GridActor.h"
#include "Sprite.h"
#include "SpriteComponent.h"
#include "RequestPoolComponent.h"
#include "DorfController.h"
#include "CameraControllerComponent.h"
#include "GridMapRenderer.h"
#include "debug_closegamecomponent.hpp"
#include "AirDepthRenderer.h"
#include "PlayerControllerComponent.h"
#include "TextComponent.h"
#include "ItemContainer.h"
#include "ShowPositionComponent.h"
#include "FpsCounter.h"
#include "GridMapLoader.h"

#define ENTRY(type) \
  { type::getTypeString(), &StringToComponent::unserializeComponent<type> }

const std::map<std::string, void (*)(GameObject&, const SerializedObj&)>
    StringToComponent::unserializeComponentMap = {ENTRY(SpriteComponent),
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
                                                  ENTRY(GridMapLoader),
                                                  ENTRY(RequestPoolComponent)};