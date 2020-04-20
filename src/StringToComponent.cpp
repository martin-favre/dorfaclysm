#include "StringToComponent.h"
#include "Sprite.h"
#include "SpriteComponent.h"
#include "Component.h"
const std::map<std::string, void (*)(GameObject&, const SerializedObj&)>
    StringToComponent::unserializeComponentMap = {
        {SpriteComponent::getTypeName(),
         &StringToComponent::unserializeComponent<SpriteComponent>}};
