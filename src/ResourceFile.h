#pragma once
#include <iostream>
#include <string>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include "Helpers.h"
#include <memory>
/*-------------------------------------------------------
	Contains the reference to one file-buffer
---------------------------------------------------------*/

class ResourceFile{
	public:
		ResourceFile(const void* data, size_t size);
		~ResourceFile();
		ResourceFile(const ResourceFile &) = delete;
		ResourceFile & operator=(const ResourceFile &) = delete;
		std::string getAsText() const;
		TTF_Font * getAsFont(int fontSize) const;
		SDL_Surface * getAsSurface() const;
		size_t getSize() const;
	private:
		SDL_RWops * mFile{nullptr};
		const size_t mSize{0};

		/* Need to keep track if it's a font to handle memory clearing properly */
		mutable bool mIsFont = false;
};