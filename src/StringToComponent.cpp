#include "StringToComponent.h"

#include "Component.h"
#include "GridActor.h"
#include "Sprite.h"
#include "SpriteComponent.h"
#include "RequestPoolComponent.h"
#include "DorfController.h"

#define ENTRY(type) \
  { type::getTypeString(), &StringToComponent::unserializeComponent<type> }

const std::map<std::string, void (*)(GameObject&, const SerializedObj&)>
    StringToComponent::unserializeComponentMap = {ENTRY(SpriteComponent),
                                                  ENTRY(GridActor),
                                                  ENTRY(DorfController),
                                                  ENTRY(RequestPoolComponent)};