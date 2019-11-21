#pragma once
#include <memory>
class Internals; // To hide SDL
class Texture
{
public:
	Texture();
private:
	std::unique_ptr<Internals> m_internals;
};