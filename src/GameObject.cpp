#include "GameObject.h"
#include "Engine.h"
GameObject::GameObject(GAMEOBJECT_ID id): mId(id) {
}

GameObject::~GameObject() {
	mComponents.clear();
}

bool & GameObject::enabled() {
	return mEnabled;
}

void GameObject::setup()
{
	for(auto& component : mComponents)
	{
		component->setup();
	}
}

void GameObject::teardown()
{
	for(auto& component : mComponents)
	{
		component->teardown();
	}
}

void GameObject::updateComponents() {
	if (!mEnabled) return;
	for(auto& component : mComponents)
	{
		component->update();
	}
}

void GameObject::render() {
	if (!mEnabled) return;
	for(auto& component : mComponents)
	{
		component->render();
	}
}

int GameObject::getRenderDepth() const
{
	return mRenderDepth;
}

const Vector2D& GameObject::getPosition() const
{
	return mPosition;
}

void GameObject::destroy() {
	Engine::removeGameObject(this);
}

GAMEOBJECT_ID GameObject::id()const {
	return mId;
}
std::string & GameObject::name() {
	return mName;
}
