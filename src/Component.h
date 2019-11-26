#pragma once

class GameObject;

class Component {
public:
	Component(GameObject & user):mOwner(user) {}
	virtual void setup () {}
	virtual void update() {}
	virtual void render() {}
	virtual void teardown() {}
	virtual ~Component() {}
	bool & enabled();
	GameObject & owner() { return mOwner; }
	const GameObject & owner()const {return mOwner;}
private:
	bool mEnabled = true;

	GameObject& mOwner;
};