#pragma once
typedef unsigned long GAMEOBJECT_ID;

#include <vector>
#include <string>
#include <map>
#include "Component.h"
#include "Vector2D.h"
class Component;
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
	template <typename component_type>
	component_type * add_component();


	/*-------------------------------------------------------
	Gets pointer to component of type.
	Returns nullptr if GameObject does not have a component.
	If GameObject has two of that component one of them will
	be returned. No Guarantee which one.
	---------------------------------------------------------
	@return pointer to component. nullptr if no component found.
	---------------------------------------------------------*/
	template <class component_type>
	component_type * get_component();

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
	void update_components();

	void update_collision();
	Vector2D mPosition;
	int mRenderDepth;
	bool m_enabled = true;
	bool m_dynamic = true;
	std::string m_name = "NoName";
	const GAMEOBJECT_ID m_id;
	std::vector<Component*> m_components;
};


template <typename component_type>
component_type * GameObject::add_component() {
	component_type * new_comp_type = new component_type(this);
	m_components.push_back(new_comp_type);
	return new_comp_type;
}

template <class component_type>
component_type * GameObject::get_component() {

	for (auto it = m_components.begin(); it != m_components.end(); ++it)
	{
		component_type* comp_cast = dynamic_cast<component_type*>(*it);
		if (comp_cast)
		{
			return comp_cast;
		}
	}
	return nullptr;
}
