#pragma once
typedef unsigned long GAMEOBJECT_ID;

#include <vector>
#include <string>
#include <map>
#include "Component.h"
#include "Vector2D.h"
#include <memory>

class GameObject {
public:
	GameObject(GAMEOBJECT_ID id);
	virtual ~GameObject();

	/*-------------------------------------------------------
	Get/Set if the gameobject is enabled.
	---------------------------------------------------------
	@return reference to if the variable is enabled.
	---------------------------------------------------------*/
	bool & enabled();

	/*-------------------------------------------------------
	Add component of type.
	---------------------------------------------------------
	@return pointer to the component that was just added.
	---------------------------------------------------------*/
	template <typename componentType>
	componentType * addComponent();


	/*-------------------------------------------------------
	Gets pointer to component of type.
	Returns nullptr if GameObject does not have a component.
	If GameObject has two of that component one of them will
	be returned. No Guarantee which one.
	---------------------------------------------------------
	@return pointer to component. nullptr if no component found.
	---------------------------------------------------------*/
	template <class componentType>
	componentType * getComponent();

	int getRenderDepth() const;
	const Vector2D& getPosition() const;

	/*-------------------------------------------------------
	Orders the removal of the GameObject. It will be removed
	at the end of the current tick. 
	All pointers to this GameObject
	will then be invalid.
	---------------------------------------------------------*/
	void destroy();
	GAMEOBJECT_ID id()const;
	std::string & name();
protected:
	friend class Engine;
	/*-------------------------------------------------------
	Runs all components' setups. 
	---------------------------------------------------------*/
	void setup();

	/*-------------------------------------------------------
	Runs all components' teardowns.
	---------------------------------------------------------*/
	void teardown();
	
	/*-------------------------------------------------------
	Runs all components' renders.
	---------------------------------------------------------*/
	void render();
	
	/*-------------------------------------------------------
	Runs all components' update.
	---------------------------------------------------------*/
	void updateComponents();

	Vector2D mPosition;
	int mRenderDepth;
	bool mEnabled = true;
	std::string mName = "NoName";
	const GAMEOBJECT_ID mId;
	std::vector<std::unique_ptr<Component>> mComponents;
};


template <typename componentType>
componentType * GameObject::addComponent() {
	auto newComp = std::make_unique<componentType>(*this); 
	componentType* ptr = newComp.get();
	mComponents.push_back(std::move(newComp));
	return ptr;
}

template <class componentType>
componentType * GameObject::getComponent() {
	for(const auto& item : mComponents)
	{
		auto casted = std::dynamic_pointer_cast<componentType>(item);
		if(casted)
		{
			return casted;
		}
	}
	return nullptr;
}
