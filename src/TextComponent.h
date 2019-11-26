#pragma once
#include <string>
#include <SDL_ttf.h>
#include <cstdint>
#include "../basics/sprite.hpp"
#include "../basics/vector2d.hpp"
#include "../graphicsmanager.hpp"
#include "../gameobject.hpp"
#include "../filesystem/resourcearchive.hpp"
#include "../basics/helpers.hpp"

class TextComponent : public Component {
public:
	TextComponent(GameObject *);
	~TextComponent();
	void initialize(const std::string& pathToFont, int size = 16);
	void setFontSize(int);
	std::string getText();
	void setText(const std::string &);
	void setColor(const SDL_Color& color);
	void render() final override;
private:
	std::string mText{""};
	std::unique_ptr<Sprite> mSprite;
	std::unique_ptr<Font> mFont{nullptr};
	std::string mFontSource;
	Vector2D mScale{1, 1};
	double mAngle{0};
	bool mCentered{false};
	SDL_RendererFlip mFlip{SDL_FLIP_NONE};
	SDL_Color mColor{255, 255, 255, 1};
};