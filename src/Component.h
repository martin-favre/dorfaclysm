#pragma once

class GameObject;

class Component {
public:
	Component(GameObject * user):m_owner(user) {}
	virtual void setup () {}
	virtual void collision_step(){}
	virtual void update() {}
	virtual void render() {}
	virtual void teardown() {}
	virtual ~Component() {}
	bool & enabled();
	GameObject & owner() { return *(m_owner); }
	const GameObject & owner()const {return *(m_owner);}
protected:
	bool m_enabled = true;

	GameObject * const m_owner;
};