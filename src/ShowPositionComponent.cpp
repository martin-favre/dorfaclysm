#include "ShowPositionComponent.h"
#include "GameObject.h"
#include "Camera.h"
#include "TextComponent.h"
ShowPositionComponent::ShowPositionComponent(GameObject& owner) : Component(owner)
{}

void ShowPositionComponent::setup()
{
  mText = owner().getComponent<TextComponent>();
}
void ShowPositionComponent::update()
{
  if(mText)
  {
    std::stringstream ss;
    ss << Camera::get().getPosition();
    std::string stringText = ss.str();
    if(stringText != mText->getText()) mText->setText(stringText);
  }
}
