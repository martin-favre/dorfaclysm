#include "Sprite.h"


Sprite::Sprite(SDL_Texture * new_text): m_texture(new_text) {
	int width = 0;
	int height = 0;
	SDL_QueryTexture(m_texture, NULL, NULL, &width, &height);
	m_rect = {0, 0, width, height};
}

Sprite::Sprite(SDL_Texture * new_text, const SDL_Rect & new_rect):
	m_texture(new_text),
	m_rect(new_rect) {}


SDL_Texture * Sprite::get_sdl_texture()const {
	return m_texture;
}

void Sprite::set_sdl_rect(int x, int y, int w, int h) {
	m_rect.x = x;
	m_rect.y = y;
	m_rect.w = w;
	m_rect.h = h;
}

const SDL_Rect * Sprite::get_sdl_rect() const {
	return &m_rect;
}
